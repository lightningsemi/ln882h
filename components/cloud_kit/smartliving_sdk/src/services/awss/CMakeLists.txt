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
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/MQTTPacket)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/client)
include_directories(${COMP_SMARTLIVING_DIR}/src/sdk-impl/)
include_directories(${COMP_SMARTLIVING_DIR}/src/sdk-impl/deprecated_linkkit)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/awss/)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/dev_bind/)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/dev_bind/os)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/dev_bind/os/product)

file(GLOB_RECURSE  src_services_awss_SRCS  ${COMP_SMARTLIVING_DIR}/src/services/awss/  "*.c")

add_library (iot_awss OBJECT
    ${src_services_awss_SRCS}
    ${EXTRA_C_SOURCES}
)

