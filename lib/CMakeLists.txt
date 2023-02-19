include("${HD_LIB_DIR}/pages/CMakeLists.txt")
include("${HD_LIB_DIR}/popups/CMakeLists.txt")
include("${HD_LIB_DIR}/services/CMakeLists.txt")

add_dashboard_sources(
  "${HD_LIB_DIR}/dashboard.cpp"
  "${HD_LIB_DIR}/utils.cpp"
)

if(TD_WINDOWS)
  add_dashboard_sources("${HD_LIB_DIR}/ps5_controller_handler.cpp")
endif()