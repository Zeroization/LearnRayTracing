#pragma once

#include "material.hpp"

class Lambertian : public Material
{
public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		// Lambert反射模型 (内外表面单位球均考虑)
		Vec3 scatter_direction = rec.normal + random_unit_vector();
		// 只考虑外表面单位球: Vec3 direction = rec.normal + random_on_hemisphere(rec.normal);

		// 处理边界问题
		if (scatter_direction.near_zero())
		{
			scatter_direction = rec.normal;
		}

		scattered = Ray(rec.position, scatter_direction, r_in.time());
		attenuation = albedo;
		return true;
	}

private:
	Color albedo;
};
