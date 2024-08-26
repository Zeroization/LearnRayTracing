#pragma once

#include "../util/vec3.hpp"

class Ray 
{
public:
	Ray() {}
	Ray(const Point3& origin, const Vec3& direction, double time)
		: orig(origin), dir(direction), tm(time) {}
	Ray(const Point3& origin, const Vec3& direction)
		: Ray(origin, direction, 0) {}

	const Point3& origin() const { return orig; }
	const Vec3& direction() const { return dir; }
	double time() const { return tm; }

	// P(t) = Orig + t*dir
	Point3 at(double t) const {
		return orig + t * dir;
	}

private:
	Point3 orig;
	Vec3 dir;
	double tm;
};
