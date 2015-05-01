#ifndef Container_h
#define Container_h
#include "Vec3f.h"
#include "include\GL\glew.h"
#include "include\glm\glm.hpp"
#include <GLFW\glfw3.h>
#include <vector>

#define HEIGHT 0.5
#define WIDTH 0.5

struct BB{
	Vec3f min, max;
};


class Container{
private:
	BB containerBB;
	glm::mat4 Model;
public:
	void genVAO();
	void Render();
	Container();
	~Container();
	glm::mat4 GetModel(){return Model;}
	BB getDims(){return containerBB;}
	Vec3f cont[24];
	Vec3f contNormals[24];
	GLuint VBO, VAO, CBO, VNO;
};
#endif