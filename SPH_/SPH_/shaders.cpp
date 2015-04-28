#include "shaders.h"	

shaders::shaders(){}

GLuint shaders::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	//printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	//printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	printf("Successfully loaded and linked: %s & %s\n", vertex_file_path, fragment_file_path);
	return ProgramID;
}


//	Current shader for the container
void shaders::UseShader(GLuint program, glm::mat4 M, Camera* Cam, Vec3f l_Pos, Vec3f l_Int, float l_Ac, float l_At){
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "Model"), 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "Camera"), 1, GL_FALSE, &Cam->Matrix()[0][0]);
	glUniform3f(glGetUniformLocation(program, "camPos"), Cam->GetPosition().x, Cam->GetPosition().y, Cam->GetPosition().z);

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glUniform3f(glGetUniformLocation(program, "light.pos"), l_Pos.x, l_Pos.y, l_Pos.z);
	glUniform3f(glGetUniformLocation(program, "light.intensity"), l_Int.x, l_Int.y, l_Int.z);
	glUniform1f(glGetUniformLocation(program, "light.ambCoef"), l_Ac);
	glUniform1f(glGetUniformLocation(program, "light.atten"), l_At);

}



//void shaders::UseShaderInstanced(GLuint program, Camera* Cam, GLuint insCount, vector<glm::vec4>* IP, vector<glm::mat4>* iMV, Vec3f l_Pos, Vec3f l_Int, float l_Ac, float l_At){
//	glUseProgram(program);
//
//	for(size_t i = 0; i < insCount; i++)
//	{
//		string index = std::to_string(i);
//		GLuint loc = glGetUniformLocation(program, ("i_M["+index+"]").c_str());
//		glUniformMatrix4fv(loc, 1, GL_FALSE, &iMV->at(i)[0][0]);
//
//		loc = glGetUniformLocation(program, ("i_P["+index+"]").c_str());
//		glUniform4f(loc, IP->at(i).x, IP->at(i).y, IP->at(i).z, IP->at(i).w);
//	}
//	
//	glUniformMatrix4fv(glGetUniformLocation(program, "Camera"), 1, GL_FALSE, &Cam->Matrix()[0][0]);
//	glUniform3f(glGetUniformLocation(program, "camPos"), Cam->GetPosition().x, Cam->GetPosition().y, Cam->GetPosition().z);
//	glUniform3f(glGetUniformLocation(program, "colour"), 0.0f,0.0f,1.0f);
//	glUniform3f(glGetUniformLocation(program, "light.pos"), l_Pos.x, l_Pos.y, l_Pos.z);
//	glUniform3f(glGetUniformLocation(program, "light.intensity"), l_Int.x, l_Int.y, l_Int.z);
//	glUniform1f(glGetUniformLocation(program, "light.ambCoef"), l_Ac);
//	glUniform1f(glGetUniformLocation(program, "light.atten"), l_At);
//
//}


//current shader for the instanced particles. 
void shaders::UseShaderInstanced(GLuint program, Camera* Cam, GLuint insCount, vector<glm::vec4>* IP, vector<glm::mat4>* iMV, 
								 Vec3f l_Pos, Vec3f l_Int, float l_Ac, float l_At, std::vector<Particle>* ps){
	glUseProgram(program);

	for(size_t i = 0; i < insCount; i++)
	{
		string index = std::to_string(i);
		GLuint loc = glGetUniformLocation(program, ("i_M["+index+"]").c_str());
		glUniformMatrix4fv(loc, 1, GL_FALSE, &iMV->at(i)[0][0]);

		loc = glGetUniformLocation(program, ("i_P["+index+"]").c_str());
		glUniform4f(loc, IP->at(i).x, IP->at(i).y, IP->at(i).z, IP->at(i).w);

		Vec3f col = ps->at(i).GetColour();
		loc = glGetUniformLocation(program, ("i_C["+index+"]").c_str());
		glUniform3f(loc, col.x , col.y, col.z);
	}
	
	glUniformMatrix4fv(glGetUniformLocation(program, "Camera"), 1, GL_FALSE, &Cam->Matrix()[0][0]);
	glUniform3f(glGetUniformLocation(program, "camPos"), Cam->GetPosition().x, Cam->GetPosition().y, Cam->GetPosition().z);
	
	glUniform3f(glGetUniformLocation(program, "light.pos"), l_Pos.x, l_Pos.y, l_Pos.z);
	glUniform3f(glGetUniformLocation(program, "light.intensity"), l_Int.x, l_Int.y, l_Int.z);
	glUniform1f(glGetUniformLocation(program, "light.ambCoef"), l_Ac);
	glUniform1f(glGetUniformLocation(program, "light.atten"), l_At);

}