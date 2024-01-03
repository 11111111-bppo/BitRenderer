/*
* �����
*/
#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <iomanip>

#include "color.h"
#include "hittable.h"
#include "image.h"
#include "material.h"
#include "pdf.h"
#include "logger.h"

extern int cal_count;

class Camera
{
public:

    void render(const std::shared_ptr<Hittable>& world, const std::shared_ptr<Hittable>& light = nullptr)
    {
        initialize();

        sqrt_spp_ = (int)std::sqrt(samples_per_pixel_);

        for (int i = 0; i < height_; ++i)
        {
            std::clog << std::fixed << std::setprecision(2);
            std::clog << "\rtask remaining: " << (double)(height_ - i - 1) / height_ * 100 << "%" << std::flush;

#pragma omp parallel for

            for (int j = 0; j < width_; ++j)
            {
                Color pixel_color(0, 0, 0);
                // ��ÿ�������еĲ�������зֲ㣬����������
                for (int s_i = 0; s_i < sqrt_spp_; ++s_i)
                {
                    for (int s_j = 0; s_j < sqrt_spp_; ++s_j)
                    {
                        Ray r = get_ray(i, j, s_i, s_j);
                        pixel_color += ray_color(r, world, light, max_depth_);
                    }
                }
                image_->set_pixel(i, j, pixel_color, samples_per_pixel_);
            }
        }
        std::clog << "\rDone.                 \n";
        image_->write();
    }

    void set_image_width(const int& image_width)
    {
        width_ = image_width;
    }

    void set_aspect_ratio(const double& aspect_ratio)
    {
        aspect_ratio_ = aspect_ratio;
    }

    void set_samples_per_pixel(const int& samples_per_pixel)
    {
        samples_per_pixel_ = samples_per_pixel;
    }

    void set_max_depth(const int& max_depth)
    {
        max_depth_ = max_depth;
    }

    void set_vfov(const double& vfov)
    {
        vfov_ = vfov;
    }

    void set_lookfrom(const Vec3& lookfrom)
    {
        lookfrom_ = lookfrom;
    }

    void set_lookat(const Vec3& lookat)
    {
        lookat_ = lookat;
    }

    void set_vup(const Vec3& vup)
    {
        vup_ = vup;
    }

    void set_defocus_angle(const double& defocus_angle)
    {
        defocus_angle_ = defocus_angle;
    }

    void set_focus_dist(const double& focus_dist)
    {
        focus_dist_ = focus_dist;
    }

    void set_background(const Color& background)
    {
        background_ = background;
    }

    void set_image_name(const std::string& image_name)
    {
        image_name_ = image_name;
    }

private:

    // ��ʹ��ImageWrite image_���ᱨ�� 0xc0000005 ���ʳ�ͻ��
    // ��ʼ��image_ʹ�õ���ʱImageWrite����ᱻ��������������е�image_data_ָ���������������ͷţ�
    // image_��image_data_ָ���Ǵ���ʱImageWrite����ǳ������������Ϊ����ָ�룬�ʷ��ʳ�ͻ��
    std::unique_ptr<ImageWrite> image_;
    std::string image_name_;

    double aspect_ratio_ = 1.;
    int    width_ = 100;
    int    channel_ = 3;
    int    height_;
    Point3 camera_center_;
    Point3 pixel00_loc_; // (0,0)�����ص�λ��
    Vec3   pixel_delta_u_;
    Vec3   pixel_delta_v_;
    int    samples_per_pixel_ = 16; // ÿ���ز�����
    int    sqrt_spp_ = 4;
    int    max_depth_ = 10; // ������������

    double vfov_ = 90;  // ��ֱfov
    
    Point3 lookfrom_ = Point3(0, 0, -1);
    Point3 lookat_ = Point3(0, 0, 0);
    Vec3   vup_ = Point3(0, 1, 0);
    // �������ϵ
    Vec3   u_; // ָ������ҷ�
    Vec3   v_; // ָ������Ϸ�
    Vec3   w_; // ������ӵ㷽���෴ 

    double defocus_angle_ = 0;  // ���߾���ÿ�����صı仯
    double focus_dist_ = 10;    // ���ԭ�㵽�����۽�ƽ��ľ��룬�����뽹����ͬ
    Vec3 defocus_disk_u_;  // ɢ������뾶
    Vec3 defocus_disk_v_;  // ɢ������뾶

    Color background_ = Color(.7, .8, 1.);

