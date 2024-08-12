#pragma once

class Interval
{
public:
	double min, max;

	// 初始的区间的空的
	Interval() : min(+infinity), max(-infinity) {}
	Interval(double min, double max) : min(min), max(max) {}

	double size() const { return max - min; }
	bool contains(double x) const { return x >= min && x <= max; }
	bool surrounds(double x) const { return x > min && x < max; }

	// 常用区间
	static const Interval empty, universe;
};

// 常用区间定义
const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);