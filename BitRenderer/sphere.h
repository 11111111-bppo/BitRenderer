/*
* 球类
*/
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public HitTable
{
public:

    // 静态球
    Sphere(Point3 center, double radius, std::shared_ptr<Material> material) 
        : center_(center), radius_(radius), material_(material), is_moving_(false) {}

    // 运动球
    Sphere(Point3 center, Point3 center_end, double radius, std::shared_ptr<Material> material)
        : center_(center), radius_(radius), material_(material), is_moving_(true)
    {
        center_move_vec_ = center_end - center;
    }

    bool hit(const Ray& r, Interval interval, HitRecord& rec) const override
    {
        Point3 now_center = is_moving_ ? get_center(r.get_time()) : center_;
        Vec3 oc = r.get_origin() - now_center;
        auto a = r.get_direction().length_squared();
        auto half_b = dot(oc, r.get_direction());
        auto c = oc.length_squared() - radius_ * radius_;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) 
            return false;
        auto sqrtd = std::sqrt(discriminant);

        auto root = (-half_b - sqrtd) / a; // 更近的根
        if (root <= interval.get_min() || interval.get_max() <= root)
        {
            root = (-half_b + sqrtd) / a; // 更远的根
            if (root <= interval.get_min() || interval.get_max() <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center_) / radius_; // 单位化
        rec.set_face_normal(r, outward_normal);
        rec.material = material_;

        return true;
    }

private:

    Point3 center_;
    Vec3 center_move_vec_; // 球移动距离
    bool is_moving_; //球是否在移动

    double radius_;
    std::shared_ptr<Material> material_;

    // 获取t时刻的球心位置
    Point3 get_center(double t) const
    {
        return center_ + t * center_move_vec_;
    }

};

#endif // !SPHERE_H
