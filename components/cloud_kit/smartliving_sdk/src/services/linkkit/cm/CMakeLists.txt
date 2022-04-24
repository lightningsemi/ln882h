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
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/alcs/)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/MQTTPacket)
include_directories(${COMP_SMARTLIVING_DIR}/src/protocol/mqtt/client)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/linkkit/cm/)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/linkkit/cm/include)

file(GLOB src_services_linkkit_cm_SRCS
    ${COMP_SMARTLIVING_DIR}/src/services/linkkit/cm/*.c
)

add_library(iot_cm OBJECT
    ${src_services_linkkit_cm_SRCS}
    ${EXTRA_C_SOURCES}
)

