set(OS_DIR  "freertos")

set(EXTRA_C_SOURCES
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_awss_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_crypto_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_kv_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_os_freertos.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_ota_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_product_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_system_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_tcp_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_tls_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_udp_xxx.c
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os/${OS_DIR}/hal_wifi_xxx.c
)

include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/log)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system/facility)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/digest)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/misc)
include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/hal)
include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/os)


# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/ssl)
# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/ssl/itls)
# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/ssl/mbedtls)
# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/ssl/openssl)

# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/)
# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/include)
# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/include/mbedtls)
# include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/library)

include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/crypto)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/id2)
# include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/itls)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/km)
include_directories(${COMP_SMARTLIVING_DIR}/src/services)

include_directories(${LN_SDK_ROOT}/project/${USER_PROJECT}/bsp)
include_directories(${LN_SDK_ROOT}/project/${USER_PROJECT}/cfg)

include_directories(${MCU_LN882X_DIR}/${CHIP_SERIAL})
include_directories(${MCU_LN882X_DIR}/CMSIS_5.3.0)
include_directories(${MCU_LN882X_DIR}/driver_${CHIP_SERIAL})
include_directories(${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/reg)

include_directories(${LN_SDK_ROOT}/components)
include_directories(${COMP_KERNEL_DIR})
include_directories(${COMP_KERNEL_DIR}/osal)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS_Adapter)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/include)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F)
include_directories(${COMP_WIFI_DIR})
include_directories(${COMP_WIFI_DIR}/wifi_lib_export)
include_directories(${COMP_WIFI_DIR}/wifi_lib_import)
include_directories(${COMP_LWIP_DIR}/port)
include_directories(${COMP_LWIP_DIR}/port/ln_osal/include)
include_directories(${COMP_LWIP_DIR}/include)
include_directories(${COMP_DHCPD_DIR})
include_directories(${COMP_KV_DIR}/kv)
include_directories(${COMP_KV_DIR}/kv_port)
include_directories(${COMP_MBEDTLS_DIR}/include)
include_directories(${COMP_MBEDTLS_DIR}/port_ln/include)


# NOTE: 这里的 ssl/mbedtls/*.c 是对 mbedtls 的封装，和 hal_tls_xxx.c 冲突了
# file (GLOB src_ref-impl_hal_SRCS
#     ${COMP_SMARTLIVING_DIR}/src/ref-impl/hal/ssl/mbedtls/*.c
# )

add_library (iot_hal  OBJECT
    ${EXTRA_C_SOURCES}
    ${src_ref-impl_hal_SRCS}
)

target_link_libraries(iot_hal mbedtls)
