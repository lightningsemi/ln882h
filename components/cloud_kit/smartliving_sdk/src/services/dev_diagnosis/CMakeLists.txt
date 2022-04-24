include_directories(${COMP_SMARTLIVING_DIR})
include_directories(${COMP_SMARTLIVING_DIR}/include)
include_directories(${COMP_SMARTLIVING_DIR}/include/exports)
include_directories(${COMP_SMARTLIVING_DIR}/include/imports)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/log)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/digest)
include_directories(${COMP_SMARTLIVING_DIR}/src/infra/utils/misc)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/dev_bind)
include_directories(${COMP_SMARTLIVING_DIR}/src/services/dev_diagnosis)

file(GLOB src_services_dev_diagnosis_SRCS
    ${COMP_SMARTLIVING_DIR}/src/services/dev_diagnosis/*.c
)

add_library (iot_dev_diagnosis OBJECT
    ${src_services_dev_diagnosis_SRCS}
)

