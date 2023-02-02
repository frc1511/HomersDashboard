cmake_minimum_required(VERSION 3.1)
project(DualSense-Windows)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(FATAL_ERROR "DualSense-Windows can only be compiled for Windows")
endif()

set(DS_ARCH "")

if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
  set(DS_ARCH "x64")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "x86|i686")
  set(AN_ARCH "x86")
else()
  message(FATAL_ERROR "Invalid architecture")
endif()

set(DS_LIB_NAME "${CMAKE_CURRENT_SOURCE_DIR}/ds5w_${DS_ARCH}.lib")

#find_library(DS_LIB_NAME ds5w PATHS ${CMAKE_CURRENT_SOURCE_DIR} PATH_SUFFIXES "_x64" "_x86")

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} INTERFACE ${DS_LIB_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
