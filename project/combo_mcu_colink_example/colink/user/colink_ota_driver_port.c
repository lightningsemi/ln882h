#include "colink_ota_driver_port.h"
#include <stdint.h>
#include "Freertos.h"
#include "timers.h"

#include "hal/hal_flash.h"
#include "flash_partition_table.h"
#include "netif/ethernetif.h"
#include "proj_config.h"
#include "utils/debug/ln_assert.h"
#include "osal/osal.h"

#include "ota_port.h"
#include "ota_image.h"
#include "ota_types.h"
#include "ln_nvds.h"

#include "colink_global.h"

#include "httpsclient.h"
#include "httpsclient_wrapper.h"
#include "httpsclient_debug.h"
#include "colink_mcu_driver_port.h"
#include "colink_xcrypt_driver_port.h"

#define HTTP_OTA_DEMO_STACK_SIZE   (1024 * 16)
#define SECTOR_SIZE_4KB            (1024 * 4)

static char g_http_uri_buff[256] = "";

// a block to save http data.
static char *temp4K_buf    = NULL;
static int   temp4k_offset = 0;

// where to save OTA data in flash.
static int32_t flash_ota_start_addr  = OTA_SPACE_OFFSET;
static int32_t flash_ota_offset      = 0;
static uint8_t is_persistent_started = LN_FALSE;
static uint8_t is_ready_to_verify    = LN_FALSE;
static uint8_t is_precheck_ok        = LN_FALSE;
static OS_Thread_t g_httpota_thread;
static OS_Timer_t upgrade_timer;

void  ln_chip_reboot(void);

static void cb2UpgradeTimer(void *arg)//
{
    static uint8_t conut = 0;

    if(conut++ < 2)
    {
        Log_i("upgrade not yet complete now!\n");

    }
    else
    {
        Log_i("upgrade successfully and reboot now!");
        OS_TimerStop(&upgrade_timer);
		OS_TimerDelete(&upgrade_timer);
        ln_chip_reboot();
    }
}

/**
 * @brief Pre-check the image file to be downloaded.
 *
 * @attention None
 *
 * @param[in]  app_offset  The offset of the APP partition in Flash.
 * @param[in]  ota_hdr     pointer to ota partition info struct.
 *
 * @return  whether the check is successful.
 * @retval  #LN_TRUE     successful.
 * @retval  #LN_FALSE    failed.
 */
static int ota_download_precheck(uint32_t app_offset, image_hdr_t * ota_hdr)
{
    image_hdr_t *app_hdr = NULL;
    if (NULL == (app_hdr = OS_Malloc(sizeof(image_hdr_t)))) {
        LOG(LOG_LVL_ERROR, "[%s:%d] malloc failed.\r\n", __func__, __LINE__);
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_header_fast_read(app_offset, app_hdr) ) {
        LOG(LOG_LVL_ERROR, "failed to read app header.\r\n");
        goto ret_err;
    }

    if ((ota_hdr->image_type == IMAGE_TYPE_ORIGINAL) || \
        (ota_hdr->image_type == IMAGE_TYPE_ORIGINAL_XZ)) 
    {
        // check version
        if (((ota_hdr->ver.ver_major << 8) + ota_hdr->ver.ver_minor) == \
            ((app_hdr->ver.ver_major << 8) + app_hdr->ver.ver_minor)) {
            LOG(LOG_LVL_ERROR, "[%s:%d] same version, do not upgrade!\r\n", __func__, __LINE__);
            goto ret_err;
        }
        
        // check file size
        if (((ota_hdr->img_size_orig + sizeof(image_hdr_t)) > APP_SPACE_SIZE) || \
            ((ota_hdr->img_size_orig_xz + sizeof(image_hdr_t)) > OTA_SPACE_SIZE)) {
            LOG(LOG_LVL_ERROR, "[%s:%d] size check failed.\r\n", __func__, __LINE__);
            goto ret_err;
        }
    } else {
        //image type not support!
        goto ret_err;
    }

    OS_Free(app_hdr);
    return LN_TRUE;
    
    ret_err:
    OS_Free(app_hdr);
    return LN_FALSE;
}


