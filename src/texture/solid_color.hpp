#pragma once

#include "texture.hpp"

class SolidColor : public Texture
{
public:
	SolidColor(const Color& albedo) : albedo(albedo) {}
	SolidColor(double r, double g, double b) : SolidColor(Color(r, g, b)) {}

	Color value(double u, double v, const Point3& p) const override
	{
		return albedo;
	}

private:
	Color albedo;
};
