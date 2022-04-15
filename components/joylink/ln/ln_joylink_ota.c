#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "ln_joylink.h"
#include "ota_types.h"
#include "flash_partition_table.h"
#include "ota_err.h"
#include "crc32.h"

static int jl_verify_download(image_hdr_t * ota_header)
{
    int offset;
    int size;
    int start_addr = OTA_SPACE_OFFSET;
    uint32_t i = 0;
	uint32_t crc32_result = 0;
	crc32_ctx_t crc_ctx = {0};
    unsigned char temp_buf[128];

    LOG(LOG_LVL_INFO, "jl_verify_download.\r\n");
    if ( OTA_ERR_NONE != image_header_verify(ota_header) ) {
        LOG(LOG_LVL_ERROR, "failed to verify ota header.\r\n");
        return -1;
    }

    offset = start_addr + sizeof(image_hdr_t);
    switch (ota_header->image_type)
    {
        case IMAGE_TYPE_ORIGINAL:
            size = ota_header->img_size_orig;
            break;
        case IMAGE_TYPE_ORIGINAL_XZ:
            size = ota_header->img_size_orig_xz;
            break;
        case IMAGE_TYPE_DIFF:
            size = ota_header->img_size_diff;
            break;
        case IMAGE_TYPE_DIFF_XZ:
            size = ota_header->img_size_diff_xz;
            break;
        default:
            return OTA_ERR_IMG_TYPE;
    }
    ln_crc32_init(&crc_ctx);
    
    for (i = 0; i < size/128; i++) {
        hal_flash_read(offset, 128, temp_buf);
        ln_crc32_update(&crc_ctx, temp_buf, 128);
        offset += 128;
    }
    hal_flash_read(offset, size % 128, temp_buf);
    ln_crc32_update(&crc_ctx, temp_buf, size % 128);
    crc32_result = ln_crc32_final(&crc_ctx);
    
    /* check the CRC32 checksum */
    switch (ota_header->image_type)
    {
        case IMAGE_TYPE_ORIGINAL:
            return ((crc32_result == ota_header->img_crc32_orig) ? OTA_ERR_NONE : OTA_ERR_IMG_ENTITY_VERIFY);

        case IMAGE_TYPE_ORIGINAL_XZ:
            return ((crc32_result == ota_header->img_crc32_orig_xz) ? OTA_ERR_NONE : OTA_ERR_IMG_ENTITY_VERIFY);

        case IMAGE_TYPE_DIFF:
            return ((crc32_result == ota_header->img_crc32_diff) ? OTA_ERR_NONE : OTA_ERR_IMG_ENTITY_VERIFY);

        case IMAGE_TYPE_DIFF_XZ:
            return ((crc32_result == ota_header->img_crc32_diff_xz) ? OTA_ERR_NONE : OTA_ERR_IMG_ENTITY_VERIFY);

        default:
            return OTA_ERR_IMG_TYPE;
    }
}


int jl_check_ota(void)
{
    int ret = -1;
    image_hdr_t * ota_hdr = NULL;

    do
    {
        if (NULL == (ota_hdr = OS_Malloc(sizeof(image_hdr_t)))) {
            LOG(LOG_LVL_ERROR, "[%s:%d] malloc failed.\r\n", __func__, __LINE__);
            break;
        }
        
        hal_flash_read(OTA_SPACE_OFFSET, sizeof(image_hdr_t),ota_hdr);

        if ((ota_hdr->image_type == IMAGE_TYPE_ORIGINAL) || \
            (ota_hdr->image_type == IMAGE_TYPE_ORIGINAL_XZ)) 
        {
            if(jl_verify_download(ota_hdr) == OTA_ERR_NONE)
            {
                ret = 0;
                LOG(LOG_LVL_INFO, "[%s:%d] jl_check_ota succeed.\r\n", __func__, __LINE__);
                break;
            }
        } 
        else 
        {
            //image type not support!
            break;
        }
    }while(0);

    if(ota_hdr != NULL)
    {
        OS_Free(ota_hdr);
    }
    return ret;
}

void jl_start_update(void)
{
    upg_state_t state = UPG_STATE_DOWNLOAD_OK;
    ln_nvds_set_ota_upg_state(state);
    LOG(LOG_LVL_INFO, "[%s:%d]\r\n", __func__, __LINE__);
    OS_Delay(3);
    ln_chip_reboot();
}

