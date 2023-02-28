/**
 * @file     ln_drv_spi_sd.c
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2023-01-05
 * 
 * @copyright Copyright (c) 2023 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#include "ln_drv_spi_sd.h"

uint32_t spi_cs_base = 0;
gpio_pin_t spi_cs_pin;

#define LN_SPI_CS_LOW               hal_gpio_pin_reset(spi_cs_base,spi_cs_pin)
#define LN_SPI_CS_HIGH              hal_gpio_pin_set(spi_cs_base,spi_cs_pin)


void ln_drv_spi_init(spi_pin_cfg_t *spi_pin)
{
    uint32_t gpio_base = 0;
    if(spi_pin->gpio_clk_port == SPI_GPIO_A)
        gpio_base = GPIOA_BASE;
    else if(spi_pin->gpio_clk_port == SPI_GPIO_B)
        gpio_base = GPIOB_BASE;

    hal_gpio_pin_afio_select(gpio_base,spi_pin->gpio_clk_pin,SPI0_CLK);
    hal_gpio_pin_afio_en(gpio_base,spi_pin->gpio_clk_pin,HAL_ENABLE);

    if(spi_pin->gpio_mosi_port == SPI_GPIO_A)
        gpio_base = GPIOA_BASE;
    else if(spi_pin->gpio_mosi_port == SPI_GPIO_B)
        gpio_base = GPIOB_BASE;

    hal_gpio_pin_afio_select(gpio_base,spi_pin->gpio_mosi_pin,SPI0_MOSI);
    hal_gpio_pin_afio_en(gpio_base,spi_pin->gpio_mosi_pin,HAL_ENABLE);

    if(spi_pin->gpio_miso_port == SPI_GPIO_A)
        gpio_base = GPIOA_BASE;
    else if(spi_pin->gpio_miso_port == SPI_GPIO_B)
        gpio_base = GPIOB_BASE;

    hal_gpio_pin_afio_select(gpio_base,spi_pin->gpio_miso_pin,SPI0_MISO);
    hal_gpio_pin_afio_en(gpio_base,spi_pin->gpio_miso_pin,HAL_ENABLE);

    if(spi_pin->gpio_cs_port == SPI_GPIO_A)
        gpio_base = GPIOA_BASE;
    else if(spi_pin->gpio_cs_port == SPI_GPIO_B)
        gpio_base = GPIOB_BASE;
    
    spi_cs_base = gpio_base;
    spi_cs_pin  = spi_pin->gpio_cs_pin;
    
    gpio_init_t_def gpio_init;
    memset(&gpio_init,0,sizeof(gpio_init));
    gpio_init.dir = GPIO_OUTPUT;
    gpio_init.pin = spi_cs_pin;
    gpio_init.speed = GPIO_HIGH_SPEED;
    hal_gpio_init(spi_cs_base,&gpio_init);
    hal_gpio_pin_set(spi_cs_base,spi_cs_pin);

    spi_init_type_def spi_init;
    memset(&spi_init,0,sizeof(spi_init));

    spi_init.spi_baud_rate_prescaler = SPI_BAUDRATEPRESCALER_2;         //设置波特率
    spi_init.spi_mode      = SPI_MODE_MASTER;                           //设置主从模式
    spi_init.spi_data_size = SPI_DATASIZE_8B;                           //设置数据大小
    spi_init.spi_first_bit = SPI_FIRST_BIT_MSB;                         //设置帧格式
    spi_init.spi_cpol      = SPI_CPOL_HIGH;                             //设置时钟极性
    spi_init.spi_cpha      = SPI_CPHA_2EDGE;                            //设置时钟相位
    hal_spi_init(SPI0_BASE,&spi_init);                                  //初始化SPI
    
    hal_spi_en(SPI0_BASE,HAL_ENABLE);                                   //SPI使能
    hal_spi_ssoe_en(SPI0_BASE,HAL_DISABLE);                             //关闭CS OUTPUT
}

void ln_drv_spi_send_and_recv_data(uint8_t *send_data,uint8_t *recv_data,uint32_t size)
{
    LN_SPI_CS_LOW;
    for(int i = 0;i < size; i++){
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,send_data[i]);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        recv_data[i] = hal_spi_recv_data(SPI0_BASE);
    }
    LN_SPI_CS_HIGH;
}

bool ln_drv_sd_power_on()
{
    //发送初始化80个同步时钟（协议上是74个）
    LN_SPI_CS_HIGH;
    for(int i = 0;i < 10; i++){
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,0xFF);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        hal_spi_recv_data(SPI0_BASE);
    }
    LN_SPI_CS_HIGH;
    ln_delay_ms(100);
    return true;
}

bool ln_drv_sd_send_cmd0()
{
    //发送CMD0
    uint8_t send_data[50];
    uint8_t recv_data[50];
    memset(send_data,0xFF,50);
    
    send_data[0] = SD_CMD0;
    send_data[1] = 0x00;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = 0x00;
    send_data[5] = 0x95;    //crc
    ln_drv_spi_send_and_recv_data(send_data,recv_data,8);  //send CMD0
    
    if(recv_data[7] == 0x01){
        return true;
    }else{
        return false;
    }
}

bool ln_drv_sd_send_cmd8()
{
    //发送CMD8
    uint8_t send_data[50];
    uint8_t recv_data[50];
    memset(send_data,0xFF,50);
    
    send_data[0] = SD_CMD8;
    send_data[1] = 0x00;
    send_data[2] = 0x00;
    send_data[3] = 0x01;
    send_data[4] = 0xAA;
    send_data[5] = 0x87;    //crc
    ln_drv_spi_send_and_recv_data(send_data,recv_data,12);  //send CMD8
    
    if(recv_data[7] == 0x01){
        return true;
    }else{
        return false;
    }
}

bool ln_drv_sd_send_cmd55()
{
    //发送CMD8
    uint8_t send_data[50];
    uint8_t recv_data[50];
    memset(send_data,0xFF,50);
    
    send_data[0] = SD_CMD55;
    send_data[1] = 0x00;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = 0x00;
    send_data[5] = 0xFF;    
    ln_drv_spi_send_and_recv_data(send_data,recv_data,8);  
    
    if(recv_data[7] == 0x01){
        return true;
    }else{
        return false;
    }
}

bool ln_drv_sd_send_cmd58()
{
    //发送CMD8
    uint8_t send_data[50];
    uint8_t recv_data[50];
    memset(send_data,0xFF,50);
    
    send_data[0] = SD_CMD58;
    send_data[1] = 0x00;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = 0x00;
    send_data[5] = 0xFF;    
    ln_drv_spi_send_and_recv_data(send_data,recv_data,12);  
    
    if(recv_data[7] == 0x01){
        return true;
    }else{
        return false;
    }
}

bool ln_drv_sd_send_acmd41()
{
    //发送CMD8
    uint8_t send_data[50];
    uint8_t recv_data[50];
    memset(send_data,0xFF,50);
    
    send_data[0] = SD_ACMD41;
    send_data[1] = 0x40;
    send_data[2] = 0x00;
    send_data[3] = 0x00;
    send_data[4] = 0x00;
    send_data[5] = 0xFF;  
    ln_drv_spi_send_and_recv_data(send_data,recv_data,12);  
    
    if(recv_data[7] == 0x00){
        return true;
    }else{
        return false;
    }
}

bool ln_drv_spi_sd_init(spi_pin_cfg_t *spi_pin)
{
    ln_drv_spi_init(spi_pin);
    
    /****************SD卡初始化******************************/
    //1. 发送初始化80个同步时钟（协议上是74个）
    if(ln_drv_sd_power_on() != true){
        return false;
    }
    
    //2. 发送CMD0,判断SD卡版本
    if(ln_drv_sd_send_cmd0() != true){
        return false;
    }
    
    //3. 发送CMD8,设置电压范围
    if(ln_drv_sd_send_cmd8() != true){
        return false;
    }
    
    //5. 发送CMD58读取OCR（电源信息）
    ln_drv_sd_send_cmd58();
    
    //6. 发送ACMD41,判断SD卡状态是否初始化成功。
    while(1){
        /*设置应用命令*/
        ln_drv_sd_send_cmd55();
        /**/
        if(ln_drv_sd_send_acmd41() == true){
            break;
        }
    }
    
    //7. 发送CMD58读取OCR（电源信息）,正确的情况下可以发现OCR[31:30]为3，说明是大容量卡并且卡已经正确上电
    ln_drv_sd_send_cmd58();
    return true;
}

