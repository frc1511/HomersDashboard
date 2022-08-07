set(DASHBOARD_PLATFORM_DIR "${DASHBOARD_SRC_DIR}/platform")

add_dashboard_sources(
  "${DASHBOARD_PLATFORM_DIR}/platform.cpp"
)

if(DASHBOARD_MACOS)
  include("${DASHBOARD_PLATFORM_DIR}/macos/CMakeLists.txt")
elseif(DASHBOARD_WINDOWS)
  include("${DASHBOARD_PLATFORM_DIR}/windows/CMakeLists.txt")
elseif(DASHBOARD_LINUX)
  include("${DASHBOARD_PLATFORM_DIR}/linux/CMakeLists.txt")
endif()
