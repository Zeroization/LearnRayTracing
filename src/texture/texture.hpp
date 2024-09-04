#pragma once

class Texture
{
public:
	virtual ~Texture() = default;

	virtual Color value(double u, double v, const Point3& p) const = 0;
};
