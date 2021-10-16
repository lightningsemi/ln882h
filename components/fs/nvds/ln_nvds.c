#include "proj_config.h"
#include "ln_nvds.h"
#include "hal/hal_flash.h"

static uint32_t ln_nvds_base = 0;
static char     ln_nvds_ver[NV0_NVDS_VERSION_LEN] = {0,};

#define USR_NVDS_SECT1_OFFSET    (ln_nvds_base)
#define USR_NVDS_SECT2_OFFSET    (USR_NVDS_SECT1_OFFSET + SIZE_4KB)
#define USR_NVDS_FLAG_OFFSET     (USR_NVDS_SECT2_OFFSET + SIZE_4KB)

#define NVDS_SECT_MAGIC_CODE          (0x5344564E)/* "NVDS" */
#define NVDS_NONE_TAG                 (0xFF)

typedef enum nvds_valid_sect {
    NVDS_SECT1_VALID = 0xA5A5,
    NVDS_SECT2_VALID = 0x5A5A,
} nvds_valid_sect_t;

static void nvds_read_flash(uint32_t offset, uint32_t len, uint8_t *buf)
{
#if (defined(FLASH_XIP) && (FLASH_XIP == 1))
    hal_flash_read_by_cache(offset, len, buf);
#else
    hal_flash_read(offset, len, buf);
#endif
}

static void nvds_write_flash(uint32_t offset, uint32_t len, uint8_t *buf)
{
    hal_flash_program(offset, len, buf);
}

static void nvds_erase_flash(uint32_t offset, uint32_t len)
{
    hal_flash_erase(offset, len);
}

static uint16_t nvds_get_valid_flag(void)
{
    uint16_t flag = 0;
    nvds_read_flash(USR_NVDS_FLAG_OFFSET, sizeof(uint16_t), (uint8_t *)&flag);
    return flag;
}
static void nvds_set_valid_flag(nvds_valid_sect_t sect)
{
    uint16_t flag = sect;
    nvds_write_flash(USR_NVDS_FLAG_OFFSET, sizeof(uint16_t), (uint8_t *)&flag);
}

static uint32_t nvds_get_magic_code(uint32_t offset)
{
    uint32_t code = 0;
    nvds_read_flash(offset, sizeof(uint32_t), (uint8_t *)&code);
    return code;
}
static void nvds_set_magic_code(uint32_t offset)
{
    uint32_t code = NVDS_SECT_MAGIC_CODE;
    nvds_write_flash(offset, sizeof(uint32_t), (uint8_t *)&code);
}

/* ------------------------------ nvds write/read ------------------------------------------- */

int ln_nvds_write(uint32_t offset, uint8_t* data, uint16_t len)
{
    int ret = NVDS_ERR_FAIL;
    uint8_t rd_data = 0;
    uint32_t i = 0;

    if((offset + len) > SIZE_4KB){
        return ret;
    }

    if(nvds_get_valid_flag() == NVDS_SECT1_VALID)
    {
        nvds_erase_flash(USR_NVDS_SECT2_OFFSET, SIZE_4KB);

        nvds_set_magic_code(USR_NVDS_SECT2_OFFSET + SECT_MAGIC_CODE_OFFSET);

        for (i = SECT_MAGIC_CODE_LEN; i < offset; i++) {
            nvds_read_flash(USR_NVDS_SECT1_OFFSET + i, sizeof(uint8_t), &rd_data);
            if (rd_data != NVDS_NONE_TAG) {
                nvds_write_flash(USR_NVDS_SECT2_OFFSET + i, sizeof(uint8_t), &rd_data);
            }
        }

        nvds_write_flash(USR_NVDS_SECT2_OFFSET + offset, len, data);

        if ((offset + len) < SIZE_4KB) {
            for (i = (offset+len); i < SIZE_4KB; i++) {
                nvds_read_flash(USR_NVDS_SECT1_OFFSET + i, sizeof(uint8_t), &rd_data);
                if (rd_data != NVDS_NONE_TAG) {
                    nvds_write_flash(USR_NVDS_SECT2_OFFSET + i, sizeof(uint8_t), &rd_data);
                }
            }
        }

        nvds_erase_flash(USR_NVDS_FLAG_OFFSET, SIZE_4KB);

        nvds_set_valid_flag(NVDS_SECT2_VALID);

        ret = NVDS_ERR_OK;
    }
    else
    {
        nvds_erase_flash(USR_NVDS_SECT1_OFFSET, SIZE_4KB);

        nvds_set_magic_code(USR_NVDS_SECT1_OFFSET + SECT_MAGIC_CODE_OFFSET);

        for (i = SECT_MAGIC_CODE_LEN; i < offset; i++) {
            nvds_read_flash(USR_NVDS_SECT2_OFFSET + i, sizeof(uint8_t), &rd_data);
            if (rd_data != NVDS_NONE_TAG) {
                nvds_write_flash(USR_NVDS_SECT1_OFFSET + i, sizeof(uint8_t), &rd_data);
            }
        }

        nvds_write_flash(USR_NVDS_SECT1_OFFSET + offset, len, data);

        if ((offset + len) < SIZE_4KB) {
            for (i = (offset + len); i < SIZE_4KB; i++) {
                nvds_read_flash(USR_NVDS_SECT2_OFFSET + i, sizeof(uint8_t), &rd_data);
                if (rd_data != NVDS_NONE_TAG) {
                    nvds_write_flash(USR_NVDS_SECT1_OFFSET + i, sizeof(uint8_t), &rd_data);
                }
            }
        }

        nvds_erase_flash(USR_NVDS_FLAG_OFFSET, SIZE_4KB);

        nvds_set_valid_flag(NVDS_SECT1_VALID);

        ret = NVDS_ERR_OK;
    }

    return ret;
}

