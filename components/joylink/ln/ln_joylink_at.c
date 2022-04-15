#include "ln_nvds.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "osal/osal.h"
#include "ln_at.h"
#include "ln_joylink.h"
#include "ln_kv_key_def.h"
#include "ln_kv_err.h"

ln_at_err_t ln_at_start_joylink(const char *name)
{
    ln_start_joylink();
    ln_at_printf(LN_AT_RET_OK_STR);
}


ln_at_err_t ln_at_start_netconfig(const char *name)
{
    ln_enter_netconfig_mode();
    ln_at_printf(LN_AT_RET_OK_STR);
}

ln_at_err_t ln_at_cancel_ble_netconfig(const char *name)
{
    int ret = 0;
    ret = ln_cancel_ble_netconfig();
    if(ret == 0)
    {
        ln_at_printf(LN_AT_RET_OK_STR);
    }
    else
    {
        ln_at_printf(LN_AT_RET_ERR_STR);
    }
}

void ln_at_clear_feedid(void)
{
    ln_kv_del(KV_JOYLINK_INFO_CFG);
    ln_at_printf(LN_AT_RET_OK_STR);
    ln_at_printf("\r\nREBOOT\r\n");
    OS_Delay(1);
    ln_chip_reboot();
}

void ln_at_assert()
{
    ln_at_printf(LN_AT_RET_OK_STR);
    OS_MsDelay(10);
    LN_ASSERT(0);
}

LN_AT_CMD_REG(START_JOYLINK, NULL, NULL, NULL, ln_at_start_joylink);
LN_AT_CMD_REG(START_NETCONFIG, NULL, NULL, NULL, ln_at_start_netconfig);
LN_AT_CMD_REG(STOP_BLE_NETCONFIG, NULL, NULL, NULL, ln_at_cancel_ble_netconfig);
LN_AT_CMD_REG(CLEAR_FEEDID, NULL, NULL, NULL, ln_at_clear_feedid);
LN_AT_CMD_REG(ASSERT, NULL, NULL, NULL, ln_at_assert);