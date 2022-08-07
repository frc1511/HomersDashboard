# include("${DASHBOARD_SRC_DIR}/platform/CMakeLists.txt")
# include("${DASHBOARD_SRC_DIR}/pages/CMakeLists.txt")
# include("${DASHBOARD_SRC_DIR}/popups/CMakeLists.txt")

add_dashboard_sources(
  "${DASHBOARD_SRC_DIR}/main.cpp"
  "${DASHBOARD_SRC_DIR}/app.cpp"
)