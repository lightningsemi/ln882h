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
include_directories(${COMP_SMARTLIVING_DIR}/src/services/shadow/)

file(GLOB src_services_shadow_SRCS
    ${COMP_SMARTLIVING_DIR}/src/services/shadow/*.c
)

add_library (iot_shadow OBJECT
    ${src_services_shadow_SRCS}
    ${EXTRA_C_SOURCES}
)

