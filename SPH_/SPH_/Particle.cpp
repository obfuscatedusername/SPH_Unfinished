#include "Particle.h"

Particle::Particle(){
	ModelView = glm::mat4(1.0f);
}

Particle::Particle(Vec3f position){
	pos					= position;
	acceleration		= Vec3f(0.0f, 0.0f, 0.0f);
	colour				= Vec3f(0.0f, 0.0f, 1.0f);
	dir					= Vec3f(0.0f, 0.0f, 0.0f);
	acceleration		= Vec3f(0.0f, 0.0f, 0.0f);
	velocity			= Vec3f(0.0f, 0.0f, 0.0f);
	mass				= 1.0f;
	pressure			= 0.0f;
	viscosity			= 0.0f;
	radius				= 0.05f;
	inBounds			= true;
	isSurfaceParticle	= false;
	
}

Particle::~Particle(){

}

bool Particle::LoadModel(){
	
	return true;
}


void Particle::GenParticle(Vec3f p){
	pos					= p;
	acceleration		= Vec3f(0.0f, 0.0f, 0.0f);
	colour				= Vec3f(0.0f, 0.0f, 1.0f);
	dir					= Vec3f(0.0f, 0.0f, 0.0f);
	acceleration		= Vec3f(0.0f, 0.0f, 0.0f);
	velocity			= Vec3f(0.0f, 0.0f, 0.0f);
	mass				= 1.0f;
	pressure			= 0.0f;
	viscosity			= 0.0f;
	radius				= 0.02f;
	inBounds			= true;
	isSurfaceParticle	= false;
	ModelView			= glm::translate(ModelView, glm::vec3(p.x, p.y, p.z));
	ModelView			= glm::scale(ModelView, glm::vec3(p_Scale));
}

void Particle::Render(){
	m_Loader.renderModel(p_Model);
}

void Particle::UpdateMV(Vec3f p){
	glm::mat4 mv = glm::mat4(1.0f);
	mv			= glm::translate(mv, glm::vec3(p.x, p.y, p.z));
	mv			= glm::scale(mv, glm::vec3(p_Scale));
	ModelView	= mv;
}


