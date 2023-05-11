include(ProjModuleCollect.cmake)


file(GLOB_RECURSE  COLINK_USR_APP_SRC  colink/user/*.c)
file(GLOB_RECURSE  JSON_USR_APP_SRC  colink/json/*.c)

set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    app/usr_ble_app.c
    app/usr_ctrl.c
    bsp/serial_hw.c
    bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${MODULE_SRC}
    ${COLINK_USR_APP_SRC}
    ${JSON_USR_APP_SRC}
    ${COMP_LWIP_DIR}/apps/mdns/mdns.c
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
    ${CHIP_SERIAL}_wifi
    colink
    colinkcJSON
    colinkgw
    -lc -lm -lnosys

    PRIVATE
    -T${LINKER_SCRIPT}
    ${EXTRA_LINK_FLAGS}
)

target_link_directories(${pro_executable_target}
    PRIVATE
    ${LN_SDK_ROOT}/lib/gcclib
    ${LN_SDK_ROOT}/project/${USER_PROJECT}/colink/lib/release
)

target_include_directories(${pro_executable_target}
    PRIVATE
    app
    bsp
    cfg
    colink/include
    colink/user_include
    colink
)

include(gcc/gcc-custom-build-stage.cmake)
