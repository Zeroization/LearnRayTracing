#pragma once

#include "../rtweekend.h"

class HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual Color emitted(double u, double v, const Point3& p) const
	{
		return Color(0, 0, 0);
	}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};
