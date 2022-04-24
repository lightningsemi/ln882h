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
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/sal/)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/sal/include)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/sal/include/internal)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/mdal/sal/src)

file(GLOB src_services_mdal_sal_SRCS
    ${COMP_SMARTLIVING_DIR}/src/services/mdal/sal/./src/*.c
)

add_library (iot_sal OBJECT
    ${src_services_mdal_sal_SRCS}
    ${EXTRA_C_SOURCES}
)

