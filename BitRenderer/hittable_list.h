/*
* ������ཻ�������б�
*/
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>

#include "hittable.h"

class HittableList : public HitTable
{
public:

    HittableList() = default;
    HittableList(std::shared_ptr<HitTable> object) 
    { 
        add(object);
    }

    void clear() 
    { 
        objects_.clear(); 
    }

    void add(std::shared_ptr<HitTable> object)
    {
        objects_.push_back(object);
    }

    bool hit(const Ray& r, Interval interval, HitRecord& rec) const override
    {
        HitRecord temp_rec;
        bool hit_anything = false;
        auto closest_so_far = interval.get_max();

        for (const auto& object : objects_)
        {
            if (object->hit(r, Interval(interval.get_min(), closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t; // ���µ�ǰ����ʱ��Ĺ���Ͷ�����Ϊ�´ι���Ͷ���������
                rec = temp_rec; // ���»�����Ϣ
            }
        }

        return hit_anything;
    }

private:

    std::vector<std::shared_ptr<HitTable>> objects_;
};

#endif // !HITTABLE_LIST_H