bool ln_drv_spi_sd_read_block(uint32_t addr,uint8_t *data)
{
    uint8_t send_data[8];
    uint8_t *recv_data = data;
    uint32_t cur_len = 0;
    memset(send_data,0xFF,8);
    
    send_data[0] = SD_CMD17;
    send_data[1] = addr >> 24;
    send_data[2] = addr >> 16;
    send_data[3] = addr >> 8;
    send_data[4] = (addr >> 0) ;
    send_data[5] = 0X01;  
    
    LN_SPI_CS_LOW;
    for(int i = 0;i < 8; i++){
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,send_data[i]);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        recv_data[i] = hal_spi_recv_data(SPI0_BASE);
    }
    
    if(recv_data[7] != 0x00){
        return false;
    }
    while(1){
        if(cur_len > 100000){
            return false;
        }
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,0xFF);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        recv_data[0] = hal_spi_recv_data(SPI0_BASE);
        cur_len++;
        if(recv_data[0] == 0xFE){
            break;
        }
    }
    
    for(int i = 0; i < 512; i++){
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,0xFF);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        recv_data[i] = hal_spi_recv_data(SPI0_BASE);
    }
    
    while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
    hal_spi_send_data(SPI0_BASE,0xFF);
    while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
    hal_spi_recv_data(SPI0_BASE);

    while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
    hal_spi_send_data(SPI0_BASE,0xFF);
    while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
    hal_spi_recv_data(SPI0_BASE);
    
    LN_SPI_CS_HIGH;
    //uint16_t crc16 = crc16_ccitt((char*)recv_data,512);
    return true;
}
bool ln_drv_spi_sd_read(uint32_t addr,uint8_t *data,uint32_t len)
{
    uint32_t cur_size = 0;
    uint32_t try_cnt  = 0;
    while(1){
    
        if(ln_drv_spi_sd_read_block(addr,data+cur_size*SD_BLOCK_SIZE) == true)
        {
            addr++;
            cur_size ++;
            if(cur_size >= len){
                break;
            }
            try_cnt = 0;
        }
        if(try_cnt > 10){
            return false;
        }
    }
    return true;
}


