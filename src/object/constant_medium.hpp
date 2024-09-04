#pragma once

#include "../material/isotropic.hpp"

class ConstantMedium : public Hittable
{
public:
	ConstantMedium(shared_ptr<Hittable> boundary, double density, shared_ptr<Texture> tex)
		: boundary(boundary), neg_inv_density(-1 / density), phase_function(make_shared<Isotropic>(tex))
	{}

	ConstantMedium(shared_ptr<Hittable> boundary, double density, const Color& albedo)
		: boundary(boundary), neg_inv_density(-1 / density), phase_function(make_shared<Isotropic>(albedo))
	{}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		HitRecord rec1, rec2;
		// 找到入射点
		if (!boundary->hit(r, Interval::universe, rec1))
			return false;
		// 找到出射点
		if (!boundary->hit(r, Interval(rec1.t + 0.0001, infinity), rec2))
			return false;
		// 对入射点和出射点做clamp()操作
		if (rec1.t < ray_t.min)	rec1.t = ray_t.min;
		if (rec2.t > ray_t.max)	rec2.t = ray_t.max;
		// 没有符合要求的入射&出射点
		if (rec1.t >= rec2.t)
			return false;

		if (rec1.t < 0)
			rec1.t = 0;

		auto ray_length = r.direction().length();
		auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
		auto hit_distance = neg_inv_density * std::log(random_double());

		if (hit_distance > distance_inside_boundary)
			return false;

		rec.t = rec1.t + hit_distance / ray_length;
		rec.position = r.at(rec.t);
		rec.normal = Vec3(1, 0, 0);		// 随意选的
		rec.front_face = true;			// 也是随意选的
		rec.material = phase_function;

		return true;
	}

	AABB bounding_box() const override { return boundary->bounding_box(); }

private:
	shared_ptr<Hittable> boundary;
	double neg_inv_density;
	shared_ptr<Material> phase_function;
};
