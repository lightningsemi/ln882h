/**
 * @file     ln_drv_sht30.c
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2022-08-04
 * 
 * @copyright Copyright (c) 2022 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */


#include "hal/hal_i2c.h"

#include "hal/hal_gpio.h"
#include "ln882h.h"

#include "ln_drv_sht30.h"

#define TIMEOUT_CYCLE 2000
/**************************************I2C INIT START********************************************************/
void ln_i2c_init()
{
    hal_gpio_pin_afio_select(GPIOB_BASE, GPIO_PIN_6, I2C0_SCL); //初始化I2C引脚
    hal_gpio_pin_afio_select(GPIOB_BASE, GPIO_PIN_7, I2C0_SDA);
    hal_gpio_pin_afio_en(GPIOB_BASE, GPIO_PIN_6, HAL_ENABLE); //使能功能复用
    hal_gpio_pin_afio_en(GPIOB_BASE, GPIO_PIN_7, HAL_ENABLE);

    /* disable the i2c */
    hal_i2c_en(I2C_BASE, HAL_DISABLE); //配置参数前要先失能I2C

    /* init the i2c */
    i2c_init_t_def i2c_init;
    memset(&i2c_init, 0, sizeof(i2c_init));

    i2c_init.i2c_peripheral_clock_freq = 4;                     //时钟设置，默认4就可以（滤波使用）
    i2c_init.i2c_master_mode_sel = I2C_FM_MODE;                 //fs   = 1
    i2c_init.i2c_fm_mode_duty_cycle = I2C_FM_MODE_DUTY_CYCLE_2; //duty = 1
    /*
        快速模式下：400k SCL周期为2.5us
        外设工作时钟为80MHZ 所以Tpckl = 1/80M = 0.0125us
        
        duty = 1;fs = 1
        Thigh = 9  x CCR x TPCLK1 
        Tlow  = 16 x CCR x TPCLK1
        Thigh + Tlow = 2.5us
        ccr =  8
    */
    i2c_init.i2c_ccr = 8;
    i2c_init.i2c_trise = 0xF; //推荐默认值为0xF;

    hal_i2c_init(I2C_BASE, &i2c_init);

    hal_i2c_en(I2C_BASE, HAL_ENABLE);
}
/**
 * @brief I2C 写数据到目标设备的寄存器（设备地址7bit）
 * 
 * @param i2c_x_base      I2C寄存器基址
 * @param dev_addr        设备地址
 * @param target_addr     目标设备寄存器地址
 * @param target_addr_len 寄存器地址长度
 * @param buf             要写入的数据
 * @param buf_len         要写入数据的长度
 * @return uint8_t        是否写入成功，0：不成功，1：成功
 */
uint8_t hal_i2c_master_7bit_write_target_address(uint32_t i2c_x_base, uint8_t dev_addr, uint8_t *target_addr, uint8_t target_addr_len, const uint8_t *buf, uint16_t buf_len)
{
    //1. check busy
    if (hal_i2c_wait_bus_idle(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
    {
        hal_i2c_master_reset(i2c_x_base);
        return HAL_RESET;
    }

    //2. send start
    if (hal_i2c_master_start(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //3. send addr
    hal_i2c_master_send_data(i2c_x_base, dev_addr);

    //4. wait send complete
    if (hal_i2c_master_wait_addr(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //5. clear addr flag
    hal_i2c_clear_sr(i2c_x_base);

    //6. send target address
    for (uint16_t i = 0; i < target_addr_len; i++)
    {
        //wait tx empty flag
        if (hal_i2c_wait_txe(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        {
            return HAL_RESET;
        }
        else
        {
            hal_i2c_master_send_data(i2c_x_base, target_addr[i]);
        }
    }

    //7. wait send complete.
    if (hal_i2c_wait_btf(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //8. send data
    for (uint32_t i = 0; i < buf_len; i++)
    {
        //wait tx empty flag
        if (hal_i2c_wait_txe(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        {
            return HAL_RESET;
        }
        else
        {
            hal_i2c_master_send_data(i2c_x_base, buf[i]);
        }
    }

    //9. wait send complete.
    if (hal_i2c_wait_btf(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //10. stop the i2c.
    hal_i2c_master_stop(i2c_x_base);

    return HAL_SET;
}
/**
 * @brief I2C 读目标设备的寄存器（设备地址7bit）
 * 
 * @param i2c_x_base        I2C寄存器基址      
 * @param dev_addr          设备地址
 * @param target_addr       目标设备寄存器地址
 * @param target_addr_len   目标设备寄存器地址长度
 * @param buf               读取的数据
 * @param buf_len           读取的数据长度
 * @return uint8_t          是否读取成功，0：不成功，1：成功
 */
uint8_t hal_i2c_master_7bit_read_target_address(uint32_t i2c_x_base, uint8_t dev_addr, uint8_t *target_addr, uint8_t target_addr_len, uint8_t *buf, uint16_t buf_len)
{
    //1. check busy
    if (hal_i2c_wait_bus_idle(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
    {
        hal_i2c_master_reset(i2c_x_base);
        return HAL_RESET;
    }

    //2. send start
    if (hal_i2c_master_start(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //3. send addr
    hal_i2c_master_send_data(i2c_x_base, dev_addr);

    //4. wait send complete
    if (hal_i2c_master_wait_addr(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //5. clear addr flag
    hal_i2c_clear_sr(i2c_x_base);

    //6. send target address
    for (uint16_t i = 0; i < target_addr_len; i++)
    {
        //wait tx empty flag
        if (hal_i2c_wait_txe(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        {
            return HAL_RESET;
        }
        else
        {
            hal_i2c_master_send_data(i2c_x_base, target_addr[i]);
        }
    }

    //7. wait send complete.
    if (hal_i2c_wait_btf(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //8. reset the bus,if not do this,the bus can not run start operation
    hal_i2c_master_reset(i2c_x_base);

    //9. send start
    if (hal_i2c_master_start(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //10. send addr (+1 is read operation)
    hal_i2c_master_send_data(i2c_x_base, dev_addr + 1);

    //11. Wait for an ack after sending the address
    if (hal_i2c_master_wait_addr(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //12. clear addr flag
    hal_i2c_clear_sr(i2c_x_base);

    //13. clear the DR
    hal_i2c_master_recv_data(i2c_x_base);

    //14. receive data
    for (int i = buf_len; i > 0; i--)
    {
        //when reading the last byte,do not send the ack
        if (buf_len == 1)
        {
            //do not send the ack
            hal_i2c_ack_en(i2c_x_base, HAL_DISABLE);

            ///wait rx not empty
            if (hal_i2c_wait_rxne(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
            {
                return HAL_RESET;
            }
            else
            {
                *buf = hal_i2c_master_recv_data(i2c_x_base);
            }
            //read data
        }
        else
        {
            //send ack
            hal_i2c_ack_en(i2c_x_base, HAL_ENABLE);

            //wait rx not empty
            if (hal_i2c_wait_rxne(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
            {
                return HAL_RESET;
            }
            else
            {
                *buf = hal_i2c_master_recv_data(i2c_x_base);
            }
        }
        buf_len--;
        buf++;
    }

    //15. stop the i2c.
    hal_i2c_master_stop(i2c_x_base);

    return HAL_SET;
}

/**************************************I2C W OLED END********************************************************/

//第四部分，读取SHT30温湿度传感器
/**************************************I2C SHT30 INIT********************************************************/
unsigned char crc8_checksum(unsigned char *ptr, unsigned char len)
{
    unsigned char bit;        // bit mask
    unsigned char crc = 0xFF; // calculated checksum
    unsigned char byteCtr;    // byte counter

    // calculates 8-Bit checksum with given polynomial
    for (byteCtr = 0; byteCtr < len; byteCtr++)
    {
        crc ^= (ptr[byteCtr]);
        for (bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x131;
            else
                crc = (crc << 1);
        }
    }

    return crc;
}
/**
 * @brief I2C 写数据（设备地址7bit）
 * 
 * @param i2c_x_base I2C寄存器基址
 * @param dev_addr   设备地址
 * @param buf        要写入的数据
 * @param buf_len    要写入数据的长度
 * @return uint8_t   是否写入成功，0：不成功，1：成功
 */
uint8_t hal_i2c_master_7bit_write(uint32_t i2c_x_base, uint8_t dev_addr, const uint8_t *buf, uint16_t buf_len)
{
    //1. check busy
    if (hal_i2c_wait_bus_idle(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
    {
        hal_i2c_master_reset(i2c_x_base);
        return HAL_RESET;
    }

    //2. send start
    if (hal_i2c_master_start(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //3. send addr
    hal_i2c_master_send_data(i2c_x_base, dev_addr);

    //4. wait send complete
    if (hal_i2c_master_wait_addr(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //5. clear addr flag
    hal_i2c_clear_sr(i2c_x_base);

    //6. send data
    for (uint32_t i = 0; i < buf_len; i++)
    {
        //wait tx empty flag
        if (hal_i2c_wait_txe(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        {
            return HAL_RESET;
        }
        else
        {
            hal_i2c_master_send_data(i2c_x_base, buf[i]);
        }
    }

    //7. wait send complete.
    if (hal_i2c_wait_btf(i2c_x_base, TIMEOUT_CYCLE) == HAL_RESET)
        return HAL_RESET;

    //8. stop the i2c.
    hal_i2c_master_stop(i2c_x_base);

    return HAL_SET;
}


unsigned char ln_tem_hum_init()
{
    ln_i2c_init();

    unsigned char data[2];
    unsigned char res = 0;
    data[0] = ((CMD_MEAS_PERI_2_M >> 8) & 0x00FF);
    data[1] = ((CMD_MEAS_PERI_2_M >> 0) & 0x00FF);
    res = hal_i2c_master_7bit_write(I2C_BASE, TEM_HUM_ADDR, data, 2);
    return res;
}

unsigned char tem_hum_read_sensor_data(unsigned char *data)
{
    unsigned char addr_data[10];
    addr_data[0] = ((CMD_FETCH_DATA >> 8) & 0x00FF);
    addr_data[1] = ((CMD_FETCH_DATA >> 0) & 0x00FF);
    hal_i2c_master_7bit_read_target_address(I2C_BASE, TEM_HUM_ADDR, addr_data, 2, data, 6);

    if (data[2] != crc8_checksum(data, 2))
    {
        return 1;
    }

    if (data[5] != crc8_checksum(data + 3, 2))
    {
        return 1;
    }

    return 0;
}

/**************************************I2C SHT30 END********************************************************/