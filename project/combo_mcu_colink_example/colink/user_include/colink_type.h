#ifndef __COLINK_TYPE_H__
#define __COLINK_TYPE_H__

#include "wifi.h"
#include "netif/ethernetif.h"
#include "colink_link.h"

typedef enum 
{
    DEVICE_MODE_START          = 0,/**< 开始启动 */
    DEVICE_MODE_SETTING        = 1,/**< 进入ESP TOUCH配网模式 */
    DEVICE_MODE_SETTING_SELFAP = 2,/**< 进入AP配网模式 */
    DEVICE_MODE_WORK_NORMAL    = 3,/**< 进入正常工作模式 */
    DEVICE_MODE_UPGRADE        = 4,/**< 升级 */
	DEVICE_MODE_INIT,
	DEVICE_MODE_GET_DEVICEID,

    DEVICE_MODE_INVALID        = 255,/**< 初始无效的设备模式 */
}CoLinkDeviceMode;







#endif
