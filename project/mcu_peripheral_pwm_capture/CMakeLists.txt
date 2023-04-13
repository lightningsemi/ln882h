include(ProjModuleCollect.cmake)

set(PROJ_ALL_SRC
    app/main.c
	app/app_pwm.c
	app/pulse_cap.c
	app/pwm_port.c

    bsp/serial_hw.c

    bsp/ln_show_reg.c
    bsp/ln_test_common.c

    
    startup/startup_${CHIP_SERIAL}_gcc.c
    ${MODULE_SRC}
)


#----------------------------   project output elf   ---------------------------
set(TARGET_ELF_NAME                         ${USER_PROJECT})
set(pro_executable_target                   ${TARGET_ELF_NAME}.elf)
add_executable(${pro_executable_target}     ${PROJ_ALL_SRC})

target_link_libraries(${pro_executable_target}
    PUBLIC
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
)

include(gcc/gcc-custom-build-stage.cmake)
