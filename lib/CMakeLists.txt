include("${HD_LIB_DIR}/pages/CMakeLists.txt")
include("${HD_LIB_DIR}/popups/CMakeLists.txt")

add_dashboard_sources(
  "${HD_LIB_DIR}/dashboard.cpp"
  "${HD_LIB_DIR}/gyro.cpp"
)