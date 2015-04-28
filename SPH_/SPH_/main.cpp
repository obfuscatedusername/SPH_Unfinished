#include<stdio.h>
#include <stdlib.h>
#include <GL\glew.h>
#define GLM_FORCE_RADIANS
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>
#include <time.h>
#include <assert.h>
#include <Windows.h>
#include <IL\il.h>

//User generated includes
#include "shaders.h"
#include "modelLoader.h"
#include "Container.h"
#include "ParticleSystem.h"
#include "Camera.h"
#include "Light.h"

using namespace std;

/*--------------------------------------------------------------------------------------

	- Main program below this line, ya know

---------------------------------------------------------------------------------------*/


//Global Pointers to user generated classes
shaders sPointer;
//modelLoader mPointer;
GLuint  c_Shader, i_Shader, p_Shader,  p_Matrix;
GLFWwindow* window;
Container container;
ParticleSystem* p_System;
Camera* cam;
Light* l_Amb;

//Global variables
int g_gl_w=1280;
int g_gl_h=720;
int butt = 0;
double xPos=NULL, yPos=NULL;
float cTime, delta, f_Delta;
const float m_Sens = 0.1f;
bool simPause = true, renderContainer = true, renderParticles = true, mFocus = true, mFocFirst = true;

//Global method prototypes
void fps(GLFWwindow*);
float getDT();
void init(void);
int main();
void mouseButt(GLFWwindow*, int a, int b, int c);
void render();
void resize(GLFWwindow*, int w, int h);
void setupGLHints(void);
void setupShaders(void);
void setupWindow(void);
void update();

//Setup the GL hints
void setupGLHints(void){
	// *** SHADING
	glShadeModel	(GL_SMOOTH);
	glEnable		(GL_NORMALIZE);
	glHint			(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	// *** CULLING
	glPolygonMode	(GL_FRONT_AND_BACK, GL_FILL);
	glEnable		(GL_CULL_FACE);
	glFrontFace		(GL_CW);
	glCullFace		(GL_BACK);
	glEnable		(GL_DEPTH_TEST);
	glDepthFunc		(GL_LESS);
	glDepthMask		(GL_TRUE);
	
	// *** BLENDING
	glEnable		(GL_BLEND);
	glBlendFunc		(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//Setup the Shaders
void setupShaders(void){
	c_Shader	= sPointer.loadShaders("shaders/vs.glsl","shaders/fs.glsl");
	i_Shader	= sPointer.loadShaders("shaders/i_VS.glsl", "shaders/i_FS.glsl");
}

//Setup the window, and set the hints to get the best possible look.
void setupWindow(void){
	glfwInit();
	if(!glfwInit()){
		fprintf(stderr, "ERROR | Could not start GLFW3\n");
	}
	window = glfwCreateWindow(1280, 720, "SPH", NULL, NULL);
	if(!window){
		fprintf(stderr, "ERROR | Could not create window with GLFW\n"); 
		glfwTerminate();
		std::exit(0);
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, resize);
	glfwSetMouseButtonCallback(window, mouseButt);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); //Multi sample Anti Aliasing
	GLFWmonitor* mon=glfwGetPrimaryMonitor();
	glfwSwapInterval(1);
}

//Handle mouse and keyboard input to the program
void Input(){
	float camMoveSpeed = 0.01f;

	//	Camera interaction keys
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		cam->OffsetPosition(camMoveSpeed  *  cam->Forward());
	}
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		cam->OffsetPosition(camMoveSpeed  *  -cam->Forward());
	}
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		cam->OffsetPosition(camMoveSpeed  *  -cam->Right());
	}
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		cam->OffsetPosition(camMoveSpeed  *  cam->Right());
	}
	if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		cam->Reset();
	}

	//Sim interaction keys --currently not doing much
	if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
		if(simPause)
			simPause = false;
		else
			simPause = true;
	}

	if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
		if(renderContainer)
			renderContainer = false;
		else
			renderContainer = true;
	}

	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
		if(!renderParticles)
			renderParticles = true;
		else
			renderParticles = false;
	}

	if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		p_System->DoThisTing();
	}


	//	Program Exit key
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window, 1);
	}
	
	///	MOUSE INPUT
	glfwGetCursorPos(window, &xPos, &yPos);
	glfwSetCursorPos(window, 0, 0);	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	cam->OffsetOrientation(m_Sens * (float)xPos, m_Sens * (float)yPos);
	glfwSetCursorPos(window, 0, 0);	
	//add mouse input to camera
}

