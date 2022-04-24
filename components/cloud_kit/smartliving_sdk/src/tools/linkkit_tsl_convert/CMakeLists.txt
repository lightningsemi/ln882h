include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/)
include_directories(${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/include)
include_directories(${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/src)

if(NOT WIN32)
    add_executable(linkkit_tsl_convert
        ${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/linkkit_tsl_convert.c
        ${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/src/tsl_opt_impl.c
        ${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/src/tsl_file.c
        ${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/src/cJSON.c
        ${COMP_SMARTLIVING_DIR}/src/tools/linkkit_tsl_convert/src/tsl_format_export.c
    )

    if(NOT MSVC)
        target_link_libraries(linkkit_tsl_convert
            pthread
            rt
        )
    ENDIF (NOT MSVC)
ENDIF (NOT WIN32)

