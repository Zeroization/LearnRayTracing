#pragma once

#include "../ray/hittable.hpp"

class Sphere : public Hittable
{
public:
	Sphere(const Point3& center, double radius)
		: center(center), radius(std::fmax(0, radius)) {}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		// oc = 球心C - 光线原点Q
		Vec3 oc = center - r.origin();
		
		// 求根公式
		double a = r.direction().lengthSquared();
		double h = dot(r.direction(), oc);
		double c = oc.lengthSquared() - radius * radius;
		double delta = h * h - a * c;
		if (delta < 0)
		{
			return false;
		}
		double sqrtDelta = std::sqrt(delta);

		// 找到位于[tMin, tMax]最近的相交点
		double root = (h - sqrtDelta) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + sqrtDelta) / a;
			if (!ray_t.surrounds(root)) {
				return false;
			}
		}

		// 记录相交信息
		rec.t = root;
		rec.position = r.at(rec.t);
		Vec3 outward_normal = (rec.position - center) / radius;	// 利用定义简化法线计算
		rec.set_face_normal(r, outward_normal);

		return true;
	}

private:
	Point3 center;
	double radius;
};
