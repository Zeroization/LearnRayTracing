#pragma once

#include <format>

class Vec3 {
public:
	double e[3];

	Vec3() : e{0, 0, 0} {}
	Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	// 运算符
	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	double operator[] (int i) const { return e[i]; }
	double& operator[] (int i) { return e[i]; }
	Vec3& operator+= (const Vec3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	Vec3& operator*= (double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	Vec3& operator/= (double t)
	{
		return *this *= 1 / t;
	}

	// 获取相关属性
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	double lengthSquared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
	double length() const { return std::sqrt(lengthSquared()); }

	// 边界问题
	bool near_zero() const
	{
		// 如果该向量很接近0就返回true
		double s = 1e-8;
		return ((std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s));
	}

	// 生成随机数
	static Vec3 random()
	{
		return Vec3(random_double(), random_double(), random_double());
	}

	static Vec3 random(double min, double max)
	{
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
};

// 使用Point3作为Vec3别名, 用于表示几何信息
using Point3 = Vec3;

// 一些工具函数
inline std::ostream& operator<<(std::ostream& out, const Vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) 
{
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) 
{
	return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) 
{
	return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) 
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) 
{
	return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vec3 unitVector(const Vec3& v) 
{
	return v / v.length();
}

inline Vec3 random_in_unit_sphere()
{
	while (true)
	{
		Vec3 p = Vec3::random(-1, 1);
		if (p.lengthSquared() < 1)
		{
			return p;
		}
	}
}

// 返回一个范围是着色点单位球的随机向量
inline Vec3 random_unit_vector()
{
	return unitVector(random_in_unit_sphere());
}

// 返回一个范围是着色点外半球的随机向量
inline Vec3 random_on_hemisphere(const Vec3& normal)
{
	Vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0)
	{
		return on_unit_sphere;
	}
	return -on_unit_sphere;
}

// 返回入射方向v,单位向量n的镜面反射向量
inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
	// 点乘结果是大小, 因此还要乘个向量n
	return v - 2 * dot(v, n) * n;
}

// 利用斯涅尔定律计算反射光线
inline Vec3 refract(const Vec3& v, const Vec3& n, double etaI_div_etaR)
{
	double cos_theta = std::fmin(dot(-v, n), 1.0);
	Vec3 r_out_perp = etaI_div_etaR * (v + cos_theta * n);
	Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.lengthSquared())) * n;
	return r_out_perp + r_out_parallel;
}