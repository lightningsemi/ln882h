include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/log)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system/facility)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/digest)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/misc)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/mal/)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/mal/ica)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/mal/ica/test)

file(GLOB src_services_mdal_mal_SRCS
    ${COMP_SMARTLIVING_DIR}/src/services/mdal/mal/*.c
)

add_library (iot_mal OBJECT
    ${src_services_mdal_mal_SRCS}
    ${EXTRA_C_SOURCES}
)

