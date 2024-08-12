#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>

// C++ std usings
using std::make_shared;
using std::shared_ptr;

// 常量
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = std::numbers::pi;

// 工具函数
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180.0;
}

// 自定义头文件
#include "util/color.hpp"
#include "util/interval.hpp"
#include "ray/ray.hpp"
#include "util/vec3.hpp"
