#version 400

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexNorm;
layout (location=2) in vec2 texCoord;
layout (location=8) in vec4 i_Pos;

uniform vec3 camPos;
uniform mat4 Camera;
uniform vec3 colour;
const int insCount = 1500; //The number of instances (subject to change) < 3000 plx
uniform mat4 i_M[insCount];
uniform vec3 i_P[insCount];
uniform vec3 i_C[insCount];


out vec4 fragmentColor;
out vec3 fragmentNormal;
out vec3 fragmentVertex;
out mat4 m_Out;
out vec3 CamPos;
flat out int instanceID;

void main()
{
	fragmentColor	= vec4(i_C[gl_InstanceID].rgb, 1.0f);
	fragmentNormal	= vertexNorm;
	m_Out			= i_M[gl_InstanceID];
	fragmentVertex	= vertexPos + i_P[gl_InstanceID];
	gl_Position		= (Camera * i_M[gl_InstanceID]) * vec4(vertexPos + i_P[gl_InstanceID], 1);
	instanceID		= gl_InstanceID;
}