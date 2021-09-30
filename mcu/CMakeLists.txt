set(DRV_LN882X_DIR ${PROJ_CMAKE_DIR}/mcu)

set(DRV_LN882X_SRC
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/cache.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/exflash.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/flash.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_adc.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_aes.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_common.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_dma.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_efuse.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_gpio.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_i2c.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_i2s.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_interrupt.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_pwm.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_rtc.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_sleep.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_spim.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_spis.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_syscon.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_timer.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_trng.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_uart.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/hal_wdt.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/qspi.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/hal/simi2c.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_adc.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_aes.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_boot_header.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_cache.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_dma.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_efuse.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_gpio.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_i2c.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_i2s.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_pwm.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_qspi.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_rtc.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_sdio.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_sleep.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_spim.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_spim2.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_spis.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_syscon.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_timer.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_trng.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_uart.c
    ${DRV_LN882X_DIR}/driver_ln882x/src/ll/ll_wdt.c
    ${DRV_LN882X_DIR}/ln88xx/system_ln88xx.c
)

set(driver_static_target "driver_ln882x")
add_library(${driver_static_target} STATIC ${DRV_LN882X_SRC})
add_library(mcu::driver_ln882x ALIAS ${driver_static_target})

target_link_libraries(${driver_static_target} components::utils)
target_include_directories(${driver_static_target} PUBLIC
    ${DRV_LN882X_DIR}/ln88xx
    ${DRV_LN882X_DIR}/driver_ln882x/inc/
    ${DRV_LN882X_DIR}/driver_ln882x/inc/hal
    ${DRV_LN882X_DIR}/driver_ln882x/inc/ll
    ${DRV_LN882X_DIR}/driver_ln882x/inc/reg
    ${DRV_LN882X_DIR}/CMSIS_5.3.0
    ${PROJ_CMAKE_DIR}/project/${PROJECT}/cfg

    # ${PROJ_CMAKE_DIR}/include/cpu/ARM_CM4F/CMSIS_5.3.0
    # ${PROJ_CMAKE_DIR}/include/cpu/ARM_CM4F/ln88xx
    # ${PROJ_CMAKE_DIR}/include/driver_ln882x
    # ${PROJ_CMAKE_DIR}/include/kernel
    # ${PROJ_CMAKE_DIR}/include/kernel/FreeRTOS
    # ${PROJ_CMAKE_DIR}/include/kernel/FreeRTOS/portable/GCC/ARM_CM4F
    # ${PROJ_CMAKE_DIR}/include
    # ${PROJ_CMAKE_DIR}/project/${PROJECT}/app
    # ${DRV_LN882X_DIR}/ll/include
    # ${DRV_LN882X_DIR}/ll
    
)