int ln_nvds_read(uint32_t offset, uint8_t* data, uint16_t len)
{
    int ret = NVDS_ERR_FAIL;

    if ((offset + len) > SIZE_4KB){
        return ret;
    }

    if (nvds_get_valid_flag() == NVDS_SECT1_VALID)
    {
        nvds_read_flash(USR_NVDS_SECT1_OFFSET+offset, len, data);
        ret = NVDS_ERR_OK;
    }
    else if (nvds_get_valid_flag() == NVDS_SECT2_VALID)
    {
        nvds_read_flash(USR_NVDS_SECT2_OFFSET+offset, len, data);
        ret = NVDS_ERR_OK;
    }
    else
    {
        if (nvds_get_magic_code(USR_NVDS_SECT2_OFFSET+SECT_MAGIC_CODE_OFFSET) == NVDS_SECT_MAGIC_CODE)
        {
            nvds_read_flash(USR_NVDS_SECT2_OFFSET+offset, len, data);//default: sect2
            ret = NVDS_ERR_OK;
        }
        else
        {
            ret = NVDS_ERR_FAIL;
        }
    }

    return ret;
}


/* ---------------------------- nvds public api --------------------------------------- */
int ln_nvds_init(uint32_t base) {
    uint8_t str_len = 0;

    ln_nvds_base = base;

    if (NVDS_ERR_OK != ln_nvds_read(NV0_NVDS_VERSION_OFFST, (uint8_t *)ln_nvds_ver, NV0_NVDS_VERSION_LEN)) {
        nvds_erase_flash(ln_nvds_base, SIZE_4KB*3);
        str_len = strlen(NV0_NVDS_VERSION_STR) + 1;
        str_len = (NV0_NVDS_VERSION_LEN > str_len) ? str_len : NV0_NVDS_VERSION_LEN;
        return ln_nvds_write(NV0_NVDS_VERSION_OFFST, (uint8_t *)NV0_NVDS_VERSION_STR, str_len);
    }

    return NVDS_ERR_OK;
}

char * ln_nvds_version(void) {
    return ln_nvds_ver;
}

/* NV1_OTA_UPG_STATE_OFFST, NV1_OTA_UPG_STATE_LEN */
int ln_nvds_set_ota_upg_state(uint32_t  state) {
    uint32_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_ota_upg_state(&rd_val)) {
        if (rd_val == state) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV1_OTA_UPG_STATE_OFFST, (uint8_t *)&state, NV1_OTA_UPG_STATE_LEN);
}

int ln_nvds_get_ota_upg_state(uint32_t *state) {
    return ln_nvds_read(NV1_OTA_UPG_STATE_OFFST, (uint8_t*)state, NV1_OTA_UPG_STATE_LEN);
}

/* NV2_XTAL_COMP_VAL_OFFST, NV2_XTAL_COMP_VAL_LEN */
int ln_nvds_set_xtal_comp_val(uint8_t  val) {
    uint8_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_xtal_comp_val(&rd_val)) {
        if (rd_val == val) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV2_XTAL_COMP_VAL_OFFST, &val, NV2_XTAL_COMP_VAL_LEN);
}

