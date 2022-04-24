if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "in-tree build not allowed")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

add_compile_definitions(AWSS_DISABLE_ENROLLEE)
add_compile_definitions(AWSS_DISABLE_REGISTRAR)
# 这个选项来自 default_settings.mk
add_compile_definitions(AWSS_REGISTRAR_LOWPOWER_EN)
# add_compile_definitions(AWSS_SUPPORT_ADHA)
# add_compile_definitions(AWSS_SUPPORT_AHA)
add_compile_definitions(AWSS_SUPPORT_APLIST)
add_compile_definitions(AWSS_SUPPORT_DEV_AP)
# add_compile_definitions(AWSS_SUPPORT_PHONEASAP)
# add_compile_definitions(AWSS_SUPPORT_ROUTER)
add_compile_definitions(AWSS_SUPPORT_SMARTCONFIG)
add_compile_definitions(AWSS_SUPPORT_SMARTCONFIG_MCAST)
# add_compile_definitions(AWSS_SUPPORT_SMARTCONFIG_WPS)
add_compile_definitions(AWSS_SUPPORT_ZEROCONFIG)
add_compile_definitions(COAP_SERV_MULTITHREAD)
add_compile_definitions(CONFIG_HTTP_AUTH_TIMEOUT=10000)
add_compile_definitions(CONFIG_MID_HTTP_TIMEOUT=10000)
add_compile_definitions(CONFIG_GUIDER_AUTH_TIMEOUT=10000)
add_compile_definitions(DEVICE_MODEL_ENABLED)
add_compile_definitions(DEV_BIND_ENABLED)
add_compile_definitions(FORCE_SSL_VERIFY)
add_compile_definitions(MQTT_COMM_ENABLED)
add_compile_definitions(MQTT_DIRECT)
add_compile_definitions(OTA_ENABLED)
add_compile_definitions(OTA_SIGNAL_CHANNEL=1)
add_compile_definitions(SUPPORT_TLS)
add_compile_definitions(DEV_STATEMACHINE_ENABLE)
add_compile_definitions(WIFI_PROVISION_ENABLED)
add_compile_definitions(WITH_MQTT_ZIP_TOPIC=1)
add_compile_definitions(WITH_MQTT_SUB_SHORTCUT=1)
add_compile_definitions(WITH_MQTT_DYN_BUF=1)

add_subdirectory (${COMP_SMARTLIVING_DIR}/components/timer_service)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/infra/utils)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/infra/log)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/infra/system)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/sdk-impl)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/protocol/mqtt)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/awss)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/dev_diagnosis)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/dev_bind)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/linkkit/cm)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/linkkit/dm)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/linkkit/dev_reset)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/services/ota)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/protocol/coap/local)
add_subdirectory (${COMP_SMARTLIVING_DIR}/src/ref-impl/hal)

# NOTE: it's a mbedtls of smartliving version, be careful it conflicts with LN mbedtls.
# add_subdirectory (${COMP_SMARTLIVING_DIR}/src/ref-impl/tls)

set(smartliving_sdk_static_target  smart_iot_sdk)
add_library (${smartliving_sdk_static_target} STATIC
    $<TARGET_OBJECTS:iot_timer_service>
    $<TARGET_OBJECTS:iot_utils>
    $<TARGET_OBJECTS:iot_log>
    $<TARGET_OBJECTS:iot_system>
    $<TARGET_OBJECTS:sdk_impl>
    $<TARGET_OBJECTS:iot_mqtt>
    $<TARGET_OBJECTS:iot_ota>
    $<TARGET_OBJECTS:iot_cm>
    $<TARGET_OBJECTS:iot_dm>
    $<TARGET_OBJECTS:iot_reset>
    $<TARGET_OBJECTS:iot_coap_local>
    $<TARGET_OBJECTS:iot_awss>
    $<TARGET_OBJECTS:iot_dev_diagnosis>
    $<TARGET_OBJECTS:iot_bind>
)

target_link_directories(${smartliving_sdk_static_target}
    PUBLIC
    ${COMP_SMARTLIVING_DIR}/lib/Cortex-M4/
)

target_link_libraries(${smartliving_sdk_static_target}  awss_security)
target_link_libraries(${smartliving_sdk_static_target}  iot_hal)

target_include_directories(${smartliving_sdk_static_target}
    PUBLIC
    include
    include/exports
    include/imports
    src/infra/log
)
