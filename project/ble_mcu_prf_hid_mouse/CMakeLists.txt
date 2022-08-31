include(ProjModuleCollect.cmake)

set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    app/app_callback/ln_app_callback.c
    app/app_callback/ln_gap_callback.c
    app/app_callback/ln_gatt_callback.c
    app/app_callback/ln_bas_callback.c
    app/app_callback/ln_dis_callback.c
    app/app_callback/ln_hid_callback.c
    app/app_callback/usr_ble_app.c
    bsp/serial_hw.c
    bsp/serial_h4tl_driver.c
    # bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${MODULE_SRC}
)

#----------------------------   project output elf   ---------------------------
set(TARGET_ELF_NAME                         ${USER_PROJECT})
set(pro_executable_target                   ${TARGET_ELF_NAME}.elf)
add_executable(${pro_executable_target}     ${PROJ_ALL_SRC})

target_link_libraries(${pro_executable_target}
    PUBLIC
    ${CHIP_SERIAL}_ble_full_stack
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
