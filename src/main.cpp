#include "rtweekend.h"
#include "ray/hittable.hpp"
#include "ray/hittableList.hpp"
#include "object/sphere.hpp"
#include <format>

Color ray_color(const Ray& r, const Hittable& world)
{
	// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 和光线相交物体 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	HitRecord rec;
	if (world.hit(r, Interval(0, infinity), rec))
	{
		return 0.5 * (rec.normal + Color(1, 1, 1));
	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 和光线相交物体 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

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

	// 场景设定
	HittableList world;
	world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

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
			Color pixel_color = ray_color(ray, world);

			writeColor(std::cout, pixel_color);
		}
	}
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 渲染 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	return 0;
}
