#pragma once

#include "../ray/hittable.hpp"

class Triangle : public Hittable
{
public:
	Triangle(const Point3& p0, const Point3& p1, const Point3& p2,
			 const Vec3& n0, const Vec3& n1, const Vec3& n2,
			 shared_ptr<Material> material)
		: p0(p0), p1(p1), p2(p2), n0(n0), n1(n1), n2(n2), material(material) {}

	Triangle(const Point3& p0, const Point3& p1, const Point3& p2, 
			 shared_ptr<Material> material)
		: p0(p0), p1(p1), p2(p2), material(material) 
	{
		Vec3 e1 = p1 - p0;
		Vec3 e2 = p2 - p0;
		Vec3 normal = unitVector(cross(e1, e2));
		n0 = normal;
		n1 = normal;
		n2 = normal;
	}


	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		// 使用MT法判光线和三角形面相交
		Vec3 e1 = p1 - p0;
		Vec3 e2 = p2 - p0;
		Vec3 s1 = cross(r.direction(), e2);
		double inv_det = 1.0 / dot(s1, e1);

		Vec3 s = r.origin() - p0;
		double u = dot(s1, s) * inv_det;
		if (u < 0 || u > 1) { return false; }

		Vec3 s2 = cross(s, e1);
		double v = dot(s2, r.direction()) * inv_det;
		if (v < 0 || u + v > 1) { return false; }

		double hit_t = dot(s2, e2) * inv_det;
		if (ray_t.surrounds(hit_t))
		{
			// 将三顶点法线进行插值
			Vec3 normal = (1.0 - u - v) * n0 + u * n1 + v * n2;

			// 记录相交信息
			rec.t = hit_t;
			rec.position = r.at(hit_t);
			rec.normal = unitVector(normal);
			// rec.set_face_normal(r, unitVector(normal));
			rec.material = material;

			return true;
		}
		return false;
	}

private:
	Point3 p0, p1, p2;	// 三角形的顶点位置信息
	Vec3 n0, n1, n2;	// 三角形的顶点法线信息
	shared_ptr<Material> material;
};
