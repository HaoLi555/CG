#ifndef PLANE_H
#define PLANE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>

// TODO: Implement Plane representing an infinite plane
// function: ax+by+cz=d
// choose your representation , add more fields and fill in the functions

class Plane : public Object3D
{
public:
    Plane()
    {
    }

    Plane(const Vector3f &normal, float d, Material *m) : Object3D(m)
    {
        this->normal = normal;
        this->d = -d;
    }

    ~Plane() override = default;

    bool intersect(const Ray &r, Hit &h, float tmin) override
    {
        Vector3f norm_dir=r.getDirection().normalized();
        float t = -(this->d + Vector3f::dot(this->normal, r.getOrigin())) / Vector3f::dot(this->normal, norm_dir);
        t/=r.getDirection().length();
        if (t > 0 && t > tmin && t < h.getT())
        {
            h.set(t, this->material, Vector3f::dot(this->normal, norm_dir) < 0 ? this->normal.normalized() : -this->normal.normalized());
            return true;
        }
        return false;
    }

protected:
    // nP+d=0
    Vector3f normal;
    float d;
};

#endif // PLANE_H
