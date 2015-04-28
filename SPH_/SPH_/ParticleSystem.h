#ifndef ParticleSystem_h
#define ParticleSystem_h
#include "Particle.h"
#include "Container.h"
#include "modelLoader.h"
#include "KDTree.h"
#include <vector>
#include<random>

using namespace std;

#define MAX_PARTICLES 1500 //REMEMBER TO UPDATE NUMBER OF INSTANCES IN VERTEX SHADER, PENDEJO 
#define GRAVITY 0.005f
#define SEARCH_RADIUS 0.5f

class ParticleSystem{
private:
	//	Methods
	void NeighbourSearch(Particle*);
	void UpdateNeighbours();
	void UpdatePressure();
	void UpdateAcceleration();
	void BoundaryCheck(Particle*);
	void MovePFromBounds(Particle*);
	void MoveOutOfCollision(Particle*);
	void LoadModel();
	void BuildKD();
	//	Variables
	vector<Particle>	Particles;
	Container			container;
	BB					dim;
	model*				p_Model;
	
	bool				simPause;
	float				xc, yc, zc, px, py, pz;
	std::vector<glm::vec4> i_Positions;
	std::vector<glm::mat4> i_Models;
	KDTree *pTree;
	kdtree *aTree;
	kdIter *aIter;
public:
	modelLoader			m_Loader;
	
	//	Methods/Functions
	ParticleSystem();
	~ParticleSystem();
	void GenParticles();
	void Run(float);
	void renderIModel(size_t, const glm::mat4*, const glm::mat4*);
	void DoThisTing();
	//	Getters
	vector<Particle>*	getParticles(){return &Particles;}
	const int			getParticleCount(){return Particles.size();}
	vector<Particle>*	GetContainer(){return &Particles;}
	Particle			getParticle(int i){return Particles[i];}
	bool				GetPauseState(){return simPause;}
	model*				GetModel(){return p_Model;}
	vector<glm::vec4>*  GetIPosVec(){return &i_Positions;}
	vector<glm::mat4>*	GetIModVec(){return &i_Models;}
	//	Setters
	void				SetPause(bool b){simPause = b;}
};
#endif