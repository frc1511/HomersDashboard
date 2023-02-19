add_dashboard_sources(
  "${HD_LIB_DIR}/services/gyro.cpp"
)

if(TD_WINDOWS)
  add_dashboard_sources(
    "${HD_LIB_DIR}/services/2023/arduino.cpp"
  )
endif()
