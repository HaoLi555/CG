#ifndef SPHERE_H
#define SPHERE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement functions and add more fields as necessary

class Sphere : public Object3D
{
public:
    Sphere()
    {
        // unit ball at the center
    }

    Sphere(const Vector3f &center, float radius, Material *material) : Object3D(material)
    {
        //
        this->center = center;
        this->radius = radius;
        this->material = material;
    }

    ~Sphere() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        // 注意到输入的方向不一定是单位向量
        // 因此先归一化后再对t进行一个倍乘
        Vector3f norm_dir=r.getDirection().normalized();

        Vector3f l = center-r.getOrigin();
        // NOTE: 根据测例而言，只考虑射线起点在球外的情况
        assert(l.length() > this->radius);
        float tp = Vector3f::dot(l, norm_dir);
        float d = sqrt(l.length() * l.length() - tp * tp);
        if (d > this->radius)
            return false;
        else
        { 
            float t = tp - sqrt(pow(radius, 2) - pow(d, 2));
            t /= r.getDirection().length();
            if (t < h.getT() && t > tmin)
            {
                // std::cout<<"sphere intersected"<<std::endl;
                h.set(t, this->material, (r.pointAtParameter(t) - center).normalized());
                return true;
            }
        }
        return false;
    }

protected:
    Vector3f center;
    float radius;
};

#endif
