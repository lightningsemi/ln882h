/**
 * @file   ln_at_cmd_system.c
 * @author LightningSemi WLAN Team
 * Copyright (C) 2018-2020 LightningSemi Technology Co., Ltd. All rights reserved.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-01     MurphyZhao   the first version
 */

#include "ln_at.h"
#include "ln_at_cmd/ln_at_cmd_version.h"
#include "hal/hal_misc.h"
#include "wifi.h"
#include "ln_utils.h"
#include "ln882h.h"
#include "utils/reboot_trace/reboot_trace.h"

/**
 * name, get, set, test, exec
*/

/* declarations */
ln_at_err_t ln_at_exec_sys_rst(const char *name);
ln_at_err_t ln_at_exec_gmr(const char *name);

ln_at_err_t ln_at_exec_sys_rst(const char *name)
{
    LN_UNUSED(name);
    ln_chip_reboot();

    ln_at_printf(LN_AT_RET_OK_STR);
    return LN_AT_ERR_NONE;
}
LN_AT_CMD_REG(RST, NULL, NULL, NULL, ln_at_exec_sys_rst);

ln_at_err_t ln_at_exec_gmr(const char *name)
{
    LN_UNUSED(name);

    ln_at_printf("AT       version: v%d.%d.%d\r\n", LN_AT_VERSION, LN_AT_SUBVERSION, LN_AT_REVISION);
    ln_at_printf("AT CMD   version: v%d.%d.%d\r\n", LN_AT_CMD_VERSION, LN_AT_CMD_SUBVERSION, LN_AT_CMD_REVISION);
    ln_at_printf("SDK      version: v%s\r\n", LN882H_SDK_VERSION_STRING);
    ln_at_printf("WiFi lib version: %s[build time:%s]\r\n",
            wifi_lib_version_string_get(), wifi_lib_build_time_get());

    ln_at_printf(LN_AT_RET_OK_STR);
    return LN_AT_ERR_NONE;
}
LN_AT_CMD_REG(GMR, NULL, NULL, NULL, ln_at_exec_gmr);

ln_at_err_t ln_at_exec_ble_fw_start(const char *name)
{
    LN_UNUSED(name);

    ln_at_printf("Will jump to ble code area, can't jump back!\r\n"); 
    ln_at_printf(LN_AT_RET_OK_STR);

    hal_misc_awo_set_r_idle_reg(0xaeaebebe);
    __NVIC_SystemReset();
    while(1);
//    return LN_AT_ERR_NONE;
}
LN_AT_CMD_REG(BLE_START, NULL, NULL, NULL, ln_at_exec_ble_fw_start);
