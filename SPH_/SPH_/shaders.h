#pragma
#ifndef SHADERS_H
#define SHADERS_H
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "include\GL\glew.h"
#include "include\GLFW\glfw3.h"
#include "include\glm\glm.hpp"
#include "Particle.h"
#include "Camera.h"

using namespace std;

class shaders{
public:
	GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path);
	//void UseShader(GLuint, Particle, glm::mat4, Camera*, Vec3f, Vec3f, float, float);

	//void UseShaderInstanced(GLuint, Camera*, GLuint, vector<glm::vec4>*, vector<glm::mat4>*, Vec3f, Vec3f, float, float);
	void UseShaderInstanced(GLuint, Camera*, GLuint, vector<glm::vec4>*, vector<glm::mat4>*, Vec3f, Vec3f, float, float, std::vector<Particle>*);

	void UseShader(GLuint, glm::mat4, Camera*, Vec3f, Vec3f, float, float);
	shaders();
};
#endif