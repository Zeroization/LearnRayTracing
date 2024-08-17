#include "rtweekend.h"

#include "camera/camera.hpp"
#include "ray/hittable.hpp"
#include "ray/hittableList.hpp"
#include "material/lambertian.hpp"
#include "material/metal.hpp"
#include "object/sphere.hpp"

#include <format>

int main()
{
	// 材质设定
	auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
	auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
	auto material_left = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3);
	auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

	// 场景设定
	HittableList world;
	world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
	world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
	world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
	world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));
	

	// 初始化摄像机, 然后渲染
	Camera cam;
	cam.aspectRadio = 16.0 / 9.0;
	cam.imgWidth = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.render(world);

	return 0;
}