    // ��ʼ��
    void initialize() 
    {
        height_ = static_cast<int>(width_ / aspect_ratio_);
        height_ = (height_ < 1) ? 1 : height_;

        image_ = std::make_unique<ImageWrite>(image_name_, width_, height_, channel_);
        
        camera_center_ = lookfrom_;

        // �������
        auto theta = degrees_to_radians(vfov_);
        auto h = tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist_;

        auto viewport_width = viewport_height * (static_cast<double>(width_) / height_);
        
        // �����������ϵ������ϵ(z��ָ����Ļ��)
        w_ = unit_vector(lookfrom_ - lookat_); // ������ӵ㷽���෴ (0,0,-1)
        u_ = unit_vector(cross(vup_, w_)); // ָ������ҷ� (-1,0,0)
        v_ = cross(w_, u_); // ָ������Ϸ� (0,1,0)

        Vec3 viewport_u = viewport_width * u_;
        Vec3 viewport_v = viewport_height * -v_;

        // ÿ���ض�Ӧ���ӿڳ���
        pixel_delta_u_ = viewport_u / width_;
        pixel_delta_v_ = viewport_v / height_;
        // ���Ͻ����ص�λ�ã�����λ�������ĵ��ʾ
        auto viewport_upper_left = camera_center_ - (focus_dist_ * w_) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc_ = viewport_upper_left + 0.5 * (pixel_delta_u_ + pixel_delta_v_);

        // ���ɢ��ƽ��Ļ�����
        auto defocus_radius = focus_dist_ * tan(degrees_to_radians(defocus_angle_ / 2));
        defocus_disk_u_ = u_ * defocus_radius;
        defocus_disk_v_ = v_ * defocus_radius;
    }

    // ��ȡ���߻��д�����ɫ
    Color ray_color(const Ray& r, const std::shared_ptr<Hittable>& world, const std::shared_ptr<Hittable>& light, int depth) const
    {
        // ���ﵯ��������ޣ������ۼ��κ���ɫ
        if (depth < 0)
        {
            return Color(0, 0, 0);
        }

        HitRecord rec;

        // Interval��Сֵ����Ϊ0��������ֵ���¹��������彻���������ڲ�ʱ�������޷���������
        if (!world->hit(r, Interval(1e-3, kInfinitDouble), rec))
            return background_;

        ++cal_count;

        ScatterRecord srec;

        Color color_from_emission = rec.material->emitted(rec.u, rec.v, rec.p); // �Է�����ɫ

        // ֻ���Է�����ɫ
        if (!rec.material->scatter(r, rec, srec))
            return color_from_emission;

        if (srec.skip_pdf)
        {
            return srec.attenuation * ray_color(srec.skip_pdf_ray, world, light, depth - 1);
        }

        Ray scattered;
        double pdf_val;
        // ���޹�Դ����
        if (light != nullptr)
        {
            auto light_ptr = std::make_shared<HittablePDF>(*light, rec.p);
            MixturePDF p(light_ptr, srec.pdf_ptr);

            scattered = Ray(rec.p, p.generate(), r.get_time());
            pdf_val = p.value(scattered.get_direction());
        }
        else
        {
            scattered = Ray(rec.p, srec.pdf_ptr->generate(), r.get_time());
            pdf_val = srec.pdf_ptr->value(scattered.get_direction());
        }

        double scattering_pdf = rec.material->scattering_pdf(r, rec, scattered);

        Color sample_color = ray_color(scattered, world, light, depth - 1);
        Color color_from_scatter = (srec.attenuation * scattering_pdf * sample_color) / pdf_val;

        return color_from_scatter + color_from_emission;
    }

    // �����������
    Ray get_ray(int i, int j, int s_i, int s_j) const
    {
        // ���س���Ϊ1�����ؿ���һ���������λ��
        auto pixel_sample_square = [=](int s_i, int s_j) -> Vec3
        {
            auto px = -0.5 + 1. / sqrt_spp_ * (s_i + random_double());
            auto py = -0.5 + 1. / sqrt_spp_ * (s_j + random_double());
            return (px * pixel_delta_u_) + (py * pixel_delta_v_);
        };

        // ��(i,j)�������������һ������
        auto pixel_center = pixel00_loc_ + (j * pixel_delta_u_) + (i * pixel_delta_v_);
        auto pixel_sample = pixel_center + pixel_sample_square(s_i, s_j);

        // auto ray_origin = camera_center_;
        // ɢ������Բ��͸�����������������ԭ�㲻�������ԭ��
        auto ray_origin = (defocus_angle_ <= 0) ? camera_center_ : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        // ��[0,1)ʱ����֮��������ɹ���
        auto ray_time = random_double();

        return Ray(ray_origin, ray_direction, ray_time);
    }

    // ����Բ��͸�������һ��
    Point3 defocus_disk_sample() const 
    {
        auto p = random_in_unit_disk();
        return camera_center_ + (p[0] * defocus_disk_u_) + (p[1] * defocus_disk_v_);
    }

};

#endif // !CAMERA_H