#pragma once

#include "texture.hpp"

class CheckerTexture : public Texture
{
public:
	CheckerTexture(double scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
		: inv_scale(1.0 / scale), even(even), odd(odd) {}
	CheckerTexture(double scale, const Color& c1, const Color& c2)
		:CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

	Color value(double u, double v, const Point3& p) const override
	{
		int x_i = static_cast<int>(std::floor(inv_scale * p.x()));
		int y_i = static_cast<int>(std::floor(inv_scale * p.y()));
		int z_i = static_cast<int>(std::floor(inv_scale * p.z()));

		bool isEven = (x_i + y_i + z_i) % 2 == 0;
		return isEven ? even->value(u, v, p) : odd->value(u, v, p);
	}

private:
	double inv_scale;
	shared_ptr<Texture> even;
	shared_ptr<Texture> odd;
};
