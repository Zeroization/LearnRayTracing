#pragma once

#include "../ray/hittable.hpp"

class Translate : public Hittable
{
public:
	Translate(shared_ptr<Hittable> object, const Vec3& offset)
		: object(object), offset(offset)
	{
		bbox = object->bounding_box() + offset;
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		// 让光反向移动, 以实现物体移动的效果
		Ray offset_r(r.origin() - offset, r.direction(), r.time());

		// 移动后的光是否打到物体, 是的话就让相交点正向移动
		if (!object->hit(offset_r, ray_t, rec))
		{
			return false;
		}
		rec.position += offset;
		return true;
	}

	AABB bounding_box() const override { return bbox; }

private:
	shared_ptr<Hittable> object;
	Vec3 offset;
	AABB bbox;
};