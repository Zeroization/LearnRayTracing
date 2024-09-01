#pragma once

#include "../util/perlin.hpp"
#include "texture.hpp"

class NoiseTexture : public Texture
{
public:
	NoiseTexture(double scale) : scale(scale) {}

	Color value(double u, double v, const Point3& p) const override
	{
		return Color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
	}

private:
	Perlin noise;
	double scale;
};