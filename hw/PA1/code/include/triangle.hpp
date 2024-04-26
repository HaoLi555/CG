#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "object3d.hpp"
#include <vecmath.h>
#include <cmath>
#include <iostream>
using namespace std;

// TODO: implement this class and add more fields as necessary,
class Triangle : public Object3D
{

public:
	Triangle() = delete;

	// a b c are three vertex positions of the triangle
	Triangle(const Vector3f &a, const Vector3f &b, const Vector3f &c, Material *m) : Object3D(m)
	{
		vertices[0] = a;
		vertices[1] = b;
		vertices[2] = c;
		this->material = m;

		Vector3f edge1 = b - a;
		Vector3f edge2 = c - a;
		normal = Vector3f::cross(edge1, edge2).normalized();
	}

	bool intersect(const Ray &ray, Hit &hit, float tmin) override
	{
		// Vector3f norm_dir=ray.getDirection().normalized();

		Vector3f edge1 = this->vertices[0] - this->vertices[1];
		Vector3f edge2 = this->vertices[0] - this->vertices[2];
		Vector3f s = this->vertices[0] - ray.getOrigin();

		Matrix3f A(s, edge1, edge2, true);
		Matrix3f B(ray.getDirection(), s, edge2, true);
		Matrix3f C(ray.getDirection(), edge1, s, true);
		Matrix3f D(ray.getDirection(), edge1, edge2, true);
		float denominator = D.determinant();

		float t = A.determinant() / denominator;
		// t/=ray.getDirection().length();
		
		float beta = B.determinant() / denominator;
		float gamma = C.determinant() / denominator;
		if (t > 0 && beta > 0 && gamma > 0 && beta + gamma < 1 && t < hit.getT() && t > tmin)
		{
			// std::cout<<"hit triangle"<<std::endl;
			hit.set(t, this->material, Vector3f::dot(this->normal, ray.getDirection()) < 0 ? this->normal : -this->normal);
			return true;
		}
		// std::cout<<"not hit triangle"<<std::endl;
		// std::cout<<t<<" "<<beta<<" "<<gamma<<std::endl;
		return false;
	}
	Vector3f normal;
	Vector3f vertices[3];

protected:
};

#endif // TRIANGLE_H
