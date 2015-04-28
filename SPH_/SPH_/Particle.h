#ifndef Particle_h
#define Particle_h
#include <vector>
#include "Vec3f.h"
#include "modelLoader.h"

const static float p_Scale = 0.02f;

class Particle{
	private:
	Vec3f pos, dir, colour, acceleration, velocity;
	float mass, pressure, viscosity, radius, density;
	bool inBounds, isSurfaceParticle;
	model*	p_Model;
	bool LoadModel();
	glm::mat4 ModelView;
	std::vector<unsigned int> n_Indexes; //indexes of neighbour particles

public:
	//Methods
	Particle();
	Particle(Vec3f);
	~Particle();
	void GenParticle(Vec3f);
	void Render();
	void UpdateMV(Vec3f);
	void RemoveNeighbourIndex(unsigned int);

	//Getters
	float GetRadius(){return radius;}
	bool GetBoundCheck(){return inBounds;}
	model* GetModel(){return p_Model;}
	Vec3f GetColour(){return colour;};
	glm::mat4 GetMVM(){return ModelView;}
	Vec3f GetPosition(){return pos;}
	glm::vec3 GetPositionGLM(){return glm::vec3(pos.x, pos.y, pos.z);}
	float GetScale(){return p_Scale;}
	std::vector<unsigned int> GetNeighbourIndexes(){return n_Indexes;}
	
	//Setters
	void SetPosition(Vec3f p){pos = p; UpdateMV(p);}
	void SetBounds(bool b){inBounds = b;}
	void SetModel(model* p){p_Model = p;}
	void SetColour(float r, float g, float b){colour = Vec3f(r, g, b);}
	void SetColour(Vec3f c){colour = c;}
	void AddNeighbourIndex(unsigned int i){n_Indexes.push_back(i);}
	
	//Variables
	modelLoader m_Loader;
	
};
#endif