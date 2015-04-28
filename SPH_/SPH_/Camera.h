#ifndef Camera_H
#define Camera_H

#include "Vec3f.h"
#include "include\glm\glm.hpp"
#define GLM_FORCE_RADIANS
#include "include\glm\gtx\transform.hpp"
#include <assert.h>

#define toRadian(x) (float)(((x) * 3.141592653589f / 180.0f))

static const float rads = 3.141592653589f / 180.0f;
static const float max_VAngle = 85.0f;

class Camera{
private:
	//	Variables 
	glm::mat4 m_Perspective;
	glm::vec3 m_Position, m_Forward, m_Up, m_Right, m_At, m_Origin;
	float h_Angle, v_Angle, FOV, nearPlane, farPlane, vp_Aspect;

	//	Methods
	void NormailseAngles();
public:
	Camera();
	Camera(glm::vec3, float, float, float, float);
	void OffsetPosition(glm::vec3&);
	void OffsetOrientation(float, float);
	void LookAt(glm::vec3);
	void Reset();

	// Getters
	glm::vec3 GetPosition(){return m_Position;}
	glm::mat4 GetPerspective() const {return m_Perspective;}
	glm::mat4 GetViewProjection() const;
	float GetNearPlane(){return nearPlane;}
	float GetFarPlane(){return farPlane;}
	float GetAspect(){return vp_Aspect;}

	glm::mat4 Orientation();
	glm::mat4 Matrix();
	glm::mat4 Projection();
	glm::mat4 View();
	glm::vec3 Up();
	glm::vec3 Right();
	glm::vec3 Forward();

	//for instanced
	glm::mat4 PerspProjTransform();
	glm::mat4 GetWorldTrans();
	glm::mat4 GetVPTrans();
	glm::mat4 GetWVPTrans();
	glm::mat4 InitTranslationTransform(float, float, float);
	glm::mat4 InitScaleTransform(float, float, float);
	glm::mat4 InitRotateTransform(float, float, float);
	glm::mat4 InitCamTransform(glm::vec3, glm::vec3);

	//	Setters
	void SetPosition(glm::vec3 p){m_Position = p;}
	void SetFOV(float);
	void SetPlanes(float, float);
	void SetAspectRatio(float ar){vp_Aspect = ar;}
	

};
#endif