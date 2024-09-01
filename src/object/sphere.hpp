#pragma once

#include "../ray/hittable.hpp"

class Sphere : public Hittable
{
public:
	// 静态球
	Sphere(const Point3& center, double radius, shared_ptr<Material> material)
		: center1(center), radius(std::fmax(0, radius)), material(material), is_moving(false)
	{
		Vec3 rvec = Vec3(radius, radius, radius);
		bbox = AABB(center1 - rvec, center1 + rvec);
	}

	// 动态球
	Sphere(const Point3& center,  const Point3& center2, double radius, shared_ptr<Material> material)
		: center1(center), radius(std::fmax(0, radius)), material(material), is_moving(true)
	{
		Vec3 rvec = Vec3(radius, radius, radius);
		AABB box0(center1 - rvec, center1 + rvec);
		AABB box1(center2 - rvec, center2 + rvec);
		bbox = AABB(box0, box1);

		center_vec = center2 - center1;
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		Point3 center = is_moving ? sphere_center(r.time()) : center1;

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
		get_sphere_uv(outward_normal, rec.u, rec.v);
		rec.material = material;

		return true;
	}

	AABB bounding_box() const override { return bbox; }

private:
	Point3 center1;
	double radius;
	shared_ptr<Material> material;
	bool is_moving;
	Vec3 center_vec;
	AABB bbox;

	// 根据属于[0, 1]时间, 返回属于[center1, center2]线性插值的位置
	Point3 sphere_center(double time) const
	{
		return center1 + time * center_vec;
	}

	/**
	 * 将直角坐标转换为UV坐标
	 * @param p 单位球面上1点，用直角坐标表示
	 * @param u 返回方位角\phi到[0, 1]的映射
	 * @param v 返回极角\theta到[0, 1]的映射
	 */
	static void get_sphere_uv(const Point3& p, double& u, double& v)
	{
		double theta = std::acos(-p.y());
		double phi = std::atan2(-p.z(), p.x()) + pi;

		u = phi / (2 * pi);
		v = theta / pi;
	}
};
