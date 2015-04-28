#version 400

in vec4 fragmentColor;
in vec3 fragmentNormal;
in vec3 fragmentVertex;
in vec2 tex_UV;
in vec3 CamPos;
in mat4 m_Out;

uniform struct Light{
	vec3 pos;
	vec3 intensity;
	float ambCoef;
	float atten;
}light;

out vec3 color;
out vec4 outputColour;

void main(void)
{
	vec3 normal			= normalize(transpose(inverse(mat3(m_Out))) * fragmentNormal);
	vec3 surfacePos		= vec3(m_Out * vec4(fragmentVertex, 1));
	vec3 surfaceToLight = normalize(light.pos - surfacePos);
	vec4 surfaceColor	= fragmentColor;
	vec3 surfaceToCam	= normalize(CamPos - surfacePos);

	//	Ambient
	vec3 ambient		= light.ambCoef * surfaceColor.rgb * light.intensity;

	//	Diffuse
	float diffuseCoef	= max(0.0f, dot(normal, surfaceToLight));
	vec3 diffuse		= diffuseCoef * surfaceColor.rgb * light.intensity;

	//	Specular
	float matShininess	= 64.0f;
	vec3 matSpecular	= vec3(0.5f, 0.5f, 0.5f);
	float specCoef = 0.0f;
	if(diffuseCoef > 0.0f){
			specCoef	= pow(max(0.0f, dot(surfaceToCam, reflect(-surfaceToLight, normal))), matShininess);
			}
	vec3 specular		= specCoef * matSpecular * light.intensity;
		
	//	Attenuation
	float distToLight	= length(light.pos - surfacePos);
	float attenuation	= 1.0f / (1.0f + light.atten * pow(distToLight, 2));

	// Linear color (color before gamma correction)
	vec3 linearColor	= ambient + attenuation * (diffuse + specular);
	
	//	Final Color Output
	vec3 gamma			= vec3(1.0f/2.2f); //gamma corerction, raise to the power of 1.0f/2.2f
	outputColour		= vec4(pow(linearColor, gamma), surfaceColor.a);
}