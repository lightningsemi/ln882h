cmake_minimum_required(VERSION 3.0.0)
project(firmware_XIP C ASM CXX)
message(STATUS "PROJECT:${PROJECT}")

# 设置执行文件输出目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# 设置库输出位置
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# adjust project folder name if necessary
set(PROJ_CMAKE_DIR ${CMAKE_SOURCE_DIR})
message(STATUS "CMAKE_SOUECE_DIR:" ${PROJ_CMAKE_DIR})
set(CMAKE_VERBOSE_MAKEFILE ON)
message(STATUS "Building project:${PROJECT_NAME}")

#set the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  message(STATUS "Build type: Debug")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  message(STATUS "Build type: Release")
endif()


## add project components
set(ELF_TARGET ${PROJECT_NAME}.elf)

set(PRO_SOURCE "")
set(PRO_INCLUDE "")

# Setup cross-compile toolchain 
include(${PROJ_CMAKE_DIR}/project/${PROJECT}/gcc/gcc-arm-none-eabi.cmake)
include(${PROJ_CMAKE_DIR}/project/${PROJECT}/gcc/target-def.cmake)

# auto-set variables from user input
set(MCU_FLAGS "${MCU}")
set(LINK_FLAGS "${ARCHFLAGS} \
-Wl,--gc-sections") 
set(EXTRA_LINK_FLAGS "-Wl,-Map=${PROJECT_NAME}.map,--cref,--no-warn-mismatch --specs=nano.specs --specs=nosys.specs")
include(${PROJ_CMAKE_DIR}/project/${PROJECT}/gcc/gcc-flags.cmake)
set(FLASH_CFG ${PROJ_CMAKE_DIR}/project/${PROJECT}/cfg/flash_partition_cfg.json)
set(FLASH_TABLE ${PROJ_CMAKE_DIR}/project/${PROJECT}/cfg/flash_partition_table.h)

add_custom_target(
    FLASH_TARGET ALL
)
add_custom_command(TARGET FLASH_TARGET
    PRE_BUILD
    COMMAND python ${PROJ_CMAKE_DIR}/tools/user_cmd/before_build.py ${FLASH_CFG} ${FLASH_TABLE}
    COMMENT "Generating flash_partition_table.h" 
)
# common libraries
add_subdirectory(${PROJ_CMAKE_DIR}/components/serial)
add_subdirectory(${PROJ_CMAKE_DIR}/components/utils)
add_subdirectory(${PROJ_CMAKE_DIR}/mcu)
add_subdirectory(${PROJ_CMAKE_DIR}/components/net/lwip-2.0.3/src)
add_subdirectory(${PROJ_CMAKE_DIR}/components/net/iperf)
add_subdirectory(${PROJ_CMAKE_DIR}/components/net/ping)
add_subdirectory(${PROJ_CMAKE_DIR}/components/net/dhcpd)
add_subdirectory(${PROJ_CMAKE_DIR}/components/kernel)
add_subdirectory(${PROJ_CMAKE_DIR}/components/fs/kv)
add_subdirectory(${PROJ_CMAKE_DIR}/components/fs/nvds)
add_subdirectory(${PROJ_CMAKE_DIR}/components/fs/partition_mgr)
add_subdirectory(${PROJ_CMAKE_DIR}/components/atcmd)
add_subdirectory(${PROJ_CMAKE_DIR}/components/wifi)
add_subdirectory(${PROJ_CMAKE_DIR}/components/fota)
add_subdirectory(${PROJ_CMAKE_DIR}/project/${PROJECT})