static int ota_persistent_start(void)
{
    if ( NULL == temp4K_buf ) {
        temp4K_buf = OS_Malloc(SECTOR_SIZE_4KB);
        if ( NULL == temp4K_buf) {
            LOG_EMSG("failed to alloc 4KB!!!\r\n");
            return LN_FALSE;
        }
        memset(temp4K_buf, 0, SECTOR_SIZE_4KB);
    }

    temp4k_offset = 0;
    flash_ota_start_addr = OTA_SPACE_OFFSET;
    flash_ota_offset = 0;
    is_persistent_started = LN_TRUE;
    return LN_TRUE;
}

/**
 * @brief Save block to flash.
 *
 * @param buf
 * @param buf_len
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_persistent_write(const char *buf, const int32_t buf_len)
{
    int part_len = 0; // [0, 1, 2, ..., 4K-1], 0, 1, 2, ..., (part_len-1)

    if (!is_persistent_started) {
        return LN_TRUE;
    }

    if (temp4k_offset + buf_len < SECTOR_SIZE_4KB) {
        // just copy all buf data to temp4K_buf
        memcpy(temp4K_buf + temp4k_offset, buf, buf_len);
        temp4k_offset += buf_len;
        part_len = 0;
    } else {
        // just copy part of buf to temp4K_buf
        part_len = temp4k_offset + buf_len - SECTOR_SIZE_4KB;
        memcpy(temp4K_buf + temp4k_offset, buf, buf_len - part_len);
        temp4k_offset += buf_len - part_len;
    }

    if (temp4k_offset >= (SECTOR_SIZE_4KB - 1) ) {
        // write to flash
        LOG(LOG_LVL_INFO, "write at flash: 0x%08x\r\n", flash_ota_start_addr + flash_ota_offset);
        
        if(flash_ota_offset == 0) {
            if (LN_TRUE != ota_download_precheck(APP_SPACE_OFFSET, (image_hdr_t *) temp4K_buf)) {
                LOG(LOG_LVL_INFO, "ota download precheck failed!\r\n");
                is_precheck_ok = LN_FALSE;
                return LN_FALSE;
            }
            is_precheck_ok = LN_TRUE;
        }
        
        hal_flash_erase(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB);
        hal_flash_program(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB, (uint8_t *)temp4K_buf);

        flash_ota_offset += SECTOR_SIZE_4KB;
        memset(temp4K_buf, 0, SECTOR_SIZE_4KB);
        temp4k_offset = 0;
    }

    if (part_len > 0) {
        memcpy(temp4K_buf + temp4k_offset, buf + (buf_len - part_len), part_len);
        temp4k_offset += part_len;
    }

    return LN_TRUE;
}

/**
 * @brief save last block and clear flags.
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_persistent_finish(void)
{
    if (!is_persistent_started) {
        return LN_FALSE;
    }

    // write to flash
    Log_i("write at flash: 0x%08x\r\n", flash_ota_start_addr + flash_ota_offset);
    hal_flash_erase(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB);
    hal_flash_program(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB, (uint8_t *)temp4K_buf);

    OS_Free(temp4K_buf);
    temp4K_buf = NULL;
    temp4k_offset = 0;

    flash_ota_offset = 0;
    is_persistent_started = LN_FALSE;
    return LN_TRUE;
}

static int http_get_callback(const char *recvbuf, int32_t recvbuf_len, int32_t total_bytes, int8_t is_recv_finished)
{
    if (!is_recv_finished) {
        if (LN_TRUE != ota_persistent_write(recvbuf, recvbuf_len)) {
            return -1;
        }
    } else {
        ota_persistent_finish();
        is_ready_to_verify = LN_TRUE;
        Log_i("cb info: recv %d finished, no more data to deal with.\r\n", total_bytes);
    }
    return recvbuf_len;
}

/**
 * @brief check ota image header, body.
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_verify_download(void)
{
    image_hdr_t ota_header;

    if ( OTA_ERR_NONE != image_header_fast_read(OTA_SPACE_OFFSET, &ota_header) ) {
        Log_e("failed to read ota header.");
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_header_verify(&ota_header) ) {
        Log_e("failed to verify ota header.");
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_body_verify(OTA_SPACE_OFFSET, &ota_header)) {
        Log_e("failed to verify ota body.");
        return LN_FALSE;
    }

    Log_i("Succeed to verify OTA image content.");
    return LN_TRUE;
}

static int update_ota_state(void)
{
    upg_state_t state = UPG_STATE_DOWNLOAD_OK;
    ln_nvds_set_ota_upg_state(state);
    return LN_TRUE;
}

static void http_ota_entry(void *params)
{
    uint8_t digest_hex[65] = {0};
    char ts[16] = {0};
    int ret_code = 0;
    HTTP_INFO *http_info = NULL;
    char *url_str = NULL;
	char src_sign[10 + 10 + 36 + 1] = {0};
    int recv_total = 0;
	OTAFileInfo *pOta = (OTAFileInfo*)params;
	
	lwip_itoa(ts, sizeof(ts), get_colink_mcu_handle()->get_timep());
	Log_i("DEVICEID=[%s]", (char *)get_colink_xcrypt_handle()->colinkId);
    Log_i("ts=[%s]", ts);
    Log_i("APIKEY=[%s]", (char *)get_colink_xcrypt_handle()->colinkApikey);

	strcpy(src_sign, (char *)get_colink_xcrypt_handle()->colinkId);
	strcat(src_sign, ts);
	strcat(src_sign, (char *)get_colink_xcrypt_handle()->colinkApikey);
	colinkSha256((uint8_t*)src_sign, (uint32_t)strlen(src_sign), (char*)digest_hex);

    sprintf(g_http_uri_buff, "http://%s:%d%s?deviceid=%s&ts=%s&sign=%s", 
					pOta->ip, pOta->port, pOta->path, 
					(char *)get_colink_xcrypt_handle()->colinkId, ts, digest_hex);
	Log_i("http_get=[%s]\r\n", g_http_uri_buff);
	url_str = g_http_uri_buff;

    while(NETDEV_LINK_UP != netdev_get_link_state(netdev_get_active())){
        OS_MsDelay(500);
    }
    
	coLinkSetDeviceMode(DEVICE_MODE_UPGRADE);
    Log_i("http start.!!!\r\n");
    while(1) {

        if (NULL == (http_info = httpinfo_new())) {
            Log_e("malloc for @http_info failed.\r\n");
            ret_code = -1;
            goto flag_exit;
        }

        if (0 != http_init(&http_info, FALSE)) {
            Log_e("http init failed.!!!\r\n");
            ret_code = -1;
            goto flag_err;
        }

        url_str = g_http_uri_buff;

        if (LN_TRUE == ota_persistent_start()) {
            recv_total = http_get_with_callback(http_info, url_str, http_get_callback);
        }

        if (recv_total > 0) {
            Log_e("total received %d bytes.\r\n", recv_total);
            ret_code = 0;
        } 
        else if((recv_total < 0) && (is_precheck_ok != LN_TRUE)) {
            ret_code = -2;
        } 
        else {
            ret_code = -1;
            Log_e("http_get error!!!\r\n");
        }

        http_close(http_info);

flag_err:
        httpinfo_delete(http_info);

flag_exit:

        OS_SecDelay(5);
        if (ret_code != 0) {
            if (ret_code == -2) {
                break;
            } else {
                // retry if something is wrong.
                LOG_EMSG("http client is restart...\r\n");
            }
        } else {
            // just download once if everything is ok.
            break;
        }
    }

    Log_i("http client job done, exit...\r\n");
    if (LN_TRUE == is_precheck_ok)
    {
        if ( (LN_TRUE == is_ready_to_verify) && (LN_TRUE == ota_verify_download()) ) {
            update_ota_state();
			
			//start reboot tmr
			OS_TimerCreate(&upgrade_timer, OS_TIMER_PERIODIC, (OS_TimerCallback_t)cb2UpgradeTimer, NULL, 1000);
			OS_TimerStart(&upgrade_timer);
            //ln_chip_reboot();
        }
    }

    OS_ThreadDelete(NULL);
}


void http_ota_task_creat(void* para)
{
	if (OS_OK != OS_ThreadCreate(&g_httpota_thread, "http_ota", http_ota_entry, para, OS_PRIORITY_NORMAL, HTTP_OTA_DEMO_STACK_SIZE))
    {
        LN_ASSERT(1);
    }
}
