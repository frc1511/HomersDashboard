cmake_minimum_required(VERSION 3.1)

set(DASHBOARD_VERSION_MAJOR "0")
set(DASHBOARD_VERSION_MINOR "1")
set(DASHBOARD_VERSION_PATCH "0")
set(DASHBOARD_VERSION "${DASHBOARD_VERSION_MAJOR}.${DASHBOARD_VERSION_MINOR}.${DASHBOARD_VERSION_PATCH}")
set(DASHBOARD_VERSION_SHORT "${DASHBOARD_VERSION_MAJOR}.${DASHBOARD_VERSION_MINOR}")

list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_VERSION=${DASHBOARD_VERSION_SHORT}")
list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_VERSION_MAJOR=${DASHBOARD_VERSION_MAJOR}")
list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_VERSION_MINOR=${DASHBOARD_VERSION_MINOR}")
list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_VERSION_PATCH=${DASHBOARD_VERSION_PATCH}")
list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_VERSION_STR=\"${DASHBOARD_VERSION}\"")

set(DASHBOARD_YEAR 2022)

project(HomersDashboard LANGUAGES C CXX VERSION ${DASHBOARD_VERSION})

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
  set(DASHBOARD_DEBUG ON)
  list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_DEBUG")
endif()

option(DASHBOARD_WITH_CS "Build with camera server" ON)
option(DASHBOARD_DOWNLOAD_WPILIB "Download WPILib binaries instead of building them" OFF)
set(DASHBOARD_DOWNLOAD_WPILIB_VERSION "" CACHE STRING "WPILib version to download")

if(NOT DASHBOARD_DOWNLOAD_WPILIB_VERSION)
  set(DASHBOARD_DOWNLOAD_WPILIB_VERSION "2022.4.1")
endif()

if (DASHBOARD_WITH_CS)
  list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_WITH_CS")
endif()

