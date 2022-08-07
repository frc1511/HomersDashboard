#include <platform/platform.h>

#ifdef DASHBOARD_MACOS
# include <platform/macos/macos.h>
#elif DASHBOARD_WINDOWS
# include <platform/windows/windows.h>
#elif DASHBOARD_LINUX
# include <platform/linux/linux.h>
#endif

Platform* Platform::get_current() {
#ifdef DASHBOARD_MACOS
  return PlatformMacOS::get();
#elif DASHBOARD_WINDOWS
  return PlatformWindows::get();
#elif DASHBOARD_LINUX
  return PlatformLinux::get();
#endif
  std::cerr << "Invalid platform" << std::endl;
  return nullptr;
}