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
include_directories(${COMP_SMARTLIVING_DIR}/src/services/ota/)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/ota/impl)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/ota/prot)

file(GLOB src_services_ota_SRCS
    ${COMP_SMARTLIVING_DIR}/src/services/ota/*.c
    ${COMP_SMARTLIVING_DIR}/src/services/ota/impl/*.c
)

add_library(iot_ota OBJECT
    ${src_services_ota_SRCS}
    ${EXTRA_C_SOURCES}
)

