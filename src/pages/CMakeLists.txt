set(HD_PAGES_DIR "${HD_SRC_DIR}/pages")

add_dashboard_sources(
  "${HD_PAGES_DIR}/competition_info_page.cpp"
  "${HD_PAGES_DIR}/match_timer_page.cpp"
  "${HD_PAGES_DIR}/network_tables_page.cpp"
  "${HD_PAGES_DIR}/autonomous_chooser_page.cpp"
  "${HD_PAGES_DIR}/camera_page.cpp"
  "${HD_PAGES_DIR}/intake_camera_page.cpp"
  "${HD_PAGES_DIR}/limelight_camera_page.cpp"
  "${HD_PAGES_DIR}/robot_position_page.cpp"

  "${HD_PAGES_DIR}/2022/ball_count_page.cpp"
  "${HD_PAGES_DIR}/2022/shooter_position_page.cpp"

  "${HD_PAGES_DIR}/2023/lift_page.cpp"
  "${HD_PAGES_DIR}/2023/grabber_page.cpp"
  "${HD_PAGES_DIR}/2023/controllers_page.cpp"

  "${HD_PAGES_DIR}/2024/arm_page.cpp"
  "${HD_PAGES_DIR}/2024/shooter_page.cpp"
)

