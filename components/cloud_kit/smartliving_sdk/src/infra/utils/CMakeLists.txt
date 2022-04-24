include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/log/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/digest)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/misc)

file(GLOB src_infra_utils_SRCS
    ${COMP_SMARTLIVING_DIR}/src/infra/utils/digest/*.c
    ${COMP_SMARTLIVING_DIR}/src/infra/utils/misc/*.c
)

add_library(iot_utils OBJECT
    ${src_infra_utils_SRCS}
    ${EXTRA_C_SOURCES}
)

