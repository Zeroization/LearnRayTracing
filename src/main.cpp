#include "rtweekend.h"

#include "accelerate/bvh.hpp"
#include "camera/film.hpp"
#include "camera/camera.hpp"
#include "ray/hittable.hpp"
#include "ray/hittableList.hpp"
#include "material/materials.h"
#include "object/objects.h"

void modelScene(HittableList& world, Camera& cam)
{
	// 材质设定
	auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));

	// 场景设定
	world.add(make_shared<Model>("models/simple_dragon.obj", material_center));

    // 相机设定
    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 90;
    cam.lookFrom = Point3(-0.6, 0, 0);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);
}

void book1Scene(HittableList& world, Camera& cam)
{
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0.6;
    cam.focus_dist = 10.0;
}

int main()
{
    HittableList world;
    Camera cam;

    // 场景加载
    // modelScene(world, cam);
    book1Scene(world, cam);

    // BVH加载
    world = HittableList(make_shared<BVHNode>(world));

    // 场景渲染
	cam.render(world);

	return 0;
}
