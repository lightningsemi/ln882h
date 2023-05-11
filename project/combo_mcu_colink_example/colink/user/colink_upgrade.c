#include "colink_global.h"
#include "colink_define.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/sockets.h"
#include "ln_types.h"
#include "mbedtls/sha256.h"
#include "colink_profile.h"
#include "colink_error.h"
#include <string.h>

#include "utils/debug/log.h"

#include "colink_ota_driver_port.h"


#define IOTGO_UPGRADE_BIN1_START_SECTOR (0x01)
#define IOTGO_UPGRADE_BIN2_START_SECTOR (0x81)

typedef struct
{
    char version[12];
    char introduction[32];
    int32_t progress;
    int32_t bootTime;
} UpdateState;

static OTAFileInfo ota_file_info;
static uint32_t sector_start = 0;
static char upgrade_seq[20] = "";

static void iotgoUpgradeModeStart(void)//
{
	http_ota_task_creat((void*)(&ota_file_info));
}

static bool getServerInfo(char *url, char *ip, uint16_t *port, char *path)//
{
    char *t1, *t2, *t3;
    char port_str[8] = "";

    t1 = strstr(url, "//");
    t2 = strstr(t1, ":");

    if(NULL == t1 || NULL == t2)
    {
        os_printf("getServerInfo");
        return false;
    }

    memcpy(ip, t1+2, t2-(t1+2));

    t3 = strstr(t2, "/");

    if(NULL == t3)
    {
        os_printf("getServerInfo");
        return false;
    }

    memcpy(port_str, t2+1, t3-(t2+1));

    *port = atoi(port_str);

    strcpy(path, t3);

    return true;
}

void colinkUpgradeRequest(char *new_ver, ColinkOtaInfo file_list[], uint8_t file_num, char *sequence)
{
    os_printf("colinkUpgradeRequest,");

    strcpy(upgrade_seq, sequence);

    if(file_num != 2)
    {
        os_printf("sever send error quantity");
        colinkUpgradeRes(COLINK_OTA_MODEL_ERROR, upgrade_seq);
        return;
    }

    int user_bin = 1;//always download user1.bin

    if(user_bin == 0x00)
    {
        if(0 == strcmp(file_list[1].name, "user2.bin"))
        {
            sector_start = IOTGO_UPGRADE_BIN2_START_SECTOR;
            strcpy(ota_file_info.digest, file_list[1].digest);
            getServerInfo(file_list[1].download_url, ota_file_info.ip, &(ota_file_info.port), ota_file_info.path);
        }
        else
        {
            os_printf("err user2.bin");
            colinkUpgradeRes(COLINK_OTA_MODEL_ERROR, upgrade_seq);
            return;
        }
    }
    else
    {
        if(0 == strcmp(file_list[0].name, "user1.bin"))
        {
            sector_start = IOTGO_UPGRADE_BIN1_START_SECTOR;
            strcpy(ota_file_info.digest, file_list[0].digest);
            getServerInfo(file_list[0].download_url, ota_file_info.ip, &(ota_file_info.port), ota_file_info.path);
        }
        else
        {
            os_printf("err user1.bin");
            colinkUpgradeRes(COLINK_OTA_MODEL_ERROR, upgrade_seq);
            return;
        }
    }

    os_printf("version=[%s]\r\n", ota_file_info.version);
    os_printf("ip=[%s]\r\n", ota_file_info.ip);
    os_printf("path=[%s]\r\n", ota_file_info.path);
    os_printf("digest=[%s]\r\n", ota_file_info.digest);
    os_printf("port=[%d]\r\n", ota_file_info.port);
    os_printf("length=[%d]\r\n", ota_file_info.file_length);
    os_printf("start download sector is %d\n", sector_start);
	
    iotgoUpgradeModeStart();
}

