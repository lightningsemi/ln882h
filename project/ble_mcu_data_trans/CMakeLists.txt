include(ProjModuleCollect.cmake)

set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    app/arch_main.c
    app/usr_send_data.c
    app/app_callback/ln_app_callback.c
    app/app_callback/ln_gap_callback.c
    app/app_callback/ln_gatt_callback.c
    bsp/serial_hw.c
    bsp/serial_h4tl_driver.c
    # bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${MODULE_SRC}
)

set(COMP_DHCPD_SUPPORT                      ON  PARENT_SCOPE)
set(COMP_LWIP_SUPPORT                       ON  PARENT_SCOPE)

#----------------------------   project output elf   ---------------------------
set(TARGET_ELF_NAME                         ${USER_PROJECT})
set(pro_executable_target                   ${TARGET_ELF_NAME}.elf)
add_executable(${pro_executable_target}     ${PROJ_ALL_SRC})

target_link_libraries(${pro_executable_target}
    PUBLIC
    ${CHIP_SERIAL}_ble_full_stack
    ln::dhcpd
    lwip
    ${CHIP_SERIAL}_wifi
    -lc -lm -lnosys

    PRIVATE
    -T${LINKER_SCRIPT}
    ${EXTRA_LINK_FLAGS}
)

target_link_directories(${pro_executable_target}
    PRIVATE
    ${LN_SDK_ROOT}/lib/gcclib
)

target_include_directories(${pro_executable_target}
    PRIVATE
    app
    app/app_callback
    bsp
    cfg
)

include(gcc/gcc-custom-build-stage.cmake)
