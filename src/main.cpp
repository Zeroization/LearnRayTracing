#include "util/vec3.hpp"
#include "util/color.hpp"
#include "ray/ray.hpp"

#include <iostream>
#include <format>

// 返回光线r与以center为球心, radius为半径的球相交的最近点
double hit_sphere(const Point3& center, double radius, const Ray& r)
{
	// oc = 球心C - 光线原点Q
	Vec3 oc = center - r.origin();
	// 求根公式
	double a = r.direction().lengthSquared();
	double h = dot(r.direction(), oc);
	double c = oc.lengthSquared() - radius * radius;
	double delta = h * h - a * c;
	// 返回有实数解的结果
	if (delta < 0)
	{
		return -1.0;
	}
	return (h - std::sqrt(delta)) / a;
}

Color ray_color(const Ray& r)
{
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 球 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	double t = hit_sphere(Point3(0, 0, -1), 0.5, r);
	if (t > 0.0)
	{
		Vec3 normal = unitVector(r.at(t) - Vec3(0, 0, -1));
		return 0.5 * Color(normal.x() + 1, normal.y() + 1, normal.z() + 1);
	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 球 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 背景 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	Vec3 dir = r.direction();
	// [-1, 1] -> [0, 1]
	double a = 0.5 * (dir.y() + 1.0);
	// 线性混合
	return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 背景 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
}

int main()
{
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 数据设置 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// 图片设定
	double aspectRadio = 16.0 / 9.0;
	int imgWidth = 400;
	int imgHeight = static_cast<int>(imgWidth / aspectRadio);
	imgHeight = (imgHeight < 1) ? 1 : imgHeight;				// 确保高度至少为1

	// 相机设定
	double focalLength = 1.0;				// 焦距
	Point3 cameraCenter = Point3(0, 0, 0);	// 相机位置

	// 视口设定
	double viewportHeight = 2.0;
	double viewportWidth = viewportHeight * static_cast<double>(imgWidth) / imgHeight;

	// 4个辅助向量
	Vec3 viewport_u = Vec3(viewportWidth, 0, 0);
	Vec3 viewport_v = Vec3(0, -viewportHeight, 0);
	Vec3 pixel_delta_u = viewport_u / imgWidth;
	Vec3 pixel_delta_v = viewport_v / imgHeight;

	// 计算第一个像素位置
	Point3 viewport_upper_left = cameraCenter - Point3(0, 0, focalLength) - viewport_u / 2 - viewport_v / 2;
	Point3 pixel00_pos = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 数据设置 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 渲染 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	std::cout << std::format("P3\n{} {}\n255\n", imgWidth, imgHeight);
	for (int j = 0; j < imgHeight; ++j) 
	{
		for (int i = 0; i < imgWidth; ++i)
		{
			Point3 cur_pixel_center = pixel00_pos + (i * pixel_delta_u) + (j * pixel_delta_v);
			Vec3 ray_direction = unitVector((cur_pixel_center - cameraCenter));
			Ray ray(cameraCenter, ray_direction);
			Color pixel_color = ray_color(ray);

			writeColor(std::cout, pixel_color);
		}
	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 渲染 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	return 0;
}
