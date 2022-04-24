include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/components/timer_service/)

file(GLOB src_services_timer_service_SRCS
    ${COMP_SMARTLIVING_DIR}/components/timer_service/*.c
)

add_library (iot_timer_service OBJECT
    ${src_services_timer_service_SRCS}
)

