#pragma once

#include "vec3.hpp"

using Color = Vec3;

inline void writeColor(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // 将属于[0, 1]的RGB分量变换到[0, 255]上
    int rByte = static_cast<int>(255.999 * r);
    int gByte = static_cast<int>(255.999 * g);
    int bByte = static_cast<int>(255.999 * b);

    // 写到输出流out中
    out << rByte << ' ' << gByte << ' ' << bByte << '\n';
}
