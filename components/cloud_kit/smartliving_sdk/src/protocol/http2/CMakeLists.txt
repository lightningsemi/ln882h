include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/log/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/digest)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/misc)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/http2/)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/http2/nghttp2)

file(GLOB src_protocol_http2_SRCS
    ${COMP_SMARTLIVING_DIR}/src/protocol/http2/*.c
)

add_library (iot_http2 OBJECT
    ${src_protocol_http2_SRCS}
    ${EXTRA_C_SOURCES}
)

