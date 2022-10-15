cmake_minimum_required(VERSION 3.1)

set(HD_VERSION_MAJOR "0")
set(HD_VERSION_MINOR "1")
set(HD_VERSION_PATCH "0")
set(HD_VERSION "${HD_VERSION_MAJOR}.${HD_VERSION_MINOR}.${HD_VERSION_PATCH}")
set(HD_VERSION_SHORT "${HD_VERSION_MAJOR}.${HD_VERSION_MINOR}")

list(APPEND HD_DEF_LIST "HD_VERSION=${HD_VERSION_SHORT}")
list(APPEND HD_DEF_LIST "HD_VERSION_MAJOR=${HD_VERSION_MAJOR}")
list(APPEND HD_DEF_LIST "HD_VERSION_MINOR=${HD_VERSION_MINOR}")
list(APPEND HD_DEF_LIST "HD_VERSION_PATCH=${HD_VERSION_PATCH}")
list(APPEND HD_DEF_LIST "HD_VERSION_STR=\"${HD_VERSION}\"")

project(HomersDashboard LANGUAGES C CXX VERSION ${HD_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  #add_compile_options(/W4)
  message(STATUS "MSVC Compiler Detected")
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.\nUse a seperate directory for build files.")
endif()

set(HD_LIB_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(HD_INC_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(TD_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ThunderDashboardLibrary")

set(HD_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")

include("${TD_LIB_DIR}/cmake/util.cmake")

if (TD_MACOS)
  enable_language(OBJC)
  set(CMAKE_OBJC_STANDARD 11)
  set(CMAKE_OBJC_STANDARD_REQUIRED YES)
  set(CMAKE_OBJC_EXTENSIONS OFF)
  
  enable_language(OBJCXX)
  set(CMAKE_OBJCXX_STANDARD 17)
  set(CMAKE_OBJCXX_STANDARD_REQUIRED YES)
  set(CMAKE_OBJCXX_EXTENSIONS OFF)
endif()

# --- Sources ---

function(add_dashboard_sources)
  set(HD_SRC ${HD_SRC} ${ARGV} PARENT_SCOPE)
endfunction()

include("${HD_LIB_DIR}/CMakeLists.txt")

# --- Resources ---

build_dashboard_resource("${HD_RES_DIR}/images/2022_field.png"      "field_2022_png")
build_dashboard_resource("${HD_RES_DIR}/images/2022_cargo_blue.png" "cargo_blue_png")
build_dashboard_resource("${HD_RES_DIR}/images/2022_cargo_red.png"  "cargo_red_png")
build_dashboard_resource("${HD_RES_DIR}/images/2022_cargo_none.png" "cargo_none_png")

# --- Thirdparty ---

add_subdirectory(${TD_LIB_DIR})

# --- Build ---

if(TD_WINDOWS)
  add_executable(${PROJECT_NAME} WIN32 ${HD_SRC})
elseif(TD_MACOS)
  list(APPEND HD_RESOURCES "${HD_RES_DIR}/macos/Assets.xcassets")

  set(HD_BUNDLE_IDENTIFIER    "io.github.petelilley.thunderauto")
  set(HD_BUNDLE_NAME          "ThunderAuto")
  set(HD_VERSION              "${HD_VERSION}")
  set(HD_LONG_VERSION_STRING  "${HD_VERSION}")
  set(HD_SHORT_VERSION_STRING "${HD_VERSION_SHORT}")
  set(HD_COPYRIGHT            "Copyright © ${HD_YEAR} Peter P Lilley III. All rights reserved.")
  set(HD_APP_CATEGORY         "public.app-category.developer-tools")
  set(HD_BUNDLE_ICON_FILE     "bundle_icon.icns")

  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${HD_SRC} ${HD_RESOURCES})
  set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST          "${HD_RES_DIR}/macos/Info.plist.in"
    RESOURCE                          "${HD_RESOURCES}"
  )
elseif(TD_LINUX)
  add_executable(${PROJECT_NAME} ${HD_SRC})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${HD_INC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/res")

target_link_libraries(${PROJECT_NAME} ThunderDashboard)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${HD_DEF_LIST})
