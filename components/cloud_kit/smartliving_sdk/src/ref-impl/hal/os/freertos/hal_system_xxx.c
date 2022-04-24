#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iot_import.h"
#include "netif/ethernetif.h"
#include "hal/hal_misc.h"
#include "utils/crc32.h"
#include "utils/wrap_stdio.h"
#include "utils/debug/log.h"

/**
 * @brief Closes the file associated with the stream and disassociates it.
 *
 * @param [in] stream: @n Pointer to void that identifies the stream.
 *
 * @return If the stream is successfully closed, a zero value is returned.On failure, non-zero is returned.
 * @see None.
 * @note None.
 */
extern int HAL_Fclose(void *stream)
{
    return (int)0;
}

/**
 * @brief Opens the file whose name is specified in the parameter filename and associates it
 *  with a stream that can be identified in future operations by the void pointer returned.
 *
 * @param [in] path: @n The file path to open.With reference to fopen
 * @param [in] mode: @n C string containing a file access mode.
 * @return If the file is successfully opened, the function returns a pointer to void object that can be used to
 * identify the stream on future operations.Otherwise, a null pointer is returned.
 * @see None.
 * @note None.
 */
extern void *HAL_Fopen(const char *path, const char *mode)
{
    return NULL;
}

/**
 * @brief Reads an array of count elements, each one with a size of size bytes, from the stream and
 * stores them in the block of memory specified by ptr.
 *
 * @param [in] buff: @n Pointer to a block of memory with a size of at least (size*count) bytes, converted to a void*.
 * @param [in] size: @n size in bytes, of each element to be read.
 * @param [in] count: @n Number of elements, each one with a size of size bytes.
 * @param [in] stream: @n Pointer to void that specifies an input stream.
 * @return The total number of elements successfully read is returned.If either size or count is zero, the function returns zero
 * @see None.
 * @note None.
 */
extern uint32_t HAL_Fread(void *buff, uint32_t size, uint32_t count, void *stream)
{
    return (uint32_t)0;
}

/**
 * @brief Sets the position indicator associated with the stream to a new position.
 *
 * @param [in] stream: @n Pointer to void that identifies the stream.
 * @param [in] offset: @n Binary files: Number of bytes to offset from origin.
 * @param [in] origin: @n Position used as reference for the offset. It is specified by one of value enum in hal_fs_seek_type_t.
 *
 * @return If successful, the function returns zero.Otherwise, it returns non-zero value.
 * @see None.
 * @note None.
 */
extern int HAL_Fseek(void *stream, long offset, int framewhere)
{
    return (int)0;
}

/**
 * @brief Returns the current value of the position indicator of the stream.
 *
 * @param [in] stream: @n Pointer to void that identifies the stream.
 *
 * @return On success, the current value of the position indicator is returned.On failure, -1L is returned.
 * @see None.
 * @note None.
 */
extern long HAL_Ftell(void *stream)
{
    return (long) 0;
}

/**
 * @brief Writes an array of count elements, each one with a size of size bytes, from the block of memory pointed
 * by ptr to the current position in the stream.
 *
 * @param [in] ptr: @n Pointer to the array of elements to be written, converted to a const void*.
 * @param [in] size: @n Size in bytes of each element to be written.
 * @param [in] count: @n Number of elements, each one with a size of size bytes.
 * @param [in] stream: @n Pointer to void that specifies an output stream.
 * @return The total number of elements successfully written is returned.If either size or count is zero, the function returns zero.
 * @see None.
 * @note None.
 */
extern uint32_t HAL_Fwrite(const void *ptr, uint32_t size, uint32_t count, void *stream)
{
    return (uint32_t)0;
}

/**
 * @brief Changes the size of the memory block pointed to by ptr to size bytes.
 *
 * @param [in] ptr  @n pointer to be realloc
 * @param [in] size @n specify block size in bytes for newly allocated memory
 * @return A pointer to the beginning of newly allocated memory.
 * @see None.
 * @note Block value is indeterminate.
 */
extern void *HAL_Realloc(void *ptr, uint32_t size)
{
    void *new_ptr = NULL;

    new_ptr = OS_Malloc(size);
    if (NULL != new_ptr) {
        OS_Free(ptr);
        memcpy(new_ptr, ptr, size);
        ptr = new_ptr;
        return ptr;
    } else {
        return NULL;
    }
}