//Get Delta TIme
float getDT(){
	float nTime = glfwGetTime();
	float fTime = nTime - cTime;
	cTime = nTime;
	return fTime;
}

//Calculate FPS and show in window Title
void fps(GLFWwindow*)
{
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if(elapsed_seconds > 0.25)
	{
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		if(simPause)
			sprintf(tmp, "SPH | FPS: %.0f | Current Delta: %.10f | SIMULATION = PAUSED |", fps, f_Delta);
		else
				sprintf(tmp, "SPH | FPS: %.0f | Current Delta: %.10f | SIMULATION = RUNNING |", fps, f_Delta);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count ++;
}

//This method handles mouse presses for the program!
void mouseButt(GLFWwindow*, int a, int b, int c){
	
	//left mouse button
	if(a == 0 && b == 1){
	}else if(a == 0 && b == 0){
		
	}
	
	//middle mouse button
	if(a==2 && b == 1){printf("middle button pressed @ (%.0f, %.0f)\n", xPos, yPos);}

	//right mouse button
	if(a==1 && b == 1){
	
	}else if (a==1 && b == 0){
		printf("right button released\n");
	}

}

//Resize deals with the resizing of the program window
void resize(GLFWwindow*, int w, int h)
{
	g_gl_w=w;
	g_gl_h=h;
	glViewport(0, 0, g_gl_w, g_gl_h);
}

//Init method, to set everything up!
void init()
{
	setupWindow();
	setupGLHints();
	glewInit();
	glViewport(0, 0, g_gl_w, g_gl_h);
	// Set up scene camera
	cam = new Camera(glm::vec3(0.0f, 1.5f, 1.0f), 45.0f, (GLfloat)g_gl_w / (GLfloat)g_gl_h, 0.1f, 1000.0f);
	
	//gen the container VAO here because it wants to play silly buggers
	container.genVAO();

	//instantiate any pointer objects
	p_System = new ParticleSystem;

	//	Set up the light for the scene
	l_Amb = new Light(	1,						 //	Type
						Vec3f(0.0f, 2.0f, 0.0f), //	Position
						Vec3f(1.0f, 1.0f, 1.0f), // Intensity
						0.2f,					 // Ambient Coefficient (Default 0.2f)
						0.001f);				 // Attenuation (Default 0.005f)
}


//Render deals with all instances that need to be drawn
void render(void){

	//Render Container
	if(renderContainer){
		sPointer.UseShader(	c_Shader, container.GetModel(), cam, l_Amb->GetPos(), 
							l_Amb->GetInt(), l_Amb->GetAC(), l_Amb->GetAtt());
		container.Render();
	}
	//Render Particles
	if(renderParticles){
		sPointer.UseShaderInstanced(i_Shader, cam, p_System->getParticleCount(), p_System->GetIPosVec(), p_System->GetIModVec(), 
			l_Amb->GetPos(), l_Amb->GetInt(), l_Amb->GetAC(), l_Amb->GetAtt(), p_System->getParticles() );

		p_System->m_Loader.RenderIModel(p_System->GetModel(), p_System->getParticleCount());
	}
}


//This method handles per-frame updates, and any methods/vars that need updating
void update(void){
	f_Delta = getDT();
	Input();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	if(!simPause)
		p_System->Run(f_Delta);
}

//Main method containing the main loop
int main(){
	init();
	setupShaders();
	cTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)){
		fps(window);
		update();
		render();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	system("cls"); //clears the console window
	printf("\tHAVE NICE DAY xoxo\n\n\n");
	return 0;
}
