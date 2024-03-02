cmake_minimum_required(VERSION 3.5)

set(HD_VERSION_MAJOR "2024")
set(HD_VERSION_MINOR "0")
set(HD_VERSION_PATCH "0")
set(HD_VERSION "${HD_VERSION_MAJOR}.${HD_VERSION_MINOR}.${HD_VERSION_PATCH}")
set(HD_VERSION_SHORT "${HD_VERSION_MAJOR}.${HD_VERSION_MINOR}")

list(APPEND HD_DEF_LIST "HD_VERSION=${HD_VERSION_SHORT}")
list(APPEND HD_DEF_LIST "HD_VERSION_MAJOR=${HD_VERSION_MAJOR}")
list(APPEND HD_DEF_LIST "HD_VERSION_MINOR=${HD_VERSION_MINOR}")
list(APPEND HD_DEF_LIST "HD_VERSION_PATCH=${HD_VERSION_PATCH}")
list(APPEND HD_DEF_LIST "HD_VERSION_STR=\"${HD_VERSION}\"")

project(HomersDashboard LANGUAGES C CXX VERSION ${HD_VERSION})

option(HD_WITH_CS "Build with camera server" ON)
option(HD_DOWNLOAD_WPILIB "Download WPILib binaries instead of building them" OFF)
set(HD_DOWNLOAD_WPILIB_VERSION "2024.3.1" CACHE STRING "WPILib version to download")

if(HD_WITH_CS)
  list(APPEND HD_DEF_LIST "HD_WITH_CS")
endif()

#
# Project configuration.
#

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Disable in-source builds.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# Compiler flags.
if(MSVC)
  add_compile_options(/W4)
  message(STATUS "MSVC Compiler Detected")
else()
  add_compile_options(-Wall -Wextra -pedantic)
  add_compile_options(-Wno-deprecated-declarations -Wno-psabi)
endif()

# Default to release.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Debug build.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # compile_commands.json
  set(HD_DEBUG ON)
  list(APPEND HD_DEF_LIST "HD_DEBUG")
endif()

#
# Platform.
#

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(HD_WINDOWS ON)
  list(APPEND HD_DEF_LIST "HD_WINDOWS=1")
  set(HD_DOWNLOAD_OS "windows")
  set(OpenCV_DIR "C:\\ProgramFiles\\opencv\\build")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(HD_MACOS ON)
  list(APPEND HD_DEF_LIST "HD_MACOS=1")
  set(HD_DOWNLOAD_OS "osx")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(HD_LINUX ON)
  list(APPEND HD_DEF_LIST "HD_LINUX=1")
  set(HD_DOWNLOAD_OS "linux")
else()
  message(FATAL_ERROR "Target operating system '${CMAKE_SYSTEM_NAME}' not supported")
endif()

if(HD_MACOS)
  enable_language(OBJC)
  set(CMAKE_OBJC_STANDARD 11)
  set(CMAKE_OBJC_STANDARD_REQUIRED YES)
  set(CMAKE_OBJC_EXTENSIONS OFF)
  
  enable_language(OBJCXX)
  set(CMAKE_OBJCXX_STANDARD 17)
  set(CMAKE_OBJCXX_STANDARD_REQUIRED YES)
  set(CMAKE_OBJCXX_EXTENSIONS OFF)
endif()

#
# Sources.
#

