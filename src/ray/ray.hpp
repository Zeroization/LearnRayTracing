#pragma once

#include "../util/vec3.hpp"

class Ray 
{
public:
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

	const Point3& origin() const { return orig; }
	const Vec3& direction() const { return dir; }

	// P(t) = Orig + t*dir
	Point3 at(double t) const {
		return orig + t * dir;
	}

private:
	Point3 orig;
	Vec3 dir;
};
