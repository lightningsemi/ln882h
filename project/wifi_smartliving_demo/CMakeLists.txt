include(ProjModuleCollect.cmake)

file(GLOB_RECURSE SMARTLIVING_DEMO_SRC  app/linkkit/living_platform/*.c)
list(APPEND SMARTLIVING_DEMO_SRC        app/linkkit/cJSON.c)

set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    bsp/serial_hw.c
    bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${SMARTLIVING_DEMO_SRC}
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
    smart_iot_sdk
    mbedtls
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
    bsp
    cfg
    app/linkkit
    app/linkkit/living_platform
)

include(gcc/gcc-custom-build-stage.cmake)
