#pragma once

#include "material.hpp"

class Metal : public Material
{
public:
	Metal(const Color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const
	{
		Vec3 reflected_direction = reflect(r_in.direction(), rec.normal);
		reflected_direction = unitVector(reflected_direction) + (fuzz * random_unit_vector());
		scattered = Ray(rec.position, reflected_direction);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	Color albedo;
	double fuzz;
};
