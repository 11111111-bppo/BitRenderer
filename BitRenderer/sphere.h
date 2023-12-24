/*
* ��
*/
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class Sphere : public HitTable
{
public:

    Sphere(Point3 _center, double _radius) : center_(_center), radius_(_radius) {}

    bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override 
    {
        Vec3 oc = r.get_origin() - center_;
        auto a = r.get_direction().length_squared();
        auto half_b = dot(oc, r.get_direction());
        auto c = oc.length_squared() - radius_ * radius_;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        auto root = (-half_b - sqrtd) / a; // �����ĸ�
        if (root <= ray_tmin || ray_tmax <= root)
        {
            root = (-half_b + sqrtd) / a; // ��Զ�ĸ�
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        Vec3 outward_normal = (rec.p - center_) / radius_; // ��λ��
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:

    Point3 center_;
    double radius_;
};

#endif // !SPHERE_H
