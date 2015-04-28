#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(){
	dim			= container.getDims();
	simPause	= true;
	
	LoadModel();
	GenParticles();
	pTree	= new KDTree();
	aTree	=  NULL;
	aIter	= NULL;
}

ParticleSystem::~ParticleSystem(){}

void ParticleSystem::BuildKD(){
	aTree = new kdtree();
	pTree->BuildKDTree(&Particles, Particles.size(), &aTree);
}

void ParticleSystem::NeighbourSearch(Particle* p){
	aTree = new kdtree();
	aTree = pTree->BuildKDTree(&Particles, Particles.size(), &aTree);

	std::vector<Particle> Neighbours;
	size_t c = 0;
	Vec3f ss1, ss2, pp;
	size_t j;
	float r = 0.05f; //the search radius
	pp = p->GetPosition();
	
	ss1 = Vec3f(pp.x - r, pp.y -r, pp.z - r); //space search MIN dims
	ss2 = Vec3f(pp.x + r, pp.y + r, pp.z + r);//space search MAX dims
	pTree->KDTreeSearchSpace(aTree, &aIter, ss1.x, ss2.x, ss1.y, ss2.y, ss1.z, ss2.z);


	j = pTree->kdIterGetNext(aIter);

	while(j != KD_TREE_END){
		//do something here
		Particles.at(j).SetColour(Vec3f(1.0f, 1.0f, 1.0f));
		Neighbours.push_back(Particles.at(j));
		c++;
		j = pTree->kdIterGetNext(aIter);
	}
	p->SetColour(Vec3f(1.0f, 0.0f, 0.0f));
	printf("Neighbours? %i\n", c);

}

void ParticleSystem::UpdateNeighbours(){}

void ParticleSystem::UpdateAcceleration(){}

void ParticleSystem::UpdatePressure(){}

//Generate the particles up to the particle count
void ParticleSystem::GenParticles(){
	float PADDING		= 0.04f;	// particle scale * 2
	float PADDING_INIT	= 0.02f;	// particle scale
	
	xc = yc = zc = px = py = pz = 0.0f;
	float x, y, z;

	for(int i = 0; i < MAX_PARTICLES; i++){
		Particle temp; 
		if (xc < dim.max.x*2 - PADDING_INIT && zc == 0.0f && yc == 0.0f)
		{
			if(xc==0.0f)
				x = dim.min.x + PADDING_INIT;
			else
				x = px + PADDING;

			px = x;
			y = dim.max.y - PADDING;
			z = dim.min.z + PADDING;
			xc+=PADDING;
		}
		else{
				if(zc == 0.0f && yc == 0.0f){
					xc = 0.0f;
					zc += PADDING;
				}

				if(xc >= dim.max.x*2 - PADDING_INIT){
					zc +=PADDING;
					xc = 0.0f;
				}
				if(zc + PADDING >= dim.max.z*2 ){
					yc+=PADDING;
					zc = 0.0f;
					xc = 0.0f;
				}

				if(xc==0.0f)
					x = dim.min.x + PADDING_INIT;
				else
					x = px + PADDING;
				px = x;
				y = dim.max.y - yc - PADDING;
				z = dim.min.z + PADDING + zc;
				xc+=PADDING;
			}
	
		temp.GenParticle(Vec3f(x, y, z));
		temp.SetColour(0.0f, 0.0f, 1.0f);//-(float)i/1000);
		temp.SetModel(p_Model);

		i_Positions.push_back(glm::vec4(temp.GetPositionGLM(), 1.0f));
		i_Models.push_back(temp.GetMVM() );

		Particles.push_back(temp);
	}
	
}

//Check the postion of the particle + or - the radius is within
//the bounding box of the container. If the particle is outside
//then move it and check again until it is inside the bounds
void ParticleSystem::BoundaryCheck(Particle* p){
	Vec3f temp	= p->GetPosition();
	float r		= p->GetRadius();

	if( (temp.x + r/2 < dim.max.x &&
		 temp.y + r/2 < dim.max.y &&
		 temp.z + r/2 < dim.max.z)  &&
		(temp.x - r/2 > dim.min.x &&
		 temp.y - r/2 > dim.min.y &&
		 temp.z - r/2 > dim.min.z) ){
		 p->SetBounds(true);
		 
	}else{
		MovePFromBounds(p);
		BoundaryCheck(p);
	}
}

void ParticleSystem::MovePFromBounds(Particle* p){
	Vec3f temp	= p->GetPosition();
	float r		= p->GetRadius();
	float m		= r/2;

	if(temp.x + r > dim.max.x)
		p->SetPosition(Vec3f(temp.x - m, temp.y, temp.z));
	else if (temp.y + r*2 > dim.max.y)
		p->SetPosition(Vec3f(temp.x, temp.y - m, temp.z));
	else if (temp.z + r*2 > dim.max.z)
		p->SetPosition(Vec3f(temp.x, temp.y, temp.z - m));
	else if (temp.x - r*2 < dim.min.x)
		p->SetPosition(Vec3f(temp.x + m, temp.y, temp.z));
	else if (temp.y - r*2 < dim.min.y)
		p->SetPosition(Vec3f(temp.x, temp.y + m, temp.z));
	else if (temp.z - r*2 < dim.min.z)
		p->SetPosition(Vec3f(temp.x, temp.y, temp.z + m));
}

void ParticleSystem::Run(float delta){
	for(size_t i =0; i < Particles.size(); i++){
		BoundaryCheck(&Particles[i]);
		if(Particles[i].GetBoundCheck()){
			float gravChange = Particles[i].GetPosition().y;
			gravChange  = Particles[i].GetPosition().y - GRAVITY;
			Particles[i].SetPosition(Vec3f(Particles[i].GetPosition().x, gravChange, Particles[i].GetPosition().z));
			i_Positions.at(i) = glm::vec4(Particles[i].GetPositionGLM(), 1.0f);
			i_Models.at(i) = Particles[i].GetMVM();
		}
	}
}

void ParticleSystem::DoThisTing(){
	size_t r = rand() % MAX_PARTICLES;
	NeighbourSearch(&Particles.at(r));
}

void ParticleSystem::LoadModel(){
	p_Model = m_Loader.loadModel("models/sph.dae", true);
}

void ParticleSystem::renderIModel(size_t numInst, const glm::mat4* WVP, const glm::mat4* World){
	
}
