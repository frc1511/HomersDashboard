# include("${DASHBOARD_SRC_DIR}/platform/CMakeLists.txt")
include("${DASHBOARD_SRC_DIR}/pages/CMakeLists.txt")

add_dashboard_sources(
  "${DASHBOARD_SRC_DIR}/main.cpp"
  "${DASHBOARD_SRC_DIR}/app.cpp"
  "${DASHBOARD_SRC_DIR}/user_data.cpp"
)