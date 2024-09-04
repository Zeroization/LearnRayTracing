#include "rtweekend.h"

#include "accelerate/bvh.hpp"
#include "camera/film.hpp"
#include "camera/camera.hpp"
#include "ray/hittable.hpp"
#include "ray/hittableList.hpp"
#include "instance/transform.h"
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

    shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<Rotate>(box1, RotateAxis::Y, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(box1);
    shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<Rotate>(box2, RotateAxis::Y, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(box2);

    cam.aspectRadio = 1.0;
    cam.imgWidth = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.background = Color(0, 0, 0);

    cam.vfov = 40;
    cam.lookFrom = Point3(278, 278, -800);
    cam.lookAt = Point3(278, 278, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

void cornellSmokeScene(HittableList& world, Camera& cam)
{
    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

    world.add(make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point3(113, 554, 127), Vec3(330, 0, 0), Vec3(0, 0, 305), light));
    world.add(make_shared<Quad>(Point3(0, 555, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<Rotate>(box1, RotateAxis::Y, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));

    shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<Rotate>(box2, RotateAxis::Y, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    world.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

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

void book2Scene(HittableList& world, Camera& cam, int img_Width, int spp, int max_depth)
{
    // 地面的盒子
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));
	HittableList boxes1;
    int boxes_per_size = 20;
    for (int i = 0; i < boxes_per_size; ++i)
    {
	    for (int j = 0; j < boxes_per_size; ++j)
	    {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto y0 = 0.0;
            auto z0 = -1000.0 + j * w;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
	    }
    }
    world.add(make_shared<BVHNode>(boxes1));

    // 光源
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(make_shared<Quad>(Point3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light));

    // 运动模糊球
    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, center2, 50, sphere_material));

    // 普通球
    world.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    world.add(make_shared<Sphere>(Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    // 次表面反射球
    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, 0.0001, Color(1, 1, 1)));

    // 地图球
    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("resources/images/earthmap.jpg"));
    world.add(make_shared<Sphere>(Point3(400, 200, 400), 100, emat));

    // 柏林噪声球
    auto perlin_tex = make_shared<NoiseTexture>(0.2);
    world.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(perlin_tex)));

    // 方形排列的球们
    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    int ns = 1000;
    for (int i = 0; i < ns; ++i)
    {
        boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }
    world.add(make_shared<Translate>(
        make_shared<Rotate>(make_shared<BVHNode>(boxes2), RotateAxis::Y, 15)
        , Vec3(-100, 270, 395))
    );

    // 摄像机设置
    cam.aspectRadio = 1.0;
    cam.imgWidth = img_Width;
    cam.samples_per_pixel = spp;
    cam.max_depth = max_depth;
    cam.background = Color(0, 0, 0);

    cam.vfov = 40;
    cam.lookFrom = Point3(478, 278, -600);
    cam.lookAt = Point3(278, 278, 0);
    cam.vup = Vec3(0, 1, 0);

    cam.defocus_angle = 0;
}

int main()
{
    HittableList world;
    Camera cam;

    // 场景加载
    switch (10)
    {
        case 1: book1Scene(world, cam);         break;
        case 2: checkedSpheresScene(world, cam);break;
        case 3: modelScene(world, cam);         break;
        case 4: earthScene(world, cam);         break;
        case 5: perlinSphereScene(world, cam);  break;
        case 6: quadsScene(world, cam);         break;
        case 7: simpleLightScene(world, cam);   break;
        case 8: cornellBoxScene(world, cam);    break;
        case 9: cornellSmokeScene(world, cam);  break;
        case 10: 
            book2Scene(world, cam, 800, 10000, 40);
            break;
        default:
            book2Scene(world, cam, 400, 250, 4);
            break;
    }
	
    // BVH加载
    world = HittableList(make_shared<BVHNode>(world));

    // 场景渲染
	cam.render(world);

	return 0;
}
