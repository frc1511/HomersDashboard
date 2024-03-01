include("${HD_SRC_DIR}/pages/CMakeLists.txt")
include("${HD_SRC_DIR}/popups/CMakeLists.txt")
include("${HD_SRC_DIR}/services/CMakeLists.txt")

add_dashboard_sources(
  "${HD_SRC_DIR}/main.cpp"
  "${HD_SRC_DIR}/app.cpp"
  "${HD_SRC_DIR}/nt_handler.cpp"
  "${HD_SRC_DIR}/opengl_utils.cpp"
)

