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
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/alcs)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/alcs/)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/coap)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/coap/cloud)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/coap/local)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/http)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/http2)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/http2/nghttp2)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/MQTTPacket)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/client)

file(GLOB src_protocol_alcs_SRCS
    ${COMP_SMARTLIVING_DIR}/src/protocol/alcs/*.c
)

add_library (iot_alcs OBJECT
    ${src_protocol_alcs_SRCS}
    ${EXTRA_C_SOURCES}
)

