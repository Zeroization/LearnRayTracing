#pragma once 

#include "material.hpp"

class DiffuseLight : public Material
{
public:
	DiffuseLight(shared_ptr<Texture> tex) : tex(tex) {}
	DiffuseLight(const Color& emit) : tex(make_shared<SolidColor>(emit)) {}

	Color emitted(double u, double v, const Point3& p) const override
	{
		return tex->value(u, v, p);
	}

private:
	shared_ptr<Texture> tex;
};