set(HD_SRC_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(HD_INC_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(HD_THIRDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(HD_RES_DIR        "${CMAKE_CURRENT_SOURCE_DIR}/res")

function(add_dashboard_sources)
  set(HD_SRC ${HD_SRC} ${ARGV} PARENT_SCOPE)
endfunction()

include("${HD_SRC_DIR}/CMakeLists.txt")

#
# Build resources.
#

function(build_dashboard_resource RES_FILE RES_NAME)
  set(RES_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/res/${RES_NAME}.h")
  if(NOT EXISTS ${RES_OUTPUT})
    file(REMOVE ${RES_OUTPUT})
    file(READ ${RES_FILE} filedata HEX)
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
    file(APPEND ${RES_OUTPUT} "unsigned char ${RES_NAME}[] = {${filedata}};\nconst unsigned ${RES_NAME}_size = sizeof(${RES_NAME});\n")
  endif()
endfunction()

# build_dashboard_resource("${HD_RES_DIR}/fonts/Roboto/Roboto-Regular.ttf"       "Roboto_Regular_ttf")
# build_dashboard_resource("${HD_RES_DIR}/fonts/Roboto/Roboto-Bold.ttf"          "Roboto_Bold_ttf")
build_dashboard_resource("${HD_RES_DIR}/fonts/Ubuntu/Ubuntu-Regular.ttf"       "Ubuntu_Regular_ttf")
build_dashboard_resource("${HD_RES_DIR}/fonts/Ubuntu/Ubuntu-Bold.ttf"          "Ubuntu_Bold_ttf")
build_dashboard_resource("${HD_RES_DIR}/fonts/Font-Awesome/fa-regular-400.ttf" "FontAwesome_Regular_ttf")
build_dashboard_resource("${HD_RES_DIR}/fonts/Font-Awesome/fa-solid-900.ttf"   "FontAwesome_Solid_ttf")

build_dashboard_resource("${HD_RES_DIR}/images/no_camera.png" "no_camera_png")

# build_dashboard_resource("${HD_RES_DIR}/images/2022/field.png"      "_2022_field_png")
build_dashboard_resource("${HD_RES_DIR}/images/2022/cargo_blue.png" "_2022_cargo_blue_png")
build_dashboard_resource("${HD_RES_DIR}/images/2022/cargo_red.png"  "_2022_cargo_red_png")
build_dashboard_resource("${HD_RES_DIR}/images/2022/cargo_none.png" "_2022_cargo_none_png")

# build_dashboard_resource("${HD_RES_DIR}/images/2023/field.png"                 "_2023_field_png")
# build_dashboard_resource("${HD_RES_DIR}/images/2023/field_auto.png"            "_2023_field_auto_png")
# build_dashboard_resource("${HD_RES_DIR}/images/2023/grid_blue.png"             "_2023_grid_blue_png")
# build_dashboard_resource("${HD_RES_DIR}/images/2023/grid_red.png"              "_2023_grid_red_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/cube.png"                  "_2023_cube_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/cone.png"                  "_2023_cone_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/cube_none.png"             "_2023_cube_none_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/cone_none.png"             "_2023_cone_none_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/gp_none.png"               "_2023_gp_none_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/controller_driver.png"     "_2023_controller_driver_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/controller_aux.png"        "_2023_controller_aux_png")
build_dashboard_resource("${HD_RES_DIR}/images/2023/controller_aux_manual.png" "_2023_controller_aux_manual_png")

build_dashboard_resource("${HD_RES_DIR}/images/2024/field.png" "_2024_field_png")

#
# Thirdparty libraries.
#

if(NOT HD_WINDOWS)
  # glfw
  option(GLFW_BUILD_EXAMPLES OFF)
  option(GLFW_BUILD_TESTS OFF)
  option(GLFW_BUILD_DOCS OFF)
  option(GLFW_INSTALL OFF)
  add_subdirectory("${HD_THIRDPARTY_DIR}/glfw")

  # glad OpenGL loader
  add_subdirectory("${HD_THIRDPARTY_DIR}/glad")
endif()

# others
add_subdirectory("${HD_THIRDPARTY_DIR}/imgui")
add_subdirectory("${HD_THIRDPARTY_DIR}/stb_image")
add_subdirectory("${HD_THIRDPARTY_DIR}/IconFontCppHeaders")

# ps5 controllers.
if(HD_WINDOWS)
  # add_subdirectory("${HD_THIRDPARTY_DIR}/DualSense-Windows")
endif()

# allwpilib
set(HD_WPILIB_MODULES "wpiutil;wpinet;ntcore")
if(HD_WITH_CS)
  list(APPEND HD_WPILIB_MODULES "cscore;cameraserver")
endif()

if(HD_DOWNLOAD_WPILIB) # Download from internet.
  # Check architecture.
  if(HD_MACOS)
    set(HD_DOWNLOAD_ARCH "universal")
  else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64|amd64")
      set(HD_DOWNLOAD_ARCH "x86-64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64|arm64")
      set(HD_DOWNLOAD_ARCH "arm64")
    else()
      message(FATAL_ERROR "No pre-built wpilib binaries available for target architecture '${CMAKE_SYSTEM_PROCESSOR}'")
    endif()
  endif()

  set(HD_WPILIB_DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/wpilib_${HD_DOWNLOAD_WPILIB_VERSION}")
  string(REPLACE "." "_" HD_WPILIB_DOWNLOAD_DIR ${HD_WPILIB_DOWNLOAD_DIR})

  function(download_zip LIB_NAME LIB_URL LIB_DEST)
    message(STATUS "Downloading ${LIB_URL}")
    file(DOWNLOAD ${LIB_URL} ${LIB_DEST})
    message(STATUS "Downloaded ${LIB_NAME}")
  endfunction()

  function(extract_zip LIB_NAME ZIP_DIR LIB_DEST)
    message(STATUS "Extracting ${ZIP_DIR}")
    file(ARCHIVE_EXTRACT INPUT "${ZIP_DIR}" DESTINATION "${LIB_DEST}")
    message(STATUS "Extracted ${LIB_NAME}")
  endfunction()

  function(download_extract_lib LIB_NAME)
    set(LIB_URL "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/${LIB_NAME}/${LIB_NAME}-cpp/${HD_DOWNLOAD_WPILIB_VERSION}/${LIB_NAME}-cpp-${HD_DOWNLOAD_WPILIB_VERSION}-${HD_DOWNLOAD_OS}${HD_DOWNLOAD_ARCH}static.zip")
    set(LIB_DEST "${HD_WPILIB_DOWNLOAD_DIR}/${LIB_NAME}")

    download_zip(${LIB_NAME} ${LIB_URL} "${LIB_DEST}.zip")
    extract_zip(${LIB_NAME} "${LIB_DEST}.zip" ${LIB_DEST})
  endfunction()

  function(download_extract_headers LIB_NAME)
    set(INC_URL "https://frcmaven.wpi.edu/artifactory/release/edu/wpi/first/${LIB_NAME}/${LIB_NAME}-cpp/${HD_DOWNLOAD_WPILIB_VERSION}/${LIB_NAME}-cpp-${HD_DOWNLOAD_WPILIB_VERSION}-headers.zip")
    set(INC_DEST "${HD_WPILIB_DOWNLOAD_DIR}/${LIB_NAME}_include")

    download_zip(${LIB_NAME} ${INC_URL} "${INC_DEST}.zip")
    extract_zip(${LIB_NAME} "${INC_DEST}.zip" ${INC_DEST})
  endfunction()

  foreach(MODULE ${HD_WPILIB_MODULES})
    set(MODULE_LIB_DIR "${HD_WPILIB_DOWNLOAD_DIR}/${MODULE}/${HD_DOWNLOAD_OS}/${HD_DOWNLOAD_ARCH}/static")
    if(HD_WINDOWS)
      set(MODULE_LIB_NAME "${MODULE}.lib")
    else()
      set(MODULE_LIB_NAME "lib${MODULE}.a")
    endif()
    if(NOT EXISTS "${MODULE_LIB_DIR}/${MODULE_LIB_NAME}")
      download_extract_lib(${MODULE})
    endif()
    
    if(NOT EXISTS "${HD_WPILIB_DOWNLOAD_DIR}/${MODULE}_include")
      download_extract_headers(${MODULE})
    endif()

    link_directories("${MODULE_LIB_DIR}")
    include_directories("${HD_WPILIB_DOWNLOAD_DIR}/${MODULE}_include")
  endforeach()

else() # Build native.
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${HD_THIRDPARTY_DIR}/allwpilib/cmake/modules")
  set(WPILIB_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

  set(WITH_JAVA OFF CACHE BOOL "" FORCE)
  set(WITH_TESTS OFF CACHE BOOL "" FORCE)

  foreach(MODULE ${HD_WPILIB_MODULES})
    add_subdirectory("${HD_THIRDPARTY_DIR}/allwpilib/${MODULE}")
  endforeach()
endif()

#
# Build dashboard.
#

if(HD_WINDOWS)
  add_executable(${PROJECT_NAME} WIN32 ${HD_SRC})

elseif(HD_MACOS)
  list(APPEND HD_RESOURCES "${HD_RES_DIR}/macos/Assets.xcassets")

  set(HD_BUNDLE_IDENTIFIER    "io.github.petelilley.homersdashboard")
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
elseif(HD_LINUX)
  add_executable(${PROJECT_NAME} ${HD_SRC})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${HD_INC_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/res")

#
# Link libraries.
#

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} icons_font_awesome)
target_link_libraries(${PROJECT_NAME} stb_image)

if(HD_WINDOWS)
  #target_link_libraries(${PROJECT_NAME} DualSense-Windows)
else()
  target_link_libraries(${PROJECT_NAME} glfw)
  target_link_libraries(${PROJECT_NAME} glad)
endif()

if(HD_WITH_CS)
  find_package(OpenCV REQUIRED)
  if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV not found. Install OpenCV or add flag -DHD_WITH_CS=0")
  endif()

  target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
  target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
endif()

foreach(MODULE ${HD_WPILIB_MODULES})
  target_link_libraries(${PROJECT_NAME} ${MODULE})
endforeach()

if(HD_MACOS)
  target_link_libraries(${PROJECT_NAME} "-framework Foundation -framework AppKit -framework UniformTypeIdentifiers -framework AVFoundation -framework CoreMedia -framework CoreVideo")
endif()

#
# Definitions.
#
target_compile_definitions(${PROJECT_NAME} PRIVATE ${HD_DEF_LIST})

