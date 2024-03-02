#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <mutex>
#include <numbers>
#include <numeric>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

inline bool float_eq(float x, float y) { return std::fabs(x - y) < 1e-6; }

#ifdef HD_WINDOWS
#include <Windows.h>
#include <d3d11.h>
#endif

