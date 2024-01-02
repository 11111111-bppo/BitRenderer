#include <chrono>
#include <string>

#include "image.h"
#include "sphere.h"
#include "hittable_list.h"
#include "camera.h"
#include "logger.h"
#include "bvh_node.h"
#include "quad.h"
#include "constant_medium.h"

using std::make_shared;
using std::shared_ptr;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::minutes;
using std::chrono::seconds;

int cal_count = 0;

// �����������
void scene_1()
{
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; ++a)
    {
        for (int b = -11; b < 11; ++b)
        {
            auto choose_mat = random_double();
            Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    // ��0~1ʱ���ڴ�center�˶���center_end�����ϵĵ���Ч��
                    auto center_end = center + Vec3(0, random_double(0, .5), 0);
                    world.add(make_shared<Sphere>(center, center_end, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    // ����
    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));
    // ������
    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));
    // �߹�
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    // BVH���ٽṹ
    auto world_bvh = BVHNode(world);

    Camera cam;

    cam.set_aspect_ratio(16. / 9.);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    cam.set_max_depth(50);

    cam.set_vfov(20);
    cam.set_lookfrom(Point3(13, 2, 3));
    cam.set_lookat(Point3(0, 0, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.6);

    cam.render(world_bvh);
}

// 3D���̸�����
void scene_2()
{
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.8, Color(.2, .3, .1), Color(.9, .9, .9));

    world.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    Camera cam;

    cam.set_aspect_ratio(16. / 9.);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    cam.set_max_depth(50);

    cam.set_vfov(20);
    cam.set_lookfrom(Point3(13, 2, 3));
    cam.set_lookat(Point3(0, 0, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.render(world);
}

// ����
void scene_3()
{
    auto earth_texture = make_shared<ImageTexture>("../texture/earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = Sphere(Point3(0, 0, 0), 2, earth_surface);

    Camera cam;

    cam.set_aspect_ratio(16. / 9.);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    cam.set_max_depth(50);

    cam.set_vfov(20);
    cam.set_lookfrom(Point3(0, 0, 12));
    cam.set_lookat(Point3(0, 0, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.render(globe);
}

// ��������������
void scene_4()
{
    HittableList world;

    auto pertext = make_shared<NoiseTexture>();
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    Camera cam;

    cam.set_aspect_ratio(16. / 9.);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    cam.set_max_depth(50);

    cam.set_vfov(20);
    cam.set_lookfrom(Point3(13, 2, 3));
    cam.set_lookat(Point3(0, 0, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.render(world);
}

// ����ı���
void scene_5()
{
    HittableList world;

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

    Camera cam;

    cam.set_aspect_ratio(1.);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    cam.set_max_depth(50);

    cam.set_vfov(80);
    cam.set_lookfrom(Point3(0, 0, 9));
    cam.set_lookat(Point3(0, 0, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.render(world);
}

// �Է�������
void scene_6()
{
    HittableList world;

    auto perlin_texture = make_shared<NoiseTexture>();
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(perlin_texture)));
    world.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(perlin_texture)));

    auto diff_light = make_shared<DiffuseLight>(Color(4, 4, 4));
    world.add(make_shared<Sphere>(Point3(0, 7, 0), 2, diff_light));
    world.add(make_shared<Quad>(Point3(3, 1, -2), Vec3(2, 0, 0), Vec3(0, 2, 0), diff_light));

    Camera cam;

    cam.set_aspect_ratio(16./9.);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    cam.set_max_depth(50);

    cam.set_vfov(20);
    cam.set_lookfrom(Point3(26, 3, 6));
    cam.set_lookat(Point3(0, 2, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.set_background(Color(0. ,0. ,0.));

    cam.render(world);
}

// Cornell Box 1984
void scene_7()
{
    HittableList world;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    world.add(make_shared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.add(make_shared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.add(make_shared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.add(make_shared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.add(make_shared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    shared_ptr<Hittable> box1 = construct_box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));
    world.add(box1);

    shared_ptr<Hittable> box2 = construct_box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));
    world.add(box2);

    Camera cam;

    cam.set_aspect_ratio(1);
    cam.set_image_width(600);
    cam.set_samples_per_pixel(200);
    cam.set_max_depth(50);

    cam.set_vfov(40);
    cam.set_lookfrom(Point3(278, 278, -800));
    cam.set_lookat(Point3(278, 278, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.set_background(Color(0., 0., 0.));

    cam.render(world);
}

// Cornell Box 1984 ��Ч
void scene_8()
{
    HittableList world;

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

    shared_ptr<Hittable> box1 = construct_box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vec3(265, 0, 295));

    shared_ptr<Hittable> box2 = construct_box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vec3(130, 0, 65));

    world.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    world.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    Camera cam;

    cam.set_aspect_ratio(1);
    cam.set_image_width(600);
    cam.set_samples_per_pixel(200);
    cam.set_max_depth(50);

    cam.set_vfov(40);
    cam.set_lookfrom(Point3(278, 278, -800));
    cam.set_lookat(Point3(278, 278, 0));
    cam.set_vup(Vec3(0, 1, 0));

    cam.set_defocus_angle(0.);

    cam.set_background(Color(0., 0., 0.));

    cam.render(world);
}

// �ܳ���
void scene_9()
{
    HittableList world;
    
    // ��������ɵĵذ�
    HittableList boxes1;
    auto ground_mat = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));
    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; ++i)
    {
        for (int j = 0; j < boxes_per_side; ++j)
        {
            auto w = 100.;
            auto x0 = -1000. + i * w;
            auto z0 = -1000. + j * w;
            auto y0 = 0.;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(construct_box(Point3(x0, y0, z0), Point3(x1, y1, z1), ground_mat));
        }
    }
    world.add(make_shared<BVHNode>(boxes1));

    // ��Դ
    auto light_mat = make_shared<DiffuseLight>(Color(7, 7, 7));
    world.add(make_shared<Quad>(Point3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light_mat));

    // �˶���
    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vec3(30, 0, 0);
    auto sphere_mat = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(make_shared<Sphere>(center1, center2, 50, sphere_mat));

    // ������
    world.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    // ������
    world.add(make_shared<Sphere>(Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    // �ں���ɫ��Ч�Ĳ�����
    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));

    // ȫ������Ч
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    world.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    // ����
    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("../texture/earthmap.jpg"));
    world.add(make_shared<Sphere>(Point3(400, 200, 400), 100, emat));

    // ����������
    auto pertext = make_shared<NoiseTexture>(8);
    world.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    // ����ɵ�������
    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }
    world.add(
        make_shared<Translate>(
        make_shared<RotateY>(make_shared<BVHNode>(boxes2), 15), 
            Vec3(-100, 270, 395))
    );

    Camera cam;

    cam.set_aspect_ratio(1);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(500);
    cam.set_max_depth(40);

    cam.set_vfov(40);
    cam.set_lookfrom(Point3(478, 278, -600));
    cam.set_lookat(Point3(278, 278, 0));
    cam.set_vup(Vec3(0, 1, 0));
    cam.set_background(Color(0., 0., 0.));

    cam.set_defocus_angle(0.);

    cam.render(world);
}

int main() 
{
    auto start = steady_clock::now();

    switch (7)
    {
        case 1: scene_1(); break;
        case 2: scene_2(); break;
        case 3: scene_3(); break;
        case 4: scene_4(); break;
        case 5: scene_5(); break;
        case 6: scene_6(); break;
        case 7: scene_7(); break;
        case 8: scene_8(); break;
        case 9: scene_9(); break;
    }

    auto end = steady_clock::now();

    LOG("�ܼ�������", std::to_string(cal_count));

    auto duration = end - start;
    auto duration_min = duration_cast<minutes>(duration);
    auto duration_sec = duration_cast<seconds>(duration);

    LOG("�ܼ���ʱ����", std::to_string(duration_min.count()), "����");
    LOG("�����ٶȣ�", std::to_string(cal_count / duration_sec.count()), "��/��");

    return 0;
}