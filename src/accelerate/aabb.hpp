#pragma once

#include "../rtweekend.h"

class AABB
{
public:
	Interval x, y, z;

	// 创建一个空AABB
	AABB() = default;
	// 创建一个区间分量为x, y, z的AABB
	AABB(const Interval& x, const Interval& y, const Interval& z)
		: x(x), y(y), z(z) {}
	// 用两个点创建一个AABB
	AABB(const Point3& a, const Point3& b)
	{
		x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
		y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
		z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
	}
	// 用三个点创建一个AABB
	AABB(const Point3& a, const Point3& b, const Point3& c)
	{
		auto [minX, maxX] = std::minmax({a[0], b[0], c[0]});
		x = Interval(minX, maxX);
		auto [minY, maxY] = std::minmax({a[1], b[1], c[1]});
		y = Interval(minY, maxY);
		auto [minZ, maxZ] = std::minmax({a[2], b[2], c[2]});
		z = Interval(minZ, maxZ);
	}
	// 合并两个包围盒
	AABB(const AABB& box0, const AABB& box1)
	{
		x = Interval(box0.x, box1.x);
		y = Interval(box0.y, box1.y);
		z = Interval(box0.z, box1.z);
	}

	// 根据索引返回对应坐标轴区间, 0-x, 1-y, 2-z
	const Interval& axis_interval(int n) const
	{
		if (n == 1)	return y;
		if (n == 2)	return z;
		return x;
	}

	// 判断光线和AABB求交
	bool hit(const Ray& r, Interval ray_t) const
	{
		const Point3& ray_orig = r.origin();
		const Vec3& ray_dir = r.direction();

		for (int axis = 0; axis < 3; ++axis)
		{
			const Interval& ax = axis_interval(axis);
			const double adinv = 1.0 / ray_dir[axis];

			double t0 = (ax.min - ray_orig[axis]) * adinv;
			double t1 = (ax.max - ray_orig[axis]) * adinv;

			if (t0 < t1)
			{
				if (t0 > ray_t.min)	ray_t.min = t0;
				if (t1 < ray_t.max)	ray_t.max = t1;
			}
			else
			{
				if (t1 > ray_t.min)	ray_t.min = t1;
				if (t0 < ray_t.max)	ray_t.max = t0;
			}

			if (ray_t.max <= ray_t.min)
			{
				return false;
			}
		}

		return true;
	}
};