bool ln_drv_spi_sd_write(uint32_t addr,uint8_t *data,uint32_t len)
{
    uint8_t send_data[8];
    uint8_t *recv_data = data;
    memset(send_data,0xFF,8);
    
    send_data[0] = SD_CMD24;
    send_data[1] = addr >> 24;
    send_data[2] = addr >> 16;
    send_data[3] = addr >> 8;
    send_data[4] = (addr >> 0) ;
    send_data[5] = 0X01;  
    
    LN_SPI_CS_LOW;
    for(int i = 0;i < 8; i++){
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,send_data[i]);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        recv_data[i] = hal_spi_recv_data(SPI0_BASE);
    }
    
    if(recv_data[7] != 0x00){
        return false;
    }
    
    for(int i = 0; i < 512; i++){
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
        hal_spi_send_data(SPI0_BASE,i);
        while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
        recv_data[i] = hal_spi_recv_data(SPI0_BASE);
    }
    
    while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_TXE) != HAL_SET);
    hal_spi_send_data(SPI0_BASE,0xFF);
    while(hal_spi_get_status_flag(SPI0_BASE,SPI_STATUS_FLAG_RXNE) != HAL_SET);
    recv_data[0] = hal_spi_recv_data(SPI0_BASE);
   
    LN_SPI_CS_HIGH;
    if((recv_data[0] & 0x1F) != 0x05) 
        return false;
    //uint16_t crc16 = crc16_ccitt((char*)recv_data,512);
    return true;

}























