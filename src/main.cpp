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
	world.add(make_shared<Model>("resources/models/simple_dragon.obj", material_center));

    // 相机设定
    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 10;
    cam.max_depth = 50;
    cam.background = Color(0.70, 0.80, 1.00);

    cam.vfov = 90;
    cam.lookFrom = Point3(-0.6, 0, 0);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);
}

void book1Scene(HittableList& world, Camera& cam)
{
    auto checker_tex = make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker_tex)));

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
                    // auto center2 = center + Vec3(0, random_double(0, 0.5), 0);
                    // world.add(make_shared<Sphere>(center, center2, 0.2, sphere_material));
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
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
    cam.background = Color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void checkedSpheresScene(HittableList& world, Camera& cam)
{
    auto checker_tex = make_shared<CheckerTexture>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checker_tex)));
    world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker_tex)));

    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = Color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void earthScene(HittableList& world, Camera& cam)
{
    auto earth_tex = make_shared<ImageTexture>("resources/images/earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_tex);
    auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);
    world.add(globe);

    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = Color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = Point3(0, 0, -12);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void perlinSphereScene(HittableList& world, Camera& cam)
{
    auto perlin_tex = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlin_tex)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlin_tex)));

    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = Color(0.70, 0.80, 1.00);

    cam.vfov = 20;
    cam.lookFrom = Point3(13, 2, 3);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void quadsScene(HittableList& world, Camera& cam)
{
    auto left_red = make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green = make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue = make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal = make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    world.add(make_shared<Quad>(Point3(-3, -2, 5), Vec3(0, 0, -4), Vec3(0, 4, 0), left_red));
    world.add(make_shared<Quad>(Point3(-2, -2, 0), Vec3(4, 0, 0), Vec3(0, 4, 0), back_green));
    world.add(make_shared<Quad>(Point3(3, -2, 1), Vec3(0, 0, 4), Vec3(0, 4, 0), right_blue));
    world.add(make_shared<Quad>(Point3(-2, 3, 1), Vec3(4, 0, 0), Vec3(0, 0, 4), upper_orange));
    world.add(make_shared<Quad>(Point3(-2, -3, 5), Vec3(4, 0, 0), Vec3(0, 0, -4), lower_teal));

    cam.aspectRadio = 1.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = Color(0.70, 0.80, 1.00);

    cam.vfov = 80;
    cam.lookFrom = Point3(0, 0, 9);
    cam.lookAt = Point3(0, 0, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void simpleLightScene(HittableList& world, Camera& cam)
{
    auto perlin_tex = make_shared<NoiseTexture>(4);
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlin_tex)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlin_tex)));

    auto diffuse_light = make_shared<DiffuseLight>(Color(4, 4, 4));
    world.add(make_shared<Sphere>(Point3(0, 7, 0), 2, diffuse_light));
    world.add(make_shared<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), diffuse_light));

    cam.aspectRadio = 16.0 / 9.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = Color(0, 0, 0);

    cam.vfov = 20;
    cam.lookFrom = Point3(26, 3, 6);
    cam.lookAt = Point3(0, 2, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void cornellBoxScene(HittableList& world, Camera& cam)
{
    auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    auto red = make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    auto green = make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.add(make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    cam.aspectRadio = 1.0;
    cam.imgWidth = 600;
    cam.samples_per_pixel = 200;
    cam.max_depth = 50;
    cam.background = Color(0, 0, 0);

    cam.vfov = 40;
    cam.lookFrom = Point3(278, 278, -800);
    cam.lookAt = Point3(278, 278, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

int main()
{
    HittableList world;
    Camera cam;

    // 场景加载
    switch (8)
    {
        case 1: book1Scene(world, cam);         break;
        case 2: checkedSpheresScene(world, cam);break;
        case 3: modelScene(world, cam);         break;
        case 4: earthScene(world, cam);         break;
        case 5: perlinSphereScene(world, cam);  break;
        case 6: quadsScene(world, cam);         break;
        case 7: simpleLightScene(world, cam);   break;
        case 8: cornellBoxScene(world, cam);    break;
    }
	
    // BVH加载
    world = HittableList(make_shared<BVHNode>(world));

    // 场景渲染
	cam.render(world);

	return 0;
}
