#include "image.h"
#include "sphere.h"
#include "hittable_list.h"

Color ray_color(const Ray& r)
{
	static HittableList hittablelist(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    HitRecord hr;
    if (hittablelist.hit(r, 0.1, 1000, hr)) // ������߻������壬����������е�
    {
        // ���д�����
        Vec3 N = hr.normal;
        // ����������ÿ��ֵ��[-1,1]���ŵ�[0,1]������ɫ��
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
    }

    // ������ɫ
    Vec3 unit_direction = unit_vector(r.get_direction());
    // ��y�����׽���
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

int main() 
{
    // ͼ��
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 512;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    int channel = 3;

    // ���
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
    auto camera_center = Point3(0, 0, 0);
    // �ӿ�����ԭ��
    // �����Ͻ�Ϊԭ�㣬u��x�����ᣬv��y�Ḻ��
    auto viewport_u = Vec3(viewport_width, 0, 0);
    auto viewport_v = Vec3(0, -viewport_height, 0);
    // ÿ���ض�Ӧ���ӿڳ���
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;
    // ���Ͻ����ص�λ�ã�����λ�������ĵ��ʾ
    auto viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    std::unique_ptr<Image> image(new Image("image.png", image_width, image_height, channel));

    for (int i = 0; i < image_height; ++i) 
    {
        std::clog << "\rScanlines remaining: " << (image_height - i) << ' ' << std::flush;
        for (int j = 0; j < image_width; ++j) 
        {
            // ������λ��
            auto pixel_center = pixel00_loc + (i * pixel_delta_v) + (j * pixel_delta_u);
            auto ray_direction = pixel_center - camera_center;
            // �õ�����������Ĺ���
            Ray r(camera_center, ray_direction);

            image->set_pixel(i, j, ray_color(r));
        }
    }
    std::clog << "\rDone.                 \n";

    image->write();
    return 0;
}