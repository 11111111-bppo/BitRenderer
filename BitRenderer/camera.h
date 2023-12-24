/*
* ���
*/
#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "image.h"

class Camera
{
public:

    void render(const HitTable& world)
    {
        initialize();

        for (int i = 0; i < image_height_; ++i)
        {
            std::clog << "\rScanlines remaining: " << (image_height_ - i) << ' ' << std::flush;
            for (int j = 0; j < image_width_; ++j)
            {
                // ������λ��
                auto pixel_center = pixel00_loc_ + (i * pixel_delta_v_) + (j * pixel_delta_u_);
                auto ray_direction = pixel_center - camera_center_;
                // �õ�����������Ĺ���
                Ray r(camera_center_, ray_direction);

                Color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel_; ++sample)
                {
                    Ray r = get_ray(i, j);
                    pixel_color += ray_color(r, world);
                }
                image_->set_pixel(i, j, pixel_color, samples_per_pixel_);
            }
        }
        std::clog << "\rDone.                 \n";
        image_->write();
    }

    void set_image_width(int image_width)
    {
        image_width_ = image_width;
    }

    void set_aspect_ratio(double aspect_ratio)
    {
        aspect_ratio_ = aspect_ratio;
    }

private:

    double aspect_ratio_ = 1.;
    int    image_width_ = 100;
    int channel_ = 3;
    int    image_height_;
    Point3 camera_center_;
    Point3 pixel00_loc_; // (0,0)�����ص�λ��
    Vec3   pixel_delta_u_;
    Vec3   pixel_delta_v_;
    std::unique_ptr<Image> image_;
    int    samples_per_pixel_ = 10;

    // ��ʼ��
    void initialize() 
    {
        image_height_ = static_cast<int>(image_width_ / aspect_ratio_);
        image_height_ = (image_height_ < 1) ? 1 : image_height_;

        image_ = std::make_unique<Image>("image.png", image_width_, image_height_, channel_);

        camera_center_ = Point3(0, 0, 0);

        // �������
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width_) / image_height_);
        // �����Ͻ�Ϊԭ�㣬u��x�����ᣬv��y�Ḻ��
        auto viewport_u = Vec3(viewport_width, 0, 0);
        auto viewport_v = Vec3(0, -viewport_height, 0);
        // ÿ���ض�Ӧ���ӿڳ���
        pixel_delta_u_ = viewport_u / image_width_;
        pixel_delta_v_ = viewport_v / image_height_;
        // ���Ͻ����ص�λ�ã�����λ�������ĵ��ʾ
        auto viewport_upper_left = camera_center_ - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);
    }

    // ��ȡ���߻��д�����ɫ
    Color ray_color(const Ray& r, const HitTable& world) const
    {
        HitRecord rec;

        if (world.hit(r, Interval(0, kInfinitDouble), rec)) 
        {
            // ����������ÿ��ֵ��[-1,1]���ŵ�[0,1]������ɫ��
            return 0.5 * (rec.normal + Color(1, 1, 1));
        }

        // ������ɫ
        Vec3 unit_direction = unit_vector(r.get_direction());
        // ��y�����׽���
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

    // �����������
    Ray get_ray(int i, int j) const 
    {
        // ���س���Ϊ1�����ؿ���һ���������λ��
        auto pixel_sample_square = [=]() -> Vec3
        {
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u_) + (py * pixel_delta_v_);
        };

        // ��(i,j)�������������һ������
        auto pixel_center = pixel00_loc_ + (j * pixel_delta_u_) + (i * pixel_delta_v_);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = camera_center_;
        auto ray_direction = pixel_sample - ray_origin;

        return Ray(ray_origin, ray_direction);
    }

};

#endif // !CAMERA_H