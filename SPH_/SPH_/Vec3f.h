

#ifndef VEC3F_H
#define VEC3F_H

#include <iostream>
#include <string>


class Vec3f {
		
	public:
		Vec3f();
		Vec3f(float x, float y, float z);
		float x; float y; float z;
		
		Vec3f operator*(float scale) const;
		Vec3f operator*(const Vec3f &other) const;
		Vec3f operator/(float scale) const;
		Vec3f operator+(const Vec3f &other) const;
		Vec3f operator-(const Vec3f &other) const;
		Vec3f operator-() const;
		
		const Vec3f &operator*=(float scale);
		const Vec3f &operator/=(float scale);
		const Vec3f &operator+=(const Vec3f &other);
		const Vec3f &operator-=(const Vec3f &other);
		
		float magnitude() const;
		float magnitudeSquared() const;
		Vec3f normalize() const;
		Vec3f getNormal(float edgePoint0X, float edgePoint0Z, float edgePoint1X, float edgePoint1Z);
		float dot(const Vec3f &other) const;
		Vec3f cross(const Vec3f &other) const;
		Vec3f crossProd(Vec3f &other);
		float angleBetween(Vec3f &other);
		void printVec();
		float getComponentX();
		float getComponentY();
		float getComponentZ();
		void setComponentX(float x);
		void setComponentY(float y);
		void setComponentZ(float z);
		void setComponentXYZ(float x, float y, float z);

};



#endif
