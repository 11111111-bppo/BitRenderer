/*
* ����ɻ�������
*/
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct HitRecord
{
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;

    // ȷ����������۲���
    // outward_normalΪ������ָ����߻��е㷽��ķ���
    void set_face_normal(const Ray& r, const Vec3& outward_normal) 
    {
        front_face = dot(r.get_direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// ������
class HitTable
{
public:

    virtual ~HitTable() = default;
    virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const = 0;
};

#endif // !HITTABLE_H
