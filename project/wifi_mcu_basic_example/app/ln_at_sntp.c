#include "ln_at_sntp.h"

#include "utils/debug/log.h"
#include "ln_utils.h"
#include "ln_at.h"
#include "ping.h"
#include "iperf.h"
#include <stdlib.h>
#include <string.h>

#include "hal/hal_rtc.h"
#include "hal/hal_misc.h"

#include "lwip/apps/sntp.h"

static ln_sntp_cfg_t s_sntpCfg = {0};

ln_sntp_cfg_t* ln_get_sntp_handle()
{
	return &s_sntpCfg;
}

void RTC_IRQHandler()
{
   if(hal_rtc_get_it_flag(RTC_BASE,RTC_IT_FLAG_ACTIVE) == 1)
   {
       hal_rtc_clr_it_flag(RTC_BASE,RTC_IT_FLAG_ACTIVE);      //清除标志位
	   if (ln_get_sntp_handle()->tim > 0)
	   {
		   ln_get_sntp_handle()->tim++;
	   }
   }
}

static void start_sntp_plus_timer(void)
{
    hal_misc_awo_set_o_cpu_sleep_counter_bp(0);
	OS_MsDelay(10);
	
	uint16_t rco32k_period = hal_misc_awo_get_i_rco32k_period_ns();
    
    rtc_init_t_def rtc_init;
    rtc_init.rtc_warp_en = RTC_WRAP_EN;                         
    hal_rtc_init(RTC_BASE,&rtc_init);
    
    //load计数到match,((32000-0)*(1/32000)) = 1s
	hal_rtc_set_cnt_match(RTC_BASE, ((1000 * 1000000.0f) / rco32k_period) + 0.5f  + 1);    	// 
    hal_rtc_set_cnt_load(RTC_BASE,0);                           
    hal_rtc_it_cfg(RTC_BASE,RTC_IT_FLAG_ACTIVE,HAL_ENABLE);     
    
    NVIC_SetPriority(RTC_IRQn,4);
    NVIC_EnableIRQ(RTC_IRQn);
    
    hal_rtc_en(RTC_BASE,HAL_ENABLE);                            
}

static void stop_sntp_plus_timer()
{
	hal_rtc_en(RTC_BASE,HAL_DISABLE);   
}

/**
 * @brief ln_at_set_sntp_cfg
 * Usage
 *   AT+CIPSNTPCFG=<EN>,<TIME_ZONE>
 * Options:
 *   <EN>		      		Enable or Disable
 *   <TIME_ZONE>       	    time zone [-11, 13]
 * eg
 *   AT+CIPSNTPCFG=1,8
 *   AT+CIPSNTPCFG=0
 * @param name
 * @return ln_at_err_t
 */
ln_at_err_t ln_at_set_sntp_cfg(uint8_t para_num, const char *name)
{
    LN_UNUSED(para_num);
    LN_UNUSED(name);

    bool is_default = false;
    uint8_t para_index = 1;
	int sntpEn = 0;

    if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &sntpEn))
    {
        goto __exit;
    }

    if (is_default)
    {
        goto __exit;
    }
	
    if (sntpEn == 0)
    {
        sntp_stop();
        ln_get_sntp_handle()->sntpTimeZone = 0;
        ln_get_sntp_handle()->tim = 0;
        stop_sntp_plus_timer();
        goto __suc;
    }
	
    if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(para_index++, &is_default, &ln_get_sntp_handle()->sntpTimeZone))
    {
        goto __exit;
    }

    if (is_default)
    {
        goto __exit;
    }
	
    if (ln_get_sntp_handle()->sntpTimeZone > 13 || ln_get_sntp_handle()->sntpTimeZone < -11)
    {
		ln_get_sntp_handle()->sntpTimeZone = 0;
        goto __exit;
    }
	
    if (sntp_enabled())
	{
		LOG(LOG_LVL_INFO, "sntp is already running!\r\n");
		goto __suc;
	}

    sntp_init();
    start_sntp_plus_timer();

__suc:
    ln_at_printf(LN_AT_RET_OK_STR);
    return LN_AT_ERR_NONE;

__exit:
    ln_at_printf(LN_AT_RET_ERR_STR);
    return LN_AT_ERR_COMMON;
}
LN_AT_CMD_REG(CIPSNTPCFG, NULL, ln_at_set_sntp_cfg, NULL, NULL);

/**
 * @brief ln_at_get_sntp
 * Usage
 *   AT+CIPSNTPTIME?
 * eg
 *   AT+CIPSNTPCFG=?
 *   +CIPSNTPTIME:Thu May 26 13:58:24 2022
 *
 *	 OK
 * @param name
 * @return ln_at_err_t
 */
ln_at_err_t ln_at_get_sntp(const char *name)
{
    LN_UNUSED(name);
	
	unsigned int currentTim = 0;
	unsigned int timePlus = ln_get_sntp_handle()->sntpTimeZone * 3600;

    if (sntp_enabled())
    {
        if (timePlus > 0)
        {
            currentTim = ln_get_sntp_handle()->tim + timePlus;
        }
        else
        {
            if (ln_get_sntp_handle()->tim >= timePlus)
            {
                currentTim = ln_get_sntp_handle()->tim - timePlus;
            }
            else
            {
                currentTim = 0;
            }
        }
    }
	else
	{
		goto __exit; 
	}

    ln_at_printf("+CIPSNTPTIME:%s", ctime(&currentTim));

    ln_at_printf(LN_AT_RET_OK_STR);
    return LN_AT_ERR_NONE;

__exit:
    ln_at_printf(LN_AT_RET_ERR_STR);
    return LN_AT_ERR_COMMON;
}
LN_AT_CMD_REG(CIPSNTPTIME, ln_at_get_sntp, NULL, NULL, NULL);