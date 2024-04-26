#ifndef GROUP_H
#define GROUP_H


#include "object3d.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include <iostream>
#include <vector>


// TODO: Implement Group - add data structure to store a list of Object*
class Group : public Object3D {

public:

    Group() {

    }

    explicit Group (int num_objects) {
        objects.reserve(num_objects);
    }

    ~Group() override {

    }

    bool intersect(const Ray &r, Hit &h, float tmin) override {
        bool flag = false;
        for (auto iter=objects.begin(); iter!=objects.end(); iter++) {
            if ((*iter)->intersect(r, h, tmin)) {
                flag = true;
            }
        }
        return flag;
    }

    void addObject(int index, Object3D *obj) {
        assert(index<=objects.size());
        objects.insert(objects.begin()+index, obj);
    }

    int getGroupSize() {
        return objects.size();
    }

private:
    std::vector<Object3D *> objects;
};

#endif
	
