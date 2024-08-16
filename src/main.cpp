#include "rtweekend.h"

#include "camera/camera.hpp"
#include "ray/hittable.hpp"
#include "ray/hittableList.hpp"
#include "object/sphere.hpp"

#include <format>

int main()
{
	// 场景设定
	HittableList world;
	world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	// 初始化摄像机, 然后渲染
	Camera cam;
	cam.aspectRadio = 16.0 / 9.0;
	cam.imgWidth = 400;
	cam.samples_per_pixel = 100;
	cam.max_depth = 50;

	cam.render(world);

	return 0;
}
