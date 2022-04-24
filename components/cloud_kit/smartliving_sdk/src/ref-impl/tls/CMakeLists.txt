add_definitions (-DDLL_TLS_EXPORTS)

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
include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/)
include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/include)
include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/include/mbedtls)
include_directories(${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/library)

file (GLOB src_ref-impl_tls_SRCS
    ${COMP_SMARTLIVING_DIR}/src/ref-impl/tls/library/*.c
)

add_library (mbedtls STATIC
    ${src_ref-impl_tls_SRCS}
    ${EXTRA_C_SOURCES}
)
