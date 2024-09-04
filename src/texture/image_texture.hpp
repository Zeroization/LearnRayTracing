#pragma once

#include "rtw_image.hpp"
#include "texture.hpp"

class ImageTexture : public Texture
{
public:
	ImageTexture(const char* filepath) : image(filepath) {}

	Color value(double u, double v, const Point3& p) const override
	{
		// 没有材质就返回青色
		if (image.height() <= 0)	return Color(0, 1, 1);

		// 将u,v限定至正确的范围, 其中v需要反转
		u = Interval(0, 1).clamp(u);
		v = 1.0 - Interval(0, 1).clamp(v);

		int i = static_cast<int>(u * image.width());
		int j = static_cast<int>(v * image.height());
		auto pixel = image.pixel_data(i, j);

		double color_scale = 1.0 / 255.0;
		return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
	}

private:
	RTWImage image;
};