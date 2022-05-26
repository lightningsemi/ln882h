#ifndef  __LN_AT_SNTP_H__
#define  __LN_AT_SNTP_H__

#include <time.h>

typedef struct ln_sntp_cfg
{
	int sntpTimeZone;
	time_t tim;
}ln_sntp_cfg_t;

extern ln_sntp_cfg_t* ln_get_sntp_handle(void);


#endif /* __LN_AT_SNTP_H__ */
