#pragma once

class Interval
{
public:
	double min, max;

	// 初始化一个空区间
	Interval() : min(+infinity), max(-infinity) {}
	// 初始化一个[min, max]区间
	Interval(double min, double max) : min(min), max(max) {}
	// 合并两个区间
	Interval(const Interval& a, const Interval& b)
	{
		min = a.min <= b.min ? a.min : b.min;
		max = a.max >= b.max ? a.max : b.max;
	}

	// 返回区间长度
	double size() const { return max - min; }
	// min <= x <= max
	bool contains(double x) const { return x >= min && x <= max; }
	// min < x < max
	bool surrounds(double x) const { return x > min && x < max; }
	// 将x按[min, max]返回, 超过范围的返回对应min/max 
	double clamp(double x) const
	{
		if (x < min) return min;
		if (x > max) return max;
		return x;
	}
	// 将整个区间均匀扩大delta
	Interval expand(double delta) const
	{
		double padding = delta / 2;
		return Interval(min - padding, max + padding);
	}

	// 常用区间
	static const Interval empty, universe;
};

// 常用区间定义
const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

inline Interval operator+(const Interval& ival, double displacement)
{
	return Interval(ival.min + displacement, ival.max + displacement);
}

inline Interval operator+(double displacement, const Interval& ival)
{
	return ival + displacement;
}