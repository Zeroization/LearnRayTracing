﻿#pragma once

#include "../rtweekend.h"
#include "../accelerate/aabb.hpp"

// 不添加定义, 避免循环引用
class Material;

class HitRecord 
{
public:
	Point3 position;
	Vec3 normal;
	shared_ptr<Material> material;
	double t;
	double u, v;
	bool front_face;

	
	void set_face_normal(const Ray& r, const Vec3& outward_normal) 
	{
		// 注意: 总认为outward_normal是单位向量
		front_face = dot(r.direction(), outward_normal) < 0.0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable 
{
public:
	virtual ~Hittable() = default;

	virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;

	virtual AABB bounding_box() const = 0;
};
