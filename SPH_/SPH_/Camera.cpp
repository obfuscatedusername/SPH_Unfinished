#include "Camera.h"

Camera::Camera(){
	m_Position	= glm::vec3(0.0f, 0.0f, -13.0f);
	v_Angle		= 0.0f;
	h_Angle		= 0.0f;
	FOV			= 45.0f;
	nearPlane	= 0.01f;
	farPlane	= 1000.0f;
	vp_Aspect	= 16.0f/9.0f;
}

Camera::Camera(glm::vec3 p, float fov, float ar, float np, float fp){
	m_Position	= p;
	m_Origin	= p;
	v_Angle		= 15.0f;
	h_Angle		= 0.0f;
	FOV			= fov;
	nearPlane	= np;
	farPlane	= fp;
	vp_Aspect	= ar;
	m_At		= glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::Reset(){
	m_Position	=	m_Origin;
	v_Angle		= 15.0f;
	h_Angle		= 0.0f;
	FOV			= 45.0f;
	nearPlane	= 0.1f;
	farPlane	= 1000.0f;
	vp_Aspect	= 16.0f/9.0f;
	m_At		= glm::vec3(0.0f, 0.0f, 0.0f);
}


glm::mat4 Camera::Matrix(){
	glm::mat4 cam	= glm::perspective(FOV, vp_Aspect, nearPlane, farPlane);
	cam				*= Orientation();
	cam				= glm::translate(cam, -m_Position);
	return cam;
}

glm::mat4 Camera::GetViewProjection() const{
	return m_Perspective * glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

void Camera::SetFOV(float fov){
	assert(fov > 0.0f && fov < 180.0f);
	FOV = fov;
}

void Camera::SetPlanes(float np, float fp){
	assert(np > 0.0f);
	assert(fp > np);
	nearPlane	= np;
	farPlane	= fp;
}

glm::mat4 Camera::Orientation(){
	glm::mat4 o;
	o = glm::rotate(o, glm::radians(v_Angle), glm::vec3(1.0f, 0.0f, 0.0f));
	o = glm::rotate(o, glm::radians(h_Angle), glm::vec3(0.0f, 1.0f, 0.0f));
	return o;
}

void Camera::OffsetOrientation(float r_Angle, float u_Angle){
	h_Angle += r_Angle;
	v_Angle += u_Angle;
	NormailseAngles();
}

void Camera::OffsetPosition(glm::vec3& p){
	m_Position += p;
}

void Camera::LookAt(glm::vec3 p){
	assert(p != m_Position);
	glm::vec3 dir	= glm::normalize(p - m_Position);
	v_Angle			= glm::radians(asinf(-dir.y));
	h_Angle			= glm::radians(atan2f(-dir.x, -dir.z));
	NormailseAngles();
}

void Camera::NormailseAngles(){
	h_Angle = fmodf(h_Angle, 360.0f);
	if(h_Angle < 0.0f)
		h_Angle +=360.0f;

	if(v_Angle > max_VAngle)
		v_Angle = max_VAngle;
	else if (v_Angle < -max_VAngle)
		v_Angle = -max_VAngle;
}

glm::vec3 Camera::Up(){
	glm::vec4 temp = glm::inverse(Orientation()) * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	return glm::vec3(temp);
}

glm::vec3 Camera::Right(){
	glm::vec4 temp = glm::inverse(Orientation()) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	return glm::vec3(temp);
}

glm::vec3 Camera::Forward(){
	glm::vec4 temp = glm::inverse(Orientation()) * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
	return glm::vec3(temp);
}


glm::mat4 Camera::Projection(){
	return glm::perspective(glm::radians(FOV), vp_Aspect, nearPlane, farPlane);
}

glm::mat4 Camera::View(){
	return Orientation() * glm::translate(glm::mat4(), -m_Position);
}

// For instanced
glm::mat4 Camera::PerspProjTransform(){
	
	glm::mat4 t;
	const float zRange		= farPlane - nearPlane;
	const float tanHalfFoV	= tanf(toRadian(FOV / 2.0f));
	
	t[0][0] = 1.0f / (tanHalfFoV * vp_Aspect);
	t[1][0] = 0.0f;
	t[2][0] = 0.0f;
	t[3][0] = 0.0f;

	t[0][1] = 0.0f;
	t[1][1] = 1.0f / tanHalfFoV;
	t[2][1] = 0.0f;
	t[3][1] = 0.0f;

	t[0][2] = 0.0f;
	t[1][2] = 0.0f;
	t[2][2] = (-nearPlane - farPlane) / zRange;
	t[3][2] = 0.0f;

	t[0][3] = 0.0f;
	t[1][3] = 0.0f;
	t[2][3] = 2.0f * farPlane * nearPlane / zRange;
	t[3][3] = 0.0f;

	return t;
}

glm::mat4 Camera::GetWorldTrans(){
	glm::mat4 ST, RT, TT;

	ST = InitScaleTransform(0.5f, 1.0f, 1.0f);
	RT = InitRotateTransform(0.0f, v_Angle, h_Angle);
	TT = InitTranslationTransform(m_Position.x, m_Position.y, m_Position.z);

	return TT * RT * ST;
}

glm::mat4 Camera::GetVPTrans(){
	glm::mat4 CTT, CRT, PPT;

	CTT = InitTranslationTransform(-m_Position.x, -m_Position.y, -m_Position.z);
	CRT = InitCamTransform(m_At, m_Up);
	PPT = PerspProjTransform();

	return PPT * CRT * CTT;	
}

glm::mat4 Camera::GetWVPTrans(){
	glm::mat4 t1 = GetWorldTrans();
	glm::mat4 t2 = GetVPTrans();
	return t2 * t1;
}

glm::mat4 Camera::InitCamTransform(glm::vec3 target, glm::vec3 up){
	glm::vec3 n = glm::normalize(target);
	up = glm::normalize(up);
	glm::vec3 u = glm::cross(u, n);
	glm::vec3 v = glm::cross(n, u);
	glm::mat4 t;

	t[0][0] = u.x;	t[0][1] = u.y;	t[0][2] = u.z; t[0][3] = 0.0f;
	t[1][0] = v.x;	t[1][1] = v.y;	t[1][2] = v.z;	t[1][3] = 0.0f;
	t[2][0] = n.x;	t[2][1] = n.y;	t[2][2] = n.z;	t[2][3] = 0.0f;
	t[3][0] = 0.0f;	t[3][1] = 0.0f;	t[3][2] = 0.0f;	t[3][3] = 0.0f;
	
	return t;
}

glm::mat4 Camera::InitRotateTransform(float a, float b, float c){
	glm::mat4 rx, ry, rz;
	float x = toRadian(a);
	float y = toRadian(b);
	float z = toRadian(c);

	rx[0][0] = 1.0f; rx[0][1] = 0.0f;    rx[0][2] = 0.0f;     rx[0][3] = 0.0f;
	rx[1][0] = 0.0f; rx[1][1] = cosf(x); rx[1][2] = -sinf(x); rx[1][3] = 0.0f;
	rx[2][0] = 0.0f; rx[2][1] = sinf(x); rx[2][2] = cosf(x);  rx[2][3] = 0.0f;
	rx[3][0] = 0.0f; rx[3][1] = 0.0f;	 rx[3][2] = 0.0f;     rx[3][3] = 1.0f;

	ry[0][0] = cosf(y); ry[0][1] = 0.0f; ry[0][2] = -sinf(y); ry[0][3] = 0.0f;
	ry[1][0] = 0.0f;    ry[1][1] = 1.0f; ry[1][2] = 0.0f;	  ry[1][3] = 0.0f;
	ry[2][0] = sinf(y); ry[2][1] = 0.0f; ry[2][2] = cosf(y);  ry[2][3] = 0.0f;
	ry[3][0] = 0.0f;    ry[3][1] = 0.0f; ry[3][2] = 0.0f;     ry[3][3] = 1.0f;

	rz[0][0] = cosf(z); rz[0][1] = -sinf(z); rz[0][2] = 0.0f; rz[0][3] = 0.0f;
	rz[1][0] = sinf(z); rz[1][1] = cosf(z);	 rz[1][2] = 0.0f; rz[1][3] = 0.0f;
	rz[2][0] = 0.0f;    rz[2][1] = 0.0f;	 rz[2][2] = 1.0f; rz[2][3] = 0.0f;
	rz[3][0] = 0.0f;    rz[3][1] = 0.0f;	 rz[3][2] = 0.0f; rz[3][3] = 1.0f;

	return rz * ry * rx;
}

glm::mat4 Camera::InitTranslationTransform(float a, float b, float c){
	glm::mat4 m;
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = a;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = b;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = c;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return m;
}

glm::mat4 Camera::InitScaleTransform(float a, float b, float c){
	glm::mat4 m;
	m[0][0] = a;	m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = b;	m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = c;	m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	return m;
}
