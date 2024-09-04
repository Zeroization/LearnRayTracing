#pragma once

#include "../ray/hittable.hpp"
#include "../ray/hittableList.hpp"

class Quad : public Hittable
{
public:
	Quad(const Point3& Q, const Vec3& u, const Vec3& v, shared_ptr<Material> material)
		: Q(Q), u(u), v(v), material(material)
	{
		Vec3 n = cross(u, v);
		normal = unitVector(n);
		D = dot(normal, Q);
		w = n / dot(n, n);

		set_bounding_box();
	}

	// 为四个顶点计算包围盒
	virtual void set_bounding_box()
	{
		AABB bbox_diagonal1 = AABB(Q, Q + u + v);
		AABB bbox_diagonal2 = AABB(Q + u, Q + v);
		bbox = AABB(bbox_diagonal1, bbox_diagonal2);
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		double denom = dot(normal, r.direction());

		// 光线和平面平行, miss
		if (std::fabs(denom) < 1e-8)
		{
			return false;
		}

		// 求解t, 在范围外就返回false
		double t = (D - dot(normal, r.origin())) / denom;
		if (!ray_t.contains(t))
		{
			return false;
		}

		// 相交点是否在四边形内
		Point3 intersection = r.at(t);
		Vec3 planar_hitpt_vector = intersection - Q;
		double alpha = dot(w, cross(planar_hitpt_vector, v));
		double beta = dot(w, cross(u, planar_hitpt_vector));
		if (!is_interior(alpha, beta, rec))
		{
			return false;
		}

		// 更新rec
		rec.t = t;
		rec.position = intersection;
		rec.material = material;
		rec.set_face_normal(r, normal);

		return true;
	}

	// 判断点是否在四边形内部
	virtual bool is_interior(double a, double b, HitRecord& rec) const
	{
		Interval unit_interval = Interval(0, 1);
		if (!unit_interval.contains(a) || !unit_interval.contains(b))
		{
			return false;
		}

		rec.u = a;
		rec.v = b;
		return true;
	}

	AABB bounding_box() const override { return bbox; }

private:
	Point3 Q;
	Vec3 u, v;
	Vec3 w;
	shared_ptr<Material> material;
	AABB bbox;
	Vec3 normal;
	double D;
};

// 返回由两个点确定的3D盒子
inline shared_ptr<HittableList> box(const Point3& a, const Point3& b, shared_ptr<Material> material)
{
	auto sides = make_shared<HittableList>();

	Point3 min = Point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
	Point3 max = Point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));
	Vec3 dx = Vec3(max.x() - min.x(), 0, 0);
	Vec3 dy = Vec3(0, max.y() - min.y(), 0);
	Vec3 dz = Vec3(0, 0, max.z() - min.z());
	sides->add(make_shared<Quad>(Point3(min.x(), min.y(), max.z()), dx, dy, material));	// 前
	sides->add(make_shared<Quad>(Point3(max.x(), min.y(), max.z()), -dz, dy, material));	// 右
	sides->add(make_shared<Quad>(Point3(max.x(), min.y(), min.z()), -dx, dy, material));	// 后
	sides->add(make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dz, dy, material));	// 左
	sides->add(make_shared<Quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, material));	// 上
	sides->add(make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dx, dz, material));	// 下

	return sides;
}