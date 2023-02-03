cmake_minimum_required(VERSION 3.1)
project(DualSense-Windows)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(FATAL_ERROR "DualSense-Windows can only be compiled for Windows")
endif()

set(DSW_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/DualSense-Windows/VS19_Solution/DualSenseWindows/src/DualSenseWindows")
set(DSW_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/DualSense-Windows/VS19_Solution/DualSenseWindows/include")

file(GLOB DSW_SRC_FILES "${DSW_SRC_DIR}/*.cpp")

add_library(${PROJECT_NAME} STATIC ${DSW_SRC_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC "${DSW_INC_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${DSW_SRC_DIR}")
target_include_directories(${PROJECT_NAME} PRIVATE "${DSW_SRC_DIR}/..")

target_compile_definitions(${PROJECT_NAME} PUBLIC "DS5W_BUILD_LIB")
