#pragma once

#include "interval.hpp"
#include "vec3.hpp"

using Color = Vec3;

inline double linear_to_gamma(double linear_component)
{
	if (linear_component > 0)
	{
        return std::sqrt(linear_component);
	}
    return 0;
}

inline void writeColor(std::ostream& out, const Color& pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // 将数据从线性空间变换到Gamma空间
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // 将属于[0, 1]的RGB分量变换到[0, 255]上
    static const Interval intensity(0.000, 0.999);
    int rByte = static_cast<int>(256 * intensity.clamp(r));
    int gByte = static_cast<int>(256 * intensity.clamp(g));
    int bByte = static_cast<int>(256 * intensity.clamp(b));

    // 写到输出流out中
    out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}
