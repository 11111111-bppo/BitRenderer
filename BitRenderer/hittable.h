/*
* ���߿ɻ���������
*/
#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "interval.h"

// ǰ������������ͷ�ļ�ѭ������
class Material;

struct HitRecord
{
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> material;
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
    virtual bool hit(const Ray& r, Interval interval, HitRecord& rec) const = 0;
};

#endif // !HITTABLE_H
