include(ProjModuleCollect.cmake)


set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    bsp/serial_hw.c
    bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${MODULE_SRC}
)

set(COMP_DHCPD_SUPPORT          ON  PARENT_SCOPE)
set(COMP_LWIP_SUPPORT           ON  PARENT_SCOPE)

#----------------------------   project output elf   ---------------------------
set(TARGET_ELF_NAME                         ${USER_PROJECT})
set(pro_executable_target                   ${TARGET_ELF_NAME}.elf)
add_executable(${pro_executable_target}     ${PROJ_ALL_SRC})

target_link_libraries(${pro_executable_target}
    PUBLIC
    ${CHIP_SERIAL}_ble_full_stack
    ln::dhcpd
    lwip
    joylink
    ${CHIP_SERIAL}_wifi
    -lc -lm -lnosys

    PRIVATE
    -T${LINKER_SCRIPT}
    ${EXTRA_LINK_FLAGS}
)

target_link_directories(${pro_executable_target}
    PRIVATE
    ${LN_SDK_ROOT}/lib/gcclib
    ${COMP_JOYLINKS_DIR}/lib
)

target_include_directories(${pro_executable_target}
    PRIVATE
    app
    bsp
    cfg


)
add_definitions(-D_BLE_CONFIG_SUPPORTED_)
add_definitions(-DJOYLINK_SDK_EXAMPLE_TEST)
include(gcc/gcc-custom-build-stage.cmake)
