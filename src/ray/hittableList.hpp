#pragma once

#include "hittable.hpp"
#include <vector>

class HittableList : public Hittable {
public:
	std::vector<shared_ptr<Hittable>> objects;

	HittableList() {}
	HittableList(shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<Hittable> object) 
	{
		objects.push_back(object);
		bbox = AABB(bbox, object->bounding_box());
	}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override
	{
		HitRecord tmpRec;
		bool isHit = false;
		double ray_tClosest = ray_t.max;

		for (const auto& object : objects)
		{
			if (object->hit(r, Interval(ray_t.min, ray_tClosest), tmpRec))
			{
				isHit = true;
				ray_tClosest = tmpRec.t;
				rec = tmpRec;
			}
		}

		return isHit;
	}

	AABB bounding_box() const override { return bbox; }

private:
	AABB bbox;
};
