#pragma once

#include "material.hpp"

class Isotropic : public Material
{
public:
	Isotropic(const Color& albedo) : tex(make_shared<SolidColor>(albedo)) {}
	Isotropic(shared_ptr<Texture> tex) : tex(tex) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const override
	{
		scattered = Ray(rec.position, random_unit_vector(), r_in.time());
		attenuation = tex->value(rec.u, rec.v, rec.position);
		return true;
	}

private:
	shared_ptr<Texture> tex;
};