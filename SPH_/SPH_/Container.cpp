#include "Container.h"

static const GLfloat col2[]=
					{   0.583f,  0.771f,  0.014f,
						0.609f,  0.115f,  0.436f,
						0.327f,  0.483f,  0.844f,
						0.822f,  0.569f,  0.201f,
						0.435f,  0.602f,  0.223f,
						0.310f,  0.747f,  0.185f,
						0.597f,  0.770f,  0.761f,
						0.559f,  0.436f,  0.730f,
						0.359f,  0.583f,  0.152f,
						0.483f,  0.596f,  0.789f,
						0.559f,  0.861f,  0.639f,
						0.195f,  0.548f,  0.859f,
						0.014f,  0.184f,  0.576f,
						0.771f,  0.328f,  0.970f,
						0.406f,  0.615f,  0.116f,
						0.676f,  0.977f,  0.133f,
						0.971f,  0.572f,  0.833f,
						0.140f,  0.616f,  0.489f,
						0.997f,  0.513f,  0.064f,
						0.945f,  0.719f,  0.592f,
						0.543f,  0.021f,  0.978f,
						0.279f,  0.317f,  0.505f,
						0.167f,  0.620f,  0.077f,
						0.347f,  0.857f,  0.137f,
						0.055f,  0.953f,  0.042f,
						0.714f,  0.505f,  0.345f,
						0.783f,  0.290f,  0.734f,
						0.722f,  0.645f,  0.174f,
						0.302f,  0.455f,  0.848f,
						0.225f,  0.587f,  0.040f,
						0.517f,  0.713f,  0.338f,
						0.053f,  0.959f,  0.120f,
						0.393f,  0.621f,  0.362f,
						0.673f,  0.211f,  0.457f,
						0.820f,  0.883f,  0.371f,
						0.982f,  0.099f,  0.879f
						};


static const GLfloat col[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f, 

		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
};

Container::Container(){
	VBO = VAO = CBO = 0;
	Model =  glm::mat4(1.0f);

	containerBB.min = Vec3f(-WIDTH, -HEIGHT, -WIDTH);
	containerBB.max = Vec3f( WIDTH,  HEIGHT,  WIDTH);
	//FrontFace
	cont[0]  = Vec3f(-WIDTH, -HEIGHT, -WIDTH); //Corner Bottom Left
	cont[1]  = Vec3f(-WIDTH,  HEIGHT, -WIDTH); //Corner Bottom Right
	cont[2]  = Vec3f( WIDTH,  HEIGHT, -WIDTH); //Corner Top Right
	cont[3]  = Vec3f( WIDTH,  -HEIGHT, -WIDTH); //Corner Top Left

	contNormals[0] = Vec3f(0.0f, 0.0f, 1.0f);
	contNormals[1] = Vec3f(0.0f, 0.0f, 1.0f);
	contNormals[2] = Vec3f(0.0f, 0.0f, 1.0f);
	contNormals[3] = Vec3f(0.0f, 0.0f, 1.0f);
	//BackFace
	cont[4]  = Vec3f(-WIDTH, -HEIGHT,  WIDTH); //Corner Bottom Left
	cont[5]  = Vec3f(-WIDTH,  HEIGHT,  WIDTH); //Corner Bottom Right
	cont[6]  = Vec3f( WIDTH,  HEIGHT,  WIDTH); //Corner Top Right
	cont[7]  = Vec3f( WIDTH, -HEIGHT,  WIDTH); //Corner Top Left

	contNormals[4] = Vec3f(0.0f, 0.0f, -1.0f);
	contNormals[5] = Vec3f(0.0f, 0.0f, -1.0f);
	contNormals[6] = Vec3f(0.0f, 0.0f, -1.0f);
	contNormals[7] = Vec3f(0.0f, 0.0f, -1.0f);
	//LeftFace
	cont[8]  = Vec3f(-WIDTH, -HEIGHT,  WIDTH); //Corner Bottom Left
	cont[9]  = Vec3f(-WIDTH, -HEIGHT, -WIDTH); //Corner Bottom Right
	cont[10] = Vec3f(-WIDTH,  HEIGHT, -WIDTH); //Corner Top Right
	cont[11] = Vec3f(-WIDTH,  HEIGHT,  WIDTH); //Corner Top Left

	contNormals[8]	= Vec3f(-1.0f, 0.0f, 0.0f);
	contNormals[9]	= Vec3f(-1.0f, 0.0f, 0.0f);
	contNormals[10] = Vec3f(-1.0f, 0.0f, 0.0f);
	contNormals[11] = Vec3f(-1.0f, 0.0f, 0.0f);
	//RightFace
	cont[12] = Vec3f( WIDTH, -HEIGHT, -WIDTH); //Corner Bottom Left
	cont[13] = Vec3f( WIDTH, -HEIGHT,  WIDTH); //Corner Bottom Right
	cont[14] = Vec3f( WIDTH,  HEIGHT,  WIDTH); //Corner Top Right
	cont[15] = Vec3f( WIDTH,  HEIGHT, -WIDTH); //Corner Top Left

	contNormals[12] = Vec3f(1.0f, 0.0f, 0.0f);
	contNormals[13] = Vec3f(1.0f, 0.0f, 0.0f);
	contNormals[14] = Vec3f(1.0f, 0.0f, 0.0f);
	contNormals[15] = Vec3f(1.0f, 0.0f, 0.0f);
	//TopFace
	cont[16] = Vec3f(-WIDTH,  HEIGHT, -WIDTH); //Corner Bottom Left
	cont[17] = Vec3f( WIDTH,  HEIGHT, -WIDTH); //Corner Bottom Right
	cont[18] = Vec3f( WIDTH,  HEIGHT,  WIDTH); //Corner Top Right
	cont[19] = Vec3f(-WIDTH,  HEIGHT,  WIDTH); //Corner Top Left

	contNormals[16] = Vec3f(0.0f, 1.0f, 0.0f);
	contNormals[17] = Vec3f(0.0f, 1.0f, 0.0f);
	contNormals[18] = Vec3f(0.0f, 1.0f, 0.0f);
	contNormals[19] = Vec3f(0.0f, 1.0f, 0.0f);
	//BottomFace
	cont[20] = Vec3f(-WIDTH, -HEIGHT, -WIDTH); //Corner Bottom Left
	cont[21] = Vec3f( WIDTH, -HEIGHT, -WIDTH); //Corner Bottom Right
	cont[22] = Vec3f( WIDTH, -HEIGHT,  WIDTH); //Corner Top Right
	cont[23] = Vec3f(-WIDTH, -HEIGHT,  WIDTH); //Corner Top Left

	contNormals[20] = Vec3f(0.0f, -1.0f, 0.0f);
	contNormals[21] = Vec3f(0.0f, -1.0f, 0.0f);
	contNormals[22] = Vec3f(0.0f, -1.0f, 0.0f);
	contNormals[23] = Vec3f(0.0f, -1.0f, 0.0f);
}

void Container::genVAO(){

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cont), cont, GL_STATIC_DRAW);
	glGenBuffers(1, &CBO);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col2), col2, GL_STATIC_DRAW);
	glGenBuffers(1, &VNO);
	glBindBuffer(GL_ARRAY_BUFFER, VNO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(contNormals), contNormals, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VNO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
}

void Container::Render(){
	glBindVertexArray(VAO); 
	glDrawArrays(GL_LINES, 0, 6*4);
}
Container::~Container(){}