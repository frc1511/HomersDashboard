include("${HD_LIB_DIR}/pages/CMakeLists.txt")
include("${HD_LIB_DIR}/popups/CMakeLists.txt")

add_dashboard_sources(
  "${HD_LIB_DIR}/dashboard.cpp"
  "${HD_LIB_DIR}/gyro.cpp"
)

if(TD_WINDOWS)
  add_dashboard_sources("${HD_LIB_DIR}/windows/ps5_controller_handler.cpp")
endif()