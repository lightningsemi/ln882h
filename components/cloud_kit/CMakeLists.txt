###################################  header path  ##############################
## LN components
include_directories(${LN_SDK_ROOT}/project/${USER_PROJECT}/cfg)
include_directories(${MCU_LN882X_DIR}/CMSIS_5.3.0)
include_directories(${MCU_LN882X_DIR}/${CHIP_SERIAL})
include_directories(${MCU_LN882X_DIR}/driver_${CHIP_SERIAL})
include_directories(${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/reg)
include_directories(${COMP_TOP_DIR})
include_directories(${COMP_LWIP_DIR}/include)
include_directories(${COMP_LWIP_DIR}/port/ln_osal)
include_directories(${COMP_LWIP_DIR}/port/ln_osal/include)
include_directories(${COMP_KERNEL_DIR})
include_directories(${COMP_KERNEL_DIR}/osal)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS_Adapter)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/include)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F)
include_directories(${COMP_DHCPD_DIR})
include_directories(${COMP_WIFI_DIR})
include_directories(${COMP_KV_DIR}/kv)
include_directories(${COMP_KV_DIR}/kv_port)


