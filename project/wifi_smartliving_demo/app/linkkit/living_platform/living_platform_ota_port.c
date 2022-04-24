#include "iot_export.h"
#include "iot_import.h"

#include "ota_agent/ota_agent.h"
#include "ota_image.h"
#include "ln_nvds.h"
#include "flash_partition_table.h"
#include "utils/debug/log.h"

/**
 * @brief check ota image header, body.
 * @return return LN_TRUE on success, LN_FALSE on failure.
 */
static int ota_verify_download(void)
{
    partition_type_t type = PARTITION_TYPE_OTA;
    partition_info_t info;
    image_hdr_t      ota_header;

    if (LN_TRUE != ln_fetch_partition_info(type, &info)) {
        LOG(LOG_LVL_ERROR, "FAIL -- read OTA partition info!!!\r\n");
        return LN_FALSE;
    }

    if ( OTA_ERR_NONE != image_header_read(type, &ota_header) ) {
        LOG(LOG_LVL_ERROR, "FAIL -- read OTA header.\r\n");
        return LN_FALSE;
    }
    LOG(LOG_LVL_INFO, "PASS -- read OTA header.\r\n");

    if ( OTA_ERR_NONE != image_header_verify(&ota_header) ) {
        LOG(LOG_LVL_ERROR, "FAIL -- verify OTA header.\r\n");
        return LN_FALSE;
    }
    LOG(LOG_LVL_INFO, "PASS -- verify OTA header.\r\n");

    if ( OTA_ERR_NONE != image_body_verify(info.start_addr, &ota_header)) {
        LOG(LOG_LVL_ERROR, "FAIL -- verify OTA body.\r\n");
        return LN_FALSE;
    }

    LOG(LOG_LVL_INFO, "PASS -- verify OTA image content.\r\n");
    return LN_TRUE;
}

static void update_ota_state(void)
{
    ln_nvds_set_ota_upg_state(UPG_STATE_DOWNLOAD_OK);
}


static void linkkit_reboot(void *p)
{
    HAL_Reboot();
}

/**
 * @brief
 *
 * @return int return 0 on success, others on failure.
 */
int ota_part_verify(void)
{
    if (ota_verify_download() == LN_FALSE) {
        LOG(LOG_LVL_ERROR, "LN OTA partition verify FAILED!!!\r\n");
        return -1;
    }

    LOG(LOG_LVL_ERROR, "LN OTA partition verify SUCCEED!!!\r\n");
    update_ota_state();
    return 0;
}

/**
 * @brief
 *
 */
void ota_reboot_start(void)
{
    const int DELAY_TIME_MS = 3000;
    void *reset_timer = NULL;

    LOG(LOG_LVL_ERROR, "APP will reboot in %d ms\r\n", DELAY_TIME_MS);

    reset_timer = HAL_Timer_Create("reboot", linkkit_reboot, NULL);
    HAL_Timer_Stop(reset_timer);
    HAL_Timer_Start(reset_timer, DELAY_TIME_MS);
}

