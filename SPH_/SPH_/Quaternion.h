#ifndef QUATERNION_H
#define	QUATERNION_H

#include <math.h>
#include "vec3f.h"
#include "include\glm\gtc\matrix_transform.hpp"

class Quaternion{
public: 
	Quaternion(float angle, Vec3f* axis);
	Quaternion::Quaternion(float angle, glm::vec3* axis);
	Quaternion();
	~Quaternion();
	void normalise();
	void convertToMatrix(float matrix[]);
	void rotateVector(float matrix[], Vec3f* torotate, float vec[3]);
	void rotateVector(float matrix[], glm::vec3* torotate, float vec[3]);
	Vec3f* rotatePoint(float matrix[], Vec3f* torotate);
	float w;
	float x,y,z;
	void printMatrix(float matrix[]);
	void printQuat();
	void combineRotations(float matrixY[16], float matrixP[16], float newMat[16]);

};

#endif