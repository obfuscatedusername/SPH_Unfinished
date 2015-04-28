
#include <windows.h>
#include "Quaternion.h"
#include <iostream>


static const float rads = (3.141592654 / 180);

Quaternion::Quaternion(){
	w = 0.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Quaternion::Quaternion(float angle, Vec3f* axis){

	//q = ( cos(angle/2), n sin(angle/2) )
	float angleinradians = angle * rads;
	float a = angleinradians/2.0f;
	Vec3f n = axis->normalize();

	w = cos(a);
	x =  n.x*sin(a);
	y =  n.y*sin(a);
	z =  n.z*sin(a);
}

Quaternion::Quaternion(float angle, glm::vec3* axis){

	//q = ( cos(angle/2), n sin(angle/2) )
	float angleinradians = angle * rads;
	float a = angleinradians/2.0f;
	glm::vec3 n = glm::normalize(*axis);

	w = cos(a);
	x =  n.x*sin(a);
	y =  n.y*sin(a);
	z =  n.z*sin(a);
}

Quaternion::~Quaternion(){

}

void Quaternion::normalise(){

	 const float n = 1.0f/sqrt(w*w + x*x + y*y + z*z);
     
	 w*=n;
     x*=n;
     y*=n;
     z*=n;    
}

void Quaternion::convertToMatrix(float matrix[]){
//	      |       2     2                                    |
//        | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW      0  |
//        |                                                  |
//        |                       2     2                    |
//    M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW      0  |
//        |                                                  |
//        |                                      2     2     |
//        | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y   0  |
//        |                                                  |
//        |    0               0             0            1  |

	//float matrix[16];
	float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float xw = x * w;
    float yy = y * y;
    float yz = y * z;
    float yw = y * w;
    float zz = z * z;
    float zw = z * w;

    matrix[0]  = 1.0f - 2.0f * ( yy + zz ); //1.0f - 2.0f*yy - 2.0f*zz 
    matrix[1]  = 2.0f * ( xy + zw ); //2.0f*xy - 2.0f*zw 
    matrix[2]  = 2.0f * ( xz - yw ); //2.0f*xz + 2.0f*yw

    matrix[4]  = 2.0f * ( xy - zw ); //2.0f*xy + 2.0f*zw
    matrix[5]  = 1.0f - 2.0f * ( xx + zz ); //1.0f - 2.0f*xx - 2.0f*zz 
    matrix[6]  = 2.0f * ( yz + xw ); //2.0f*yz - 2.0f*xw

    matrix[8]  = 2.0f * ( xz + yw ); //2.0f*xz - 2.0f*yw
    matrix[9]  = 2.0f * ( yz - xw ); //2.0f*yz + 2.0f*xw, 
    matrix[10] = 1.0f - 2.0f * ( xx + yy ); //1.0f - 2.0f*xx - 2.0f*yy

	//add 4th row and column as identity values
    matrix[3]  = matrix[7] = matrix[11] = matrix[12] = matrix[13] = matrix[14] = 0;
    matrix[15] = 1;
}

void Quaternion::combineRotations(float matrixY[16], float matrixP[16], float newMat[16]){
	    newMat[0] = matrixY[0]*matrixP[0] + matrixY[4]*matrixP[1] + matrixY[8]*matrixP[2];
		newMat[1] = matrixY[1]*matrixP[0] + matrixY[5]*matrixP[1] + matrixY[9]*matrixP[2];
		newMat[2] = matrixY[2]*matrixP[0] + matrixY[6]*matrixP[1] + matrixY[10]*matrixP[2];
		newMat[3] = 0;

		newMat[4] = matrixY[0]*matrixP[4] + matrixY[4]*matrixP[5] + matrixY[8]*matrixP[6];
		newMat[5] = matrixY[1]*matrixP[4] + matrixY[5]*matrixP[5] + matrixY[9]*matrixP[6];
		newMat[6] = matrixY[2]*matrixP[4] + matrixY[6]*matrixP[5] + matrixY[10]*matrixP[6];
		newMat[7] = 0;

		newMat[8] = matrixY[0]*matrixP[8] + matrixY[4]*matrixP[9] + matrixY[8]*matrixP[10];
		newMat[9] = matrixY[1]*matrixP[8] + matrixY[5]*matrixP[9] + matrixY[9]*matrixP[10];
		newMat[10] = matrixY[2]*matrixP[8] + matrixY[6]*matrixP[9] + matrixY[10]*matrixP[10];
		newMat[11] = 0;

		newMat[12] = matrixY[0]*matrixP[12] + matrixY[4]*matrixP[13] + matrixY[8]*matrixP[14] + matrixY[12];
		newMat[13] = matrixY[1]*matrixP[12] + matrixY[5]*matrixP[13] + matrixY[9]*matrixP[14] + matrixY[13];
		newMat[14] = matrixY[2]*matrixP[12] + matrixY[6]*matrixP[13] + matrixY[10]*matrixP[14] + matrixY[14];
		newMat[15] = 1;
}

void Quaternion::rotateVector(float matrix[], Vec3f* torotate, float vec[3]){
		// in maths:
		//  x   y   z   w     
		// |a   b   c   d|    |x|   |ax + by + cz + dw| so new_x = (ax + by + cz + dw)
		// |e   f   g   h|  * |y| = |ex + fy + gz + hw|=y
		// |i   j   k   l|    |z|   |ix + jy + kz + lw|=z
		// |m   n   o   p|    |w|   |mx + ny + oz + pw|=w
		//
		//in opengl (and implemented here):
		//     
		// |a   e   i   m|x    |x|   |ax + by + cz + dw|=x
		// |b   f   j   n|y  * |y| = |ex + fy + gz + hw|=y
		// |c   g   k   o|z    |z|   |ix + jy + kz + lw|=z
		// |d   h   l   p|x    |w|   |mx + ny + oz + pw|=w

		Vec3f result;
		vec[0] = matrix[0] * torotate->x + matrix[4] * torotate->y + matrix[8]  * torotate->z;
		vec[1] = matrix[1] * torotate->x + matrix[5] * torotate->y + matrix[9]  * torotate->z;
		vec[2] = matrix[2] * torotate->x + matrix[6] * torotate->y + matrix[10] * torotate->z;

	
}

void Quaternion::rotateVector(float matrix[], glm::vec3* torotate, float vec[3]){

		Vec3f result;
		vec[0] = matrix[0] * torotate->x + matrix[4] * torotate->y + matrix[8]  * torotate->z;
		vec[1] = matrix[1] * torotate->x + matrix[5] * torotate->y + matrix[9]  * torotate->z;
		vec[2] = matrix[2] * torotate->x + matrix[6] * torotate->y + matrix[10] * torotate->z;

	
}

Vec3f* Quaternion::rotatePoint(float matrix[], Vec3f* torotate){

		Vec3f result;

		result.x = matrix[0] * torotate->x + matrix[4] * torotate->y + matrix[8]  * torotate->z + matrix[12];
		result.y = matrix[1] * torotate->x + matrix[5] * torotate->y + matrix[9]  * torotate->z + matrix[13];
		result.z = matrix[2] * torotate->x + matrix[6] * torotate->y + matrix[10] * torotate->z + matrix[14];
		return &result;
}

void Quaternion::printMatrix(float m[]){
	printf("%.1f", m[0]); printf("  "); printf("%.1f", m[1]); printf("  "); printf("%.1f", m[2]); printf("  "); printf("%.1f", m[3]); 
	printf("\n");
	printf("%.1f", m[4]); printf("  "); printf("%.1f", m[5]); printf("  "); printf("%.1f", m[6]); printf("  "); printf("%.1f", m[7]); 
	printf("\n");
	printf("%.1f", m[8]); printf("  "); printf("%.1f", m[9]); printf("  "); printf("%.1f", m[10]); printf("  "); printf("%.1f", m[11]); 
	printf("\n");
	printf("%.1f", m[12]); printf("  "); printf("%.1f", m[13]); printf("  "); printf("%.1f", m[14]); printf("  "); printf("%.1f", m[15]);
	printf("\n");
	printf("\n");
}

void Quaternion::printQuat(){
	printf("x: "); printf("%.1f",x); printf("\n");
	printf("y: "); printf("%.1f",y); printf("\n");
	printf("z: "); printf("%.1f",z); printf("\n");
	printf("w: "); printf("%.1f",w); printf("\n");printf("\n");
}