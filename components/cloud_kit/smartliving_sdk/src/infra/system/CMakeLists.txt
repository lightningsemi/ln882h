include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/log)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/system/facility)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/digest)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/misc)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt)
include_directories(${COMP_SMARTLIVING_DIR}/src/sdk-impl)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/crypto)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/id2)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/itls)
include_directories(${COMP_SMARTLIVING_DIR}/src/security/pro/km)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/dev_bind)

file(GLOB src_infra_system_SRCS
    ${COMP_SMARTLIVING_DIR}/src/infra/system/*.c
    ${COMP_SMARTLIVING_DIR}/src/infra/system/facility/*.c
)

add_library (iot_system OBJECT
    ${src_infra_system_SRCS}
    ${EXTRA_C_SOURCES}
)

