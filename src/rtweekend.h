#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <numbers>
#include <random>

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

inline double random_double()
{
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
	return static_cast<int>(random_double(min, max + 1));
}

// 自定义头文件
#include "util/color.hpp"
#include "util/interval.hpp"
#include "ray/ray.hpp"
#include "util/vec3.hpp"
#include "util/thread_pool.hpp"
