#pragma once

#include "../rtweekend.h"

class HitRecord;

class Material
{
public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
	{
		return false;
	}
};