set(DASHBOARD_SRC_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DASHBOARD_INC_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(DASHBOARD_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")

set(DASHBOARD_RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(DASHBOARD_WINDOWS ON)
  list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_WINDOWS")
  set(DASHBOARD_DOWNLOAD_OS "windows")
  set(OpenCV_DIR "C:\\Program Files\\opencv\\build")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(DASHBOARD_MACOS ON)
  list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_MACOS")
  set(DASHBOARD_DOWNLOAD_OS "osx")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(DASHBOARD_LINUX ON)
  list(APPEND DASHBOARD_DEF_LIST "DASHBOARD_LINUX")
  set(DASHBOARD_DOWNLOAD_OS "linux")
else()
  message(FATAL_ERROR "Target operating system '${CMAKE_SYSTEM_NAME}' not supported")
endif()

set(DASHBOARD_DOWNLOAD_ARCH "x86-64")

if(DASHBOARD_MACOS)
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
  set(DASHBOARD_SRC ${DASHBOARD_SRC} ${ARGV} PARENT_SCOPE)
endfunction()

include("${DASHBOARD_SRC_DIR}/CMakeLists.txt")

# --- Resources ---

function(build_dashboard_resource RES_FILE RES_NAME)
  set(RES_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/res/${RES_NAME}.h")
  file(REMOVE ${RES_OUTPUT})
  file(READ ${RES_FILE} filedata HEX)
  string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
  file(APPEND ${RES_OUTPUT} "unsigned char ${RES_NAME}[] = {${filedata}};\nconst unsigned ${RES_NAME}_size = sizeof(${RES_NAME});\n")
endfunction()

build_dashboard_resource("${DASHBOARD_RES_DIR}/fonts/Roboto/Roboto-Regular.ttf"       "Roboto_Regular_ttf")
build_dashboard_resource("${DASHBOARD_RES_DIR}/fonts/Roboto/Roboto-Bold.ttf"          "Roboto_Bold_ttf")
build_dashboard_resource("${DASHBOARD_RES_DIR}/fonts/Ubuntu/Ubuntu-Regular.ttf"       "Ubuntu_Regular_ttf")
build_dashboard_resource("${DASHBOARD_RES_DIR}/fonts/Ubuntu/Ubuntu-Bold.ttf"          "Ubuntu_Bold_ttf")
build_dashboard_resource("${DASHBOARD_RES_DIR}/fonts/Font-Awesome/fa-regular-400.ttf" "FontAwesome_Regular_ttf")
build_dashboard_resource("${DASHBOARD_RES_DIR}/fonts/Font-Awesome/fa-solid-900.ttf"   "FontAwesome_Solid_ttf")

build_dashboard_resource("${DASHBOARD_RES_DIR}/images/no_camera.png"                  "no_camera_png")
build_dashboard_resource("${DASHBOARD_RES_DIR}/images/field-2022.png"                 "field_2022_png")

# --- Thirdparty ---

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_INSTALL OFF)
add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/glfw")
add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/imgui")
add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/IconFontCppHeaders")
add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/glad")
add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/fmt")
add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/stb_image")

set(DASHBOARD_WPILIB_DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/wpilib_${DASHBOARD_DOWNLOAD_WPILIB_VERSION}")
string(REPLACE "." "_" DASHBOARD_WPILIB_DOWNLOAD_DIR ${DASHBOARD_WPILIB_DOWNLOAD_DIR})

# WPILib ntcore and cscore.
if(DASHBOARD_DOWNLOAD_WPILIB)
  function(download_lib LIB_NAME LIB_URL LIB_DEST)
    message(STATUS "Downloading ${LIB_URL}")
    file(DOWNLOAD ${LIB_URL} ${LIB_DEST})
    message(STATUS "Downloaded ${LIB_NAME}")
  endfunction()

  function(extract_lib LIB_NAME ZIP_DIR LIB_DEST)
    message(STATUS "Extracting ${ZIP_DIR}")
    file(ARCHIVE_EXTRACT INPUT "${ZIP_DIR}" DESTINATION "${LIB_DEST}")
    message(STATUS "Extracted ${LIB_NAME}")
  endfunction()

  function(download_extract_lib LIB_NAME)
    set(LIB_URL "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/${LIB_NAME}/${LIB_NAME}-cpp/${DASHBOARD_DOWNLOAD_WPILIB_VERSION}/${LIB_NAME}-cpp-${DASHBOARD_DOWNLOAD_WPILIB_VERSION}-${DASHBOARD_DOWNLOAD_OS}${DASHBOARD_DOWNLOAD_ARCH}static.zip")
    set(LIB_DEST "${DASHBOARD_WPILIB_DOWNLOAD_DIR}/${LIB_NAME}")

    download_lib(${LIB_NAME} ${LIB_URL} "${LIB_DEST}.zip")
    extract_lib(${LIB_NAME} "${LIB_DEST}.zip" ${LIB_DEST})
  endfunction()

  function(download_extract_headers LIB_NAME)
    set(INC_URL "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/${LIB_NAME}/${LIB_NAME}-cpp/${DASHBOARD_DOWNLOAD_WPILIB_VERSION}/${LIB_NAME}-cpp-${DASHBOARD_DOWNLOAD_WPILIB_VERSION}-headers.zip")

    set(INC_DEST "${DASHBOARD_WPILIB_DOWNLOAD_DIR}/${LIB_NAME}_include")

    download_lib(${LIB_NAME} ${INC_URL} "${INC_DEST}.zip")
    extract_lib(${LIB_NAME} "${INC_DEST}.zip" ${INC_DEST})
  endfunction()

  download_extract_lib("ntcore")
  download_extract_headers("ntcore")

  download_extract_lib("wpiutil")
  download_extract_headers("wpiutil")

  if(DASHBOARD_WITH_CS)
    download_extract_lib("cscore")
    download_extract_headers("cscore")
  endif()
  
else()
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DASHBOARD_THIRDPARTY_DIR}/allwpilib/cmake/modules")
  set(WPILIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

  set(WITH_JAVA OFF CACHE BOOL "" FORCE)
  set(WITH_TESTS OFF CACHE BOOL "" FORCE)

  add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/allwpilib/wpinet")
  add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/allwpilib/wpiutil")
  add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/allwpilib/ntcore")

  #WPILib CS Core.
  if(DASHBOARD_WITH_CS)
    add_subdirectory("${DASHBOARD_THIRDPARTY_DIR}/allwpilib/cscore")
  endif()
endif()

# --- Build ---

if(DASHBOARD_WINDOWS)
  add_executable(${PROJECT_NAME} WIN32 ${DASHBOARD_SRC})
elseif(DASHBOARD_MACOS)
  list(APPEND DASHBOARD_RESOURCES "${DASHBOARD_RES_DIR}/macos/Assets.xcassets")

  set(DASHBOARD_BUNDLE_IDENTIFIER    "io.github.petelilley.thunderauto")
  set(DASHBOARD_BUNDLE_NAME          "ThunderAuto")
  set(DASHBOARD_VERSION              "${DASHBOARD_VERSION}")
  set(DASHBOARD_LONG_VERSION_STRING  "${DASHBOARD_VERSION}")
  set(DASHBOARD_SHORT_VERSION_STRING "${DASHBOARD_VERSION_SHORT}")
  set(DASHBOARD_COPYRIGHT            "Copyright © ${DASHBOARD_YEAR} Peter P Lilley III. All rights reserved.")
  set(DASHBOARD_APP_CATEGORY         "public.app-category.developer-tools")
  set(DASHBOARD_BUNDLE_ICON_FILE     "bundle_icon.icns")

  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${DASHBOARD_SRC} ${DASHBOARD_RESOURCES})
  set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST          "${DASHBOARD_RES_DIR}/macos/Info.plist.in"
    RESOURCE                          "${DASHBOARD_RESOURCES}"
  )
elseif(DASHBOARD_LINUX)
  add_executable(${PROJECT_NAME} ${DASHBOARD_SRC})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${DASHBOARD_INC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/res")

target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} icons_font_awesome)
target_link_libraries(${PROJECT_NAME} glad)
target_link_libraries(${PROJECT_NAME} fmt)
target_link_libraries(${PROJECT_NAME} stb_image)

if(DASHBOARD_DOWNLOAD_WPILIB)
  target_link_directories(${PROJECT_NAME} PRIVATE ${DASHBOARD_WPILIB_DOWNLOAD_DIR})

  function(find_lib_dir LIB_DIR LIB_NAME)
    find_library(${LIB_DIR}
      NAMES ${LIB_NAME}
      PATHS "${DASHBOARD_WPILIB_DOWNLOAD_DIR}/${LIB_NAME}/${DASHBOARD_DOWNLOAD_OS}/${DASHBOARD_DOWNLOAD_ARCH}/static"
      PATH_SUFFIXES "lib" "lib32" "lib64"
    )
  endfunction()

  function(find_inc_dir INC_DIR LIB_NAME)
    set(${INC_DIR} "${DASHBOARD_WPILIB_DOWNLOAD_DIR}/${LIB_NAME}_include" PARENT_SCOPE)
  endfunction()

  find_lib_dir(NTCORE_LIB_DIR "ntcore")
  find_inc_dir(NTCORE_INC_DIR "ntcore")
  target_link_libraries(${PROJECT_NAME} ${NTCORE_LIB_DIR})
  target_include_directories(${PROJECT_NAME} PUBLIC ${NTCORE_INC_DIR})

  find_lib_dir(WPIUTIL_LIB_DIR "wpiutil")
  find_inc_dir(WPIUTIL_INC_DIR "wpiutil")
  target_link_libraries(${PROJECT_NAME} ${WPIUTIL_LIB_DIR})
  target_include_directories(${PROJECT_NAME} PUBLIC ${WPIUTIL_INC_DIR})

  if(DASHBOARD_WITH_CS)
    find_package(OpenCV REQUIRED)
    if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV not found. Install OpenCV or add flag -DDASHBOARD_WITH_CS=0")
    endif()

    target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${OpenCV_INCLUDE_DIRS})

    find_lib_dir(CSCORE_LIB_DIR "cscore")
    find_inc_dir(CSCORE_INC_DIR "cscore")
    target_link_libraries(${PROJECT_NAME} ${CSCORE_LIB_DIR})
    target_include_directories(${PROJECT_NAME} PUBLIC ${CSCORE_INC_DIR})
  endif()
else()
  target_link_libraries(${PROJECT_NAME} ntcore)

  if(DASHBOARD_WITH_CS)
    target_link_libraries(${PROJECT_NAME} cscore)
  endif()
endif()

if(DASHBOARD_MACOS)
  target_link_libraries(${PROJECT_NAME} "-framework Foundation -framework AppKit -framework UniformTypeIdentifiers")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE ${DASHBOARD_DEF_LIST})
