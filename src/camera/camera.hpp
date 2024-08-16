#pragma once

#include "../rtweekend.h"
#include "../ray/hittable.hpp"

class Camera
{
public:
	double aspectRadio = 1.0;			// 图像的宽高比
	int imgWidth = 100;					// 图像宽度
	int samples_per_pixel = 10;			// 每像素采样数, 即SPP
	int max_depth = 10;					// 光线的最大弹射次数

	void render(const Hittable& world)
	{
		initialize();

		std::cout << std::format("P3\n{} {}\n255\n", imgWidth, imgHeight);
		for (int j = 0; j < imgHeight; ++j)
		{
			for (int i = 0; i < imgWidth; ++i)
			{
				Color final_pixel_color(0, 0, 0);
				for (int sampleCnt = 0; sampleCnt < samples_per_pixel; ++sampleCnt)
				{
					Ray r = get_ray(i, j);
					final_pixel_color += ray_color(r, max_depth, world);
				}
				writeColor(std::cout, pixel_sample_scale * final_pixel_color);
			}
		}
	}

private:
	int imgHeight;						// 图像高度
	double pixel_sample_scale;			// spp / 1
	Point3 center;						// 相机位置
	Point3 pixel00_pos;					// 像素(0, 0)的位置
	Vec3 pixel_delta_u;					// 定位像素用的辅助向量
	Vec3 pixel_delta_v;

	void initialize()
	{
		imgHeight = static_cast<int>(imgWidth / aspectRadio);
		imgHeight = (imgHeight < 1) ? 1 : imgHeight;				// 确保高度至少为1

		center = Point3(0, 0, 0);

		pixel_sample_scale = 1.0 / samples_per_pixel;

		// 相机设定
		double focalLength = 1.0;									// 焦距

		// 视口设定
		double viewportHeight = 2.0;
		double viewportWidth = viewportHeight * static_cast<double>(imgWidth) / imgHeight;

		// 4个辅助向量
		Vec3 viewport_u = Vec3(viewportWidth, 0, 0);
		Vec3 viewport_v = Vec3(0, -viewportHeight, 0);
		pixel_delta_u = viewport_u / imgWidth;
		pixel_delta_v = viewport_v / imgHeight;

		// 计算第一个像素位置
		Point3 viewport_upper_left = center - Point3(0, 0, focalLength) - viewport_u / 2 - viewport_v / 2;
		pixel00_pos = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	}

	// 随机生成一条从摄像机原点出发, 到达像素(i,j)周围的光线
	Ray get_ray(int i, int j) const
	{
		Vec3 offset = sample_square();
		Point3 pixel_sample = pixel00_pos + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

		Point3 ray_origin = center;
		Vec3 ray_direction = pixel_sample - ray_origin;

		return Ray(ray_origin, ray_direction);
	}

	// 返回位于[-.5,-.5]-[+.5,+.5]间的随机2D偏移量
	Vec3 sample_square() const
	{
		return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	Color ray_color(const Ray& r, int depth, const Hittable& world) const
	{
		// 递归退出
		if (depth <= 0)
		{
			return Color(0, 0, 0);
		}

		// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 和光线相交物体 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
		HitRecord rec;
		if (world.hit(r, Interval(0.001, infinity), rec))
		{
			// Lambert反射模型 (内外表面单位球均考虑)
			Vec3 direction = rec.normal + random_unit_vector();
			// 只考虑外表面单位球: Vec3 direction = rec.normal + random_on_hemisphere(rec.normal);
			// 开始下一轮反射
			return 0.5 * ray_color(Ray(rec.position, direction), depth - 1, world);
		}
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 和光线相交物体 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

		// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv 背景 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
		Vec3 dir = r.direction();
		// a in [-1, 1] -> a in [0, 1]
		double a = 0.5 * (dir.y() + 1.0);
		// 线性混合
		return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ 背景 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	}
};