

#include <math.h>

#include "vec3f.h"

using namespace std;

Vec3f::Vec3f() {
	x=0;
	y=0;
	z=0;
}

Vec3f::Vec3f(float vx, float vy, float vz) { //a vector of 3 floating point values
	x = vx;
	y = vy;
	z = vz;
}

float Vec3f::getComponentX(){
	return x;
}
float Vec3f::getComponentY(){
	return y;
}
float Vec3f::getComponentZ(){
	return z;
}
void Vec3f::setComponentX(float vx){
	x=vx;
}
void Vec3f::setComponentY(float vy){
	y=vy;
}
void Vec3f::setComponentZ(float vz){
	z=vz;
}

void Vec3f::setComponentXYZ(float vx, float vy, float vz){
	x=vx;
	y=vy;
	z=vz;
}


Vec3f Vec3f::operator*(float scale) const {
	return Vec3f(x * scale, y * scale, z * scale);
}

Vec3f Vec3f::operator*(const Vec3f &other) const
		{
			return Vec3f(x*other.x, y*other.y, z*other.z);
		}

Vec3f Vec3f::operator/(float scale) const {
	return Vec3f(x / scale, y / scale, z / scale);
}

Vec3f Vec3f::operator+(const Vec3f &other) const {
	return Vec3f(x + other.x, y + other.y, z + other.z);
}

Vec3f Vec3f::operator-(const Vec3f &other) const {
	return Vec3f(x - other.x, y - other.y, z - other.z);
}

Vec3f Vec3f::operator-() const {
	return Vec3f(-x, -y, -z);
}

const Vec3f &Vec3f::operator*=(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

const Vec3f &Vec3f::operator/=(float scale) {
	x /= scale;
	y /= scale;
	z /= scale;
	return *this;
}

const Vec3f &Vec3f::operator+=(const Vec3f &other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

const Vec3f &Vec3f::operator-=(const Vec3f &other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

float Vec3f::magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

float Vec3f::magnitudeSquared() const {
	return x * x + y * y + z * z;
}

float Vec3f::angleBetween(Vec3f &other){
	float dot=x * other.x + y * other.y + z * other.z;
	float lengths=((sqrt(x * x + y * y + z * z))*(sqrt(other.x * other.x + other.y* other.y + other.z * other.z)));
	float cosangle=dot/lengths;
	float ab=acos(cosangle);
	return ab;
}

//cosine of the dotprod/length*length
Vec3f Vec3f::normalize() const {
	float m = sqrt(x * x + y * y + z * z);
	return Vec3f(x/ m, y/ m, z / m);
}

Vec3f Vec3f::getNormal(float edgePoint0X, float edgePoint0Z, float edgePoint1X, float edgePoint1Z)
{
	//dx=x2-x1 and dy=y2-y1, then the normals are (-dy, dx)
	//new co-ordninates worked out by getting the normal -> y1-y, -x1,-x
	float nX=(edgePoint1Z-edgePoint0Z);
	float nZ=-(edgePoint1X-edgePoint0X);
	return Vec3f(nX, 0, nZ );
}

float Vec3f::dot(const Vec3f &other) const {
	return x * other.x + y * other.y + z * other.z;
}


Vec3f Vec3f::crossProd(Vec3f &other){
	return Vec3f(x * other.z - z * other.y,
				 y * other.x - x * other.z,
				 z * other.y - y * other.x);
}

Vec3f operator*(float scale, const Vec3f &v) {
	return v * scale;
}



void Vec3f::printVec(){

	printf("(");
	printf("%.1f",this->x); printf(", ");
	printf("%.1f",this->y); printf(", ");
	printf("%.1f",this->z);
	printf(")");
	printf("\n");

}









