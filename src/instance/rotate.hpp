#pragma once

#include "../ray/hittable.hpp"

enum class RotateAxis { X, Y, Z };

class Rotate : public Hittable
{
public:
	Rotate(shared_ptr<Hittable> object, RotateAxis axis, double angle)
	: object(object), axis(axis), angle(angle)
	{
		bbox = object->bounding_box();

		// 利用线性插值找旋转后的bbox
		Point3 min(infinity, infinity, infinity);
		Point3 max(-infinity, -infinity, -infinity);

		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				for (int k = 0; k < 2; ++k)
				{
					double x = i * bbox.x.max + (1 - i) * bbox.x.min;
					double y = j * bbox.y.max + (1 - j) * bbox.y.min;
					double z = k * bbox.z.max + (1 - k) * bbox.z.min;

					Vec3 tester = rotate(Vec3(x, y, z), axis);
					for (int c = 0; c < 3; ++c)
					{
						min[c] = std::fmin(min[c], tester[c]);
						max[c] = std::fmax(max[c], tester[c]);
					}
				}
			}
		}

		bbox = AABB(min, max);
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		// 首先逆向旋转光线, 将光线从世界坐标系转换为模型坐标系
		Point3 origin = rotate(r.origin(), axis, false);
		Vec3 direction = rotate(r.direction(), axis, false);
		Ray rotated_r(origin, direction, r.time());

		// 在模型坐标系中, 光线是否和物体相交
		if (!object->hit(rotated_r, ray_t, rec))
		{
			return false;
		}

		// 是的话就将正向旋转相交点和法线, 从模型坐标系转换到世界坐标系
		rec.position = rotate(rec.position, axis);
		rec.normal = rotate(rec.normal, axis);

		return true;
	}

	AABB bounding_box() const override { return bbox; }

private:
	shared_ptr<Hittable> object;
	RotateAxis axis;
	double angle;
	AABB bbox;

	// 默认按逆时针旋转向量Vec
	Vec3 rotate(const Vec3& vec, RotateAxis axis, bool is_ccw = true) const
	{
		double radians = degrees_to_radians(angle);
		if (!is_ccw)
		{
			radians = -radians;
		}
		double sin_theta = std::sin(radians);
		double cos_theta = std::cos(radians);

		Vec3 ret;
		if (axis == RotateAxis::X)
		{
			ret.e[0] = vec.x();
			ret.e[1] = (cos_theta * vec.y()) - (sin_theta * vec.z());
			ret.e[2] = (sin_theta * vec.y()) + (cos_theta * vec.z());
		}
		else if (axis == RotateAxis::Y)
		{
			ret.e[0] = (cos_theta * vec.x()) + (sin_theta * vec.z());
			ret.e[1] = vec.y();
			ret.e[2] = (-sin_theta * vec.x()) + (cos_theta * vec.z());
		}
		else if (axis == RotateAxis::Z)
		{
			ret.e[0] = (cos_theta * vec.x()) - (sin_theta * vec.y());
			ret.e[1] = (sin_theta * vec.x()) + (cos_theta * vec.y());
			ret.e[2] = vec.z();
		}

		return ret;
	}
};