/**
 * @brief Writes formatted data to stream.
 *
 * @param [in] fmt: @n String that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_Printf(const char *fmt, ...)
{
    // __wrap_sprintf((stdio_write_fn)log_stdio_write, fmt, ##__VA_ARGS__);
    // __wrap_sprintf((stdio_write_fn)log_stdio_write, fmt, __VA_ARGS__);

    va_list args;
    va_start(args, fmt);
    __sprintf("", (stdio_write_fn)log_stdio_write, fmt, args);
//    __wrap_sprintf((stdio_write_fn)log_stdio_write, fmt, args);
    va_end(args);
}
/**
 * @brief Writes formatted data to string.
 *
 * @param [out] str: @n String that holds written text.
 * @param [in] len: @n Maximum length of character will be written
 * @param [in] fmt: @n Format that contains the text to be written, it can optionally contain embedded format specifiers
     that specifies how subsequent arguments are converted for output.
 * @param [in] ...: @n the variable argument list, for formatted and inserted in the resulting string replacing their respective specifiers.
 * @return bytes of character successfully written into string.
 * @see None.
 * @note None.
 */
int HAL_Snprintf(char *str, const int len, const char *fmt, ...)
{
    va_list args;
    int rc;

    va_start(args, fmt);
    rc = vsnprintf(str, len, fmt, args);
    va_end(args);

    return rc;
}

int HAL_Vsnprintf(char *str, const int len, const char *format, va_list ap)
{
    return vsnprintf(str, len, format, ap);
}

uint32_t HAL_Random(uint32_t region)
{
    return (region != 0) ? (fakeRandom() % region) : 0;
}

void HAL_Srandom(uint32_t seed)
{
    // srand(seed);
}

void HAL_Reboot()
{
    hal_misc_reset_all();
}

/**
 * @brief check system network is ready(get ip address) or not.
 *
 * @param None.
 * @return 0, net is not ready; 1, net is ready.
 * @see None.
 * @note None.
 */
int HAL_Sys_Net_Is_Ready()
{
    if (NETDEV_LINK_UP == netdev_get_link_state(netdev_get_active())) {
        return 1;
    }

    return 0;
}

/**
 ** @brief Retrieves all the info of the current network interfaces,
 ** including eth Mac, WiFi Mac, and IMEI/ICCID/IMSI/MSISDN for cellular connections.
 ** Note that the network interface length MUST NOT exceed NIF_STRLEN_MAX
 **
 ** @param [nif_str] give buffer to save network interface
 ** @return the lenth of the nif_str info
 ** @see None.
 ** @note None.
 **/
int HAL_GetNetifInfo(char *nif_str)
{
    memset(nif_str, 0x0, NIF_STRLEN_MAX);

    // TODO: 如果需要添加 MAC/IMEI/ICCID/IMSI/MSISDN 等信息，需要进一步完善。
    const char *net_info = "WiFi|LN";
    strncpy(nif_str, net_info, strlen(net_info));

#ifdef __DEMO__
    /* if the device have only WIFI, then list as follow, note that the len MUST NOT exceed NIF_STRLEN_MAX */
    const char *net_info = "WiFi|03ACDEFF0032";
    strncpy(nif_str, net_info, strlen(net_info));
    /* if the device have ETH, WIFI, GSM connections, then list all of them as follow, note that the len MUST NOT exceed NIF_STRLEN_MAX */
    // const char *multi_net_info = "ETH|0123456789abcde|WiFi|03ACDEFF0032|Cellular|imei_0123456789abcde|iccid_0123456789abcdef01234|imsi_0123456789abcde|msisdn_86123456789ab");
    // strncpy(nif_str, multi_net_info, strlen(multi_net_info));
#endif
    return strlen(nif_str);
}

static int64_t sg_delta_ms = 0;

/**
 * @brief Set the UTC time in milliseconds.
 *
 * @param[in] ms: @the time value to be set in milliseconds.
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_UTC_Set(long long ms)
{
    sg_delta_ms = ms - HAL_UptimeMs();
}

/**
 * @brief Get the UTC time in milliseconds.
 *
 * @param None.
 * @return  the UTC time in milliseconds.
 * @see None.
 * @note None.
 */
DLL_HAL_API long long HAL_UTC_Get(void)
{
    long long ret = HAL_UptimeMs() + sg_delta_ms;
    return ret;
}

/**
 * @brief
 * @param crc_data
 * @param crc_len
 * @return int
 */
int HAL_CRC_32(const unsigned char* crc_data, unsigned int crc_len)
{
    return (int)ln_crc32_signle_cal( (uint8_t *)crc_data, (int)crc_len );
}