int ln_nvds_get_xtal_comp_val(uint8_t *val) {
    return ln_nvds_read(NV2_XTAL_COMP_VAL_OFFST, val, NV2_XTAL_COMP_VAL_LEN);
}

/* NV3_TX_POWER_COMP_OFFST, NV3_TX_POWER_COMP_LEN */
int ln_nvds_set_tx_power_comp(uint8_t  val) {
    uint8_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_tx_power_comp(&rd_val)) {
        if (rd_val == val) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV3_TX_POWER_COMP_OFFST, &val, NV3_TX_POWER_COMP_LEN);
}

int ln_nvds_get_tx_power_comp(uint8_t *val) {
    return ln_nvds_read(NV3_TX_POWER_COMP_OFFST, val, NV3_TX_POWER_COMP_LEN);
}

/* NV4_CHIP_SN_OFFSET, NV4_CHIP_SN_LEN */
int ln_nvds_set_chip_sn(uint8_t *sn) {
    uint8_t rd_val[NV4_CHIP_SN_LEN] = { 0 };

    if (NVDS_ERR_OK == ln_nvds_get_chip_sn(rd_val, NV4_CHIP_SN_LEN)) {
        if (memcmp(rd_val, sn, NV4_CHIP_SN_LEN) == 0) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV4_CHIP_SN_OFFSET, sn, NV4_CHIP_SN_LEN);
}

int ln_nvds_get_chip_sn(uint8_t *sn_buf, uint8_t buf_len) {
    uint16_t len = (NV4_CHIP_SN_LEN > buf_len) ? buf_len : NV4_CHIP_SN_LEN;
    return ln_nvds_read(NV4_CHIP_SN_OFFSET, sn_buf, len);
}

/* NV5_BOOT_LOG_UART_BR_OFFSET, NV5_BOOT_LOG_UART_BR_LEN */
int ln_nvds_set_boot_log_uart_baudrate(uint32_t baudrate) {
    uint32_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_boot_log_uart_baudrate(&rd_val)) {
        if (rd_val == baudrate) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV5_BOOT_LOG_UART_BR_OFFSET, (uint8_t *)&baudrate, NV5_BOOT_LOG_UART_BR_LEN);
}

int ln_nvds_get_boot_log_uart_baudrate(uint32_t *baudrate) {
    return ln_nvds_read(NV5_BOOT_LOG_UART_BR_OFFSET, (uint8_t*)baudrate, NV5_BOOT_LOG_UART_BR_LEN);
}

/* NV6_BOOT_LOG_EN_OFFSET, NV6_BOOT_LOG_EN_LEN */
int ln_nvds_set_boot_log_en(uint8_t  val) {
    uint8_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_boot_log_en(&rd_val)) {
        if (rd_val == val) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV6_BOOT_LOG_EN_OFFSET, &val, NV6_BOOT_LOG_EN_LEN);
}

int ln_nvds_get_boot_log_en(uint8_t *val) {
    return ln_nvds_read(NV6_BOOT_LOG_EN_OFFSET, val, NV6_BOOT_LOG_EN_LEN);
}

/* NV7_BOOT_DELAY_MS_OFFSET, NV7_BOOT_DELAY_MS_LEN */
int ln_nvds_set_boot_delay_ms(uint16_t  val) {
    uint16_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_boot_delay_ms(&rd_val)) {
        if (rd_val == val) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV7_BOOT_DELAY_MS_OFFSET, (uint8_t *)&val, NV7_BOOT_DELAY_MS_LEN);
}

int ln_nvds_get_boot_delay_ms(uint16_t *val) {
    return ln_nvds_read(NV7_BOOT_DELAY_MS_OFFSET, (uint8_t *)val, NV7_BOOT_DELAY_MS_LEN);
}

/* NV8_BOOT_DELAY_EN_OFFSET, NV8_BOOT_DELAY_EN_LEN */
int ln_nvds_set_boot_delay_en(uint8_t  val) {
    uint8_t rd_val = 0;
    if (NVDS_ERR_OK == ln_nvds_get_boot_delay_en(&rd_val)) {
        if (rd_val == val) {
            return NVDS_ERR_OK;
        }
    }

    return ln_nvds_write(NV8_BOOT_DELAY_EN_OFFSET, &val, NV8_BOOT_DELAY_EN_LEN);
}

int ln_nvds_get_boot_delay_en(uint8_t *val) {
    return ln_nvds_read(NV8_BOOT_DELAY_EN_OFFSET, val, NV8_BOOT_DELAY_EN_LEN);
}




