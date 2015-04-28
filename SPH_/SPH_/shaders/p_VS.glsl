#version 400

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexNorm;
layout (location=2) in vec2 texCoord;

uniform mat4 Model;
uniform vec3 camPos;
uniform mat4 Camera;
uniform vec3 colour;

out vec4 fragmentColor;
out vec3 fragmentNormal;
out vec3 fragmentVertex;
out mat4 m_Out;
out vec3 CamPos;

void main()
{
	fragmentColor	= vec4(colour.rgb, 1.0f);
	fragmentNormal	= vertexNorm;
	fragmentVertex = vertexPos;
	CamPos			= camPos;
	m_Out			= Model;
	gl_Position		= Camera * Model * vec4(vertexPos, 1);
}