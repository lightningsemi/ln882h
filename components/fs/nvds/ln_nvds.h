#ifndef __LN_NVDS_H__
#define __LN_NVDS_H__

//Non-Volatile Data Store[NVDS]
#include "stdint.h"

#ifndef SIZE_4KB
#define SIZE_4KB                 (0x1000)
#endif

/*----------------------------------------------------------*/
/*                      Data Mapping                        */
/*----------------------------------------------------------*/
/*                    !!! WARNING !!!                       */
/*       Warning: Vendor defines the following macro,       */
/*       users cannot modify these macro definition!!!      */
/*       We store important and infrequently modified       */
/*       SDK data in NVDS.                                  */
/*----------------------------------------------------------*/
#define SECT_MAGIC_CODE_OFFSET         (0)
#define SECT_MAGIC_CODE_LEN            (4)

#define NV0_NVDS_VERSION_OFFST         (SECT_MAGIC_CODE_OFFSET + SECT_MAGIC_CODE_LEN)
#define NV0_NVDS_VERSION_LEN           (16)
#define NV0_NVDS_VERSION_STR           ("[Ver 1.0]")

#define NV1_OTA_UPG_STATE_OFFST        (NV0_NVDS_VERSION_OFFST + NV0_NVDS_VERSION_LEN)
#define NV1_OTA_UPG_STATE_LEN          (4)

#define NV2_XTAL_COMP_VAL_OFFST        (NV1_OTA_UPG_STATE_OFFST + NV1_OTA_UPG_STATE_LEN)
#define NV2_XTAL_COMP_VAL_LEN          (1)

#define NV3_TX_POWER_COMP_OFFST        (NV2_XTAL_COMP_VAL_OFFST + NV2_XTAL_COMP_VAL_LEN)
#define NV3_TX_POWER_COMP_LEN          (1)

#define NV4_CHIP_SN_OFFSET             (NV3_TX_POWER_COMP_OFFST + NV3_TX_POWER_COMP_LEN)
#define NV4_CHIP_SN_LEN                (20)

#define NV5_BOOT_LOG_UART_BR_OFFSET    (NV4_CHIP_SN_OFFSET + NV4_CHIP_SN_LEN)
#define NV5_BOOT_LOG_UART_BR_LEN       (1)

#define NV6_BOOT_LOG_EN_OFFSET         (NV5_BOOT_LOG_UART_BR_OFFSET + NV5_BOOT_LOG_UART_BR_LEN)
#define NV6_BOOT_LOG_EN_LEN            (1)
#define NV6_BOOT_LOG_EN                ('e')
#define NV6_BOOT_LOG_DIS               (0xFF)

#define NV7_BOOT_DELAY_MS_OFFSET       (NV6_BOOT_LOG_EN_OFFSET + NV6_BOOT_LOG_EN_LEN)
#define NV7_BOOT_DELAY_MS_LEN          (2)

#define NV8_BOOT_DELAY_EN_OFFSET       (NV7_BOOT_DELAY_MS_OFFSET + NV7_BOOT_DELAY_MS_LEN)
#define NV8_BOOT_DELAY_EN_LEN          (1)
#define NV8_BOOT_DELAY_EN              ('e')
#define NV8_BOOT_DELAY_DIS             (0xFF)

//If vendor add new macro into the nvds,please update it!
#if ((NV8_BOOT_DELAY_EN_OFFSET + NV8_BOOT_DELAY_EN_LEN) > SIZE_4KB)
  #error "NVDS area overflow!!!"
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef enum nvds_err_en {
    NVDS_ERR_OK   = 0,
    NVDS_ERR_FAIL = -1,
} nvds_err_t;

/* ----------------------- nvds api ---------------------------- */
int    ln_nvds_init(uint32_t base);
char * ln_nvds_version(void);
int    ln_nvds_write(uint32_t offset, uint8_t* data, uint16_t len);
int    ln_nvds_read(uint32_t offset, uint8_t* data, uint16_t len);

int    ln_nvds_set_ota_upg_state(uint32_t  state);
int    ln_nvds_get_ota_upg_state(uint32_t *state);
       
int    ln_nvds_set_xtal_comp_val(uint8_t  val);
int    ln_nvds_get_xtal_comp_val(uint8_t *val);
       
int    ln_nvds_set_tx_power_comp(uint8_t  val);
int    ln_nvds_get_tx_power_comp(uint8_t *val);
       
int    ln_nvds_set_chip_sn(uint8_t *sn);
int    ln_nvds_get_chip_sn(uint8_t *sn_buf, uint8_t buf_len);
       
int    ln_nvds_set_boot_log_uart_baudrate(uint32_t baudrate);
int    ln_nvds_get_boot_log_uart_baudrate(uint32_t *baudrate);
       
int    ln_nvds_set_boot_log_en(uint8_t  val);
int    ln_nvds_get_boot_log_en(uint8_t *val);
       
int    ln_nvds_set_boot_delay_ms(uint16_t  ms);
int    ln_nvds_get_boot_delay_ms(uint16_t *ms);
       
int    ln_nvds_set_boot_delay_en(uint8_t  val);
int    ln_nvds_get_boot_delay_en(uint8_t *val);


#ifdef __cplusplus
}
#endif

#endif /* __LN_NVDS_H__ */
