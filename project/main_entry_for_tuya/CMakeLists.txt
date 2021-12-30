include(ProjModuleCollect.cmake)



if(BUILD_INCLUDE_BLE)
message(STATUS      "add ble usr app code!")
file(GLOB_RECURSE  BLE_USR_APP_SRC  app/ble_usr_app/*.c)
else()
set(BLE_USR_APP_SRC )
endif()

set(PROJ_ALL_SRC
    app/main.c
    app/usr_app.c
    app/ate/ln_ty_ate.c
    bsp/serial_hw.c
    bsp/drv_adc_measure.c
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${BLE_USR_APP_SRC}
    ${MODULE_SRC}
)

set(COMP_DHCPD_SUPPORT          ON  PARENT_SCOPE)
set(COMP_LWIP_SUPPORT           ON  PARENT_SCOPE)

#----------------------------   project output elf   ---------------------------
set(TARGET_ELF_NAME                         ${APP_BIN_NAME}-${USER_SW_VER})
set(pro_executable_target                   ${TARGET_ELF_NAME}.elf)
add_executable(${pro_executable_target}     ${PROJ_ALL_SRC})



target_link_libraries(${pro_executable_target}
    PUBLIC
    tuya::common
    tuya::os_adapter
    # tuya::adapter_test
    app::tuya_app
    ln::dhcpd
    lwip
    ${CHIP_SERIAL}_wifi
    tuya_iot
    -lc -lm -lnosys

    PRIVATE
    -T${LINKER_SCRIPT}
    ${EXTRA_LINK_FLAGS}
)

if(BUILD_INCLUDE_BLE)
message(STATUS      "link ble lib!")
target_link_libraries(${pro_executable_target}
    PUBLIC
    ${CHIP_SERIAL}_ble_full_stack
)
endif()

target_link_directories(${pro_executable_target}
    PRIVATE
    ${LN_SDK_ROOT}/lib/gcclib
    ${CMAKE_SOURCE_DIR}/sdk/lib
)

target_include_directories(${pro_executable_target}
    PRIVATE
    app
    app/ate
    app/ble_usr_app
    app/ble_usr_app/app_callback
    bsp
    cfg
)

include(gcc/gcc-custom-build-stage.cmake)
