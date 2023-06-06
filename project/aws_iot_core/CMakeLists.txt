include(ProjModuleCollect.cmake)

set(COMP_DHCPD_SUPPORT          ON  PARENT_SCOPE)
set(COMP_LWIP_SUPPORT           ON  PARENT_SCOPE)

if(${AWS_DEMO} STREQUAL "demo_http_mutual_auth")
    message(STATUS ${AWS_DEMO})
    set(AWS_DEMO_SRC
        demo_http_mutual_auth/http_mutual_auth/http_demo_mutual_auth.c
        demo_http_mutual_auth/http_mutual_auth/http_demo_url_utils.c
        demo_http_mutual_auth/http_mutual_auth/http_demo_utils.c
    )

    set(AWS_DEMO_INC
        demo_http_mutual_auth/http_mutual_auth
    )
elseif(${AWS_DEMO} STREQUAL "demo_mqtt_mutual_auth")
    message(STATUS ${AWS_DEMO})
    set(AWS_DEMO_SRC
        demo_mqtt_mutual_auth/mqtt_mutual_auth/mqtt_demo_mutual_auth.c
    )

    set(AWS_DEMO_INC
        demo_mqtt_mutual_auth/mqtt_mutual_auth
    )
else()
    message(FATAL_ERROR "Please set AWS_DEMO as demo_http_mutual_auth or demo_mqtt_mutual_auth")
endif()

set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    bsp/serial_hw.c
    bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    certificates/ca_cert_def.c
    ${MODULE_SRC}
    ${AWS_DEMO_SRC}
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
    certificates
    ${AWS_DEMO_INC}
    ${PROJECT_DIR}/cfg
)

include(gcc/gcc-custom-build-stage.cmake)
