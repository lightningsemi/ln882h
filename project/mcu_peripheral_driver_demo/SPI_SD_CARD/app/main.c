/**
 * @file     main.c
 * @author   BSP Team 
 * @brief    
 * @version  0.0.0.1
 * @date     2021-08-05
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#include "hal/hal_common.h"
#include "ln_show_reg.h"

#include "ln_test_common.h"

#include "ln_sd_type.h"
#include "ln_drv_spi_sd.h"
#include "ff.h"			        /* Declarations of FatFs API */
#include "diskio.h"		        /* Declarations of device I/O functions */
#include "utils/debug/log.h"


static unsigned char tx_data[100];
static unsigned char rx_data[100];

static unsigned char status[2];
static unsigned int err_cnt = 0;


static FATFS fs;           /* Filesystem object */
static FIL fil;            /* File object */
static DIR dp;
static FRESULT res;        /* API result code */
static UINT i;
static FILINFO Fileinfo;

int main (int argc, char* argv[])
{  
    /****************** 1. 系统初始化 ***********************/
    SetSysClock();
    log_init();   
    LOG(LOG_LVL_INFO,"ln882H init! \n");
    ln_show_reg_init();

    /****************** 2. spi sd 测试***********************/
    spi_pin_cfg_t spi_pin_cfg;
    memset(&spi_pin_cfg,0,sizeof(spi_pin_cfg));

    spi_pin_cfg.gpio_clk_port  = SPI_GPIO_A;
    spi_pin_cfg.gpio_clk_pin   = GPIO_PIN_0;

    spi_pin_cfg.gpio_mosi_port = SPI_GPIO_B;
    spi_pin_cfg.gpio_mosi_pin  = GPIO_PIN_5;

    spi_pin_cfg.gpio_miso_port = SPI_GPIO_B;
    spi_pin_cfg.gpio_miso_pin  = GPIO_PIN_4;

    spi_pin_cfg.gpio_cs_port   = SPI_GPIO_A;
    spi_pin_cfg.gpio_cs_pin    = GPIO_PIN_12;

    if(ln_drv_spi_sd_init(&spi_pin_cfg) != true){
        LOG(LOG_LVL_INFO,"TF card error! \n");
    }


    FRESULT res;
    BYTE work[FF_MAX_SS];

    /* 1. 挂载文件系统 */
	res = f_mount(&fs, "0:", 1);
	if (res){
		LOG(LOG_LVL_INFO,"Can not load FAT FS.\r\n");
        return false;
	}else{
		LOG(LOG_LVL_INFO,"FAT FS init success.\r\n");
	}
    
    res = f_opendir(&dp, (const TCHAR*)"0:");
	
	if (res == FR_NO_PATH) {
        LOG(LOG_LVL_INFO,"Dir does not exist.\r\n");
        return false;
    }
    
    if (res != FR_OK) {
        LOG(LOG_LVL_INFO,"Dir can not open.\r\n");
        return false;
    }else{
        while(1){
            res = f_readdir(&dp, &Fileinfo);
            if(res != FR_OK){
                LOG(LOG_LVL_INFO,"Read dir failed.Dir can not open.\r\n");
                return false;
            }else if(Fileinfo.fname[0] == 0){
                break;		// 读取失败或者读取完所有条目
            }else if (Fileinfo.fname[0] == '.')	{// 隐藏文件
                continue;
            }else if(Fileinfo.fattrib & AM_DIR)	{
                LOG(LOG_LVL_INFO,"File:\n");
                continue;	// 文件夹不处理，继续读下一个
            }else{   
                LOG(LOG_LVL_INFO,"%s\n\r\n" ,Fileinfo.fname);
            }  
        }
    }
    f_mount(0, "0:", 0);
    //文件写入有问题，暂时没有写操作的Demo.
}
