##################################  kernel  ####################################
set(KERNEL_SRC
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/event_groups.c
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/list.c
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/queue.c
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/tasks.c
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/timers.c
    ${COMP_KERNEL_DIR}/FreeRTOS/hooks.c
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c
    ${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/MemMang/heap_5.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_common.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_cpuusage.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_debug.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_mutex.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_queue.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_semaphore.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_thread.c
    ${COMP_KERNEL_DIR}/FreeRTOS_Adapter/freertos_timer.c
)
include_directories(${COMP_KERNEL_DIR})
include_directories(${COMP_KERNEL_DIR}/osal)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS_Adapter)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/include)
include_directories(${COMP_KERNEL_DIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F)
list(APPEND MODULE_SRC ${KERNEL_SRC})

#############################   Partition MGR   ################################
file(GLOB_RECURSE  PARTMGR_SRC  ${COMP_PARTMGR_DIR}/*.c)
include_directories(${COMP_PARTMGR_DIR})
list(APPEND MODULE_SRC ${COMP_PARTMGR_DIR})

###################################   kv   #####################################
file(GLOB_RECURSE  KV_SRC  ${COMP_KV_DIR}/*.c)
include_directories(${COMP_KV_DIR}/kv)
include_directories(${COMP_KV_DIR}/kv_port)
list(APPEND MODULE_SRC ${KV_SRC})

###################################  nvds  #####################################
file(GLOB_RECURSE  NVDS_SRC  ${COMP_NVDS_DIR}/*.c)
include_directories(${COMP_NVDS_DIR})
list(APPEND MODULE_SRC ${NVDS_SRC})

###################################  utils  ####################################
set(UTILS_SRC
    ${COMP_UTILS_DIR}/debug/CmBacktrace/cm_backtrace.c
    ${COMP_UTILS_DIR}/debug/ln_assert.c
    ${COMP_UTILS_DIR}/debug/log.c
    ${COMP_UTILS_DIR}/fifo/fifobuf.c
    ${COMP_UTILS_DIR}/reboot_trace/reboot_trace.c
    ${COMP_UTILS_DIR}/runtime/runtime.c

    ${COMP_UTILS_DIR}/ln_misc.c
    ${COMP_UTILS_DIR}/crc16.c
    ${COMP_UTILS_DIR}/crc32.c
    ${COMP_UTILS_DIR}/wrap_stdio.c
)

include_directories(${COMP_UTILS_DIR})
include_directories(${COMP_UTILS_DIR}/runtime)
include_directories(${COMP_UTILS_DIR}/reboot_trace)
include_directories(${COMP_UTILS_DIR}/fifo)
include_directories(${COMP_UTILS_DIR}/debug)
include_directories(${COMP_UTILS_DIR}/debug/CmBacktrace)
list(APPEND MODULE_SRC ${UTILS_SRC})

###################################  serial  ###################################
file(GLOB_RECURSE  SERIAL_SRC  ${COMP_SERIAL_DIR}/*.c)
include_directories(${COMP_SERIAL_DIR})
list(APPEND MODULE_SRC ${SERIAL_SRC})

###################################   MCU   ####################################
set(MCU_SRC
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_adc.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_cache.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_common.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_flash.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_qspi.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_uart.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_wdt.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_trng.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_gpio.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_interrupt.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_clock.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_misc.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_ext.c
    ${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/hal/hal_timer.c
    ${MCU_LN882X_DIR}/${CHIP_SERIAL}/system_${CHIP_SERIAL}.c
)

include_directories(${MCU_LN882X_DIR}/${CHIP_SERIAL})
include_directories(${MCU_LN882X_DIR}/CMSIS_5.3.0)
include_directories(${MCU_LN882X_DIR}/driver_${CHIP_SERIAL})
include_directories(${MCU_LN882X_DIR}/driver_${CHIP_SERIAL}/reg)
list(APPEND MODULE_SRC ${MCU_SRC})

###############################   BLE (export) #################################
set(NIMBLE_SRC
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/services/gap/src/ble_svc_gap.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/services/gatt/src/ble_svc_gatt.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_att.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_att_clt.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_att_cmd.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_att_svr.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_eddystone.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_gap.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_gattc.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_gatts.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_gatts_lcl.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_adv.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_atomic.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_cfg.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_conn.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_flow.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_hci.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_hci_cmd.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_hci_evt.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_hci_util.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_id.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_log.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_mbuf.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_misc.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_mqueue.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_periodic_sync.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_pvcy.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_resolv.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_shutdown.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_startup.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_hs_stop.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_ibeacon.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_l2cap.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_l2cap_coc.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_l2cap_sig.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_l2cap_sig_cmd.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_monitor.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_sm.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_sm_alg.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_sm_cmd.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_sm_lgcy.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_sm_sc.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_store.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_store_util.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src/ble_uuid.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/util/src/addr.c
    ${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/store/ram/src/ble_store_ram.c

    ${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/src/endian.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/src/mem.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/src/nimble_port.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/src/os_mbuf.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/src/os_mempool.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/src/os_msys_init.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/npl/freertos/src/nimble_port_freertos.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/npl/freertos/src/npl_os_freertos.c
    ${COMP_BLE_DIR}/thirdparty/nimble/porting/ln/port/src/ln_nimble_mem.c

    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/aes_decrypt.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/aes_encrypt.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/cbc_mode.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ccm_mode.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/cmac_mode.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ctr_mode.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ctr_prng.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ecc.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ecc_dh.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ecc_dsa.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/ecc_platform_specific.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/hmac.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/hmac_prng.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/sha256.c
    ${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/src/utils.c

    ${COMP_BLE_DIR}/thirdparty/nimble/porting/ln/hci_adapter/ln_hci_adapter.c
)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/porting/ln/hci_adapter)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/services/gap/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/services/gatt/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/src)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/util/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/porting/nimble/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/porting/npl/freertos/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/porting/ln/port/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/ext/tinycrypt/include)
include_directories(${COMP_BLE_DIR}/thirdparty/nimble/nimble/host/store/ram/include)

list(APPEND MODULE_SRC ${NIMBLE_SRC})


file(GLOB_RECURSE  BLE_PORT_SRC   ${COMP_BLE_DIR}/ble_port/*.c)

include_directories(${COMP_BLE_DIR})
include_directories(${COMP_BLE_DIR}/ble_port)
include_directories(${COMP_BLE_DIR}/mac/ble/ll/api)
include_directories(${COMP_BLE_DIR}/mac/ble/ll/import)
include_directories(${COMP_BLE_DIR}/mac/ble/ll/src)
include_directories(${COMP_BLE_DIR}/mac/ble/ll/src/llm)
include_directories(${COMP_BLE_DIR}/mac/em/api)
include_directories(${COMP_BLE_DIR}/mac/hci/api)
include_directories(${COMP_BLE_DIR}/mac/sch/api)
include_directories(${COMP_BLE_DIR}/mac/sch/import)
include_directories(${COMP_BLE_DIR}/modules/aes/api)
include_directories(${COMP_BLE_DIR}/modules/aes/api)
include_directories(${COMP_BLE_DIR}/modules/common/api)
include_directories(${COMP_BLE_DIR}/modules/dbg/api)
include_directories(${COMP_BLE_DIR}/modules/ecc_p256/api)
include_directories(${COMP_BLE_DIR}/modules/h4tl/api)
include_directories(${COMP_BLE_DIR}/modules/ke/api)
include_directories(${COMP_BLE_DIR}/modules/lib_ver/api)
include_directories(${COMP_BLE_DIR}/modules/nvds/api)
include_directories(${COMP_BLE_DIR}/modules/rf/api)
include_directories(${COMP_BLE_DIR}/modules/rwip/api)


list(APPEND MODULE_SRC ${BLE_PORT_SRC})

###################################   MISC  ####################################
set(MISC_SRC
    ${COMP_LIBC_STUB_DIR}/newlib_stub.c
)
include_directories(${LN_SDK_ROOT}/components)
list(APPEND MODULE_SRC ${MISC_SRC})