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

//Generate the particles up to the particle count
void ParticleSystem::GenParticles(){
	for(size_t i = 0; i < MAX_PARTICLES; i++){
		Particle temp; 
		temp.GenParticle(Vec3f(0.0f, 0.0f, 0.0f));
		temp.SetColour(0.0f, 0.0f, 1.0f);
		temp.SetModel(p_Model);
		Particles.push_back(temp);
		i_Positions.push_back(glm::vec4(temp.GetPositionGLM(), 1.0f));
		i_Models.push_back(temp.GetMVM() );
		Vec3f t = temp.GetColour();
		i_Color.push_back(glm::vec3(t.x, t.y, t.z));
	}
	SetDistribution(inBB);
}

void ParticleSystem::SetDistribution(int type){
	float xmin = dim.min.x, xmax = dim.max.x, ymin = dim.min.y, ymax = dim.max.y, zmin = dim.min.z, zmax = dim.max.z;
	float PADDING		= 0.03f;	// particle scale * 2
	float PADDING_INIT	= 0.02f;	// particle scale
	if(type == 0){ //inBB
		
	size_t con = 0;
	float lx = (abs(xmax-xmin) - (PADDING_INIT*2)) / PADDING;
	float ly = (abs(ymax - ymin) - (PADDING_INIT *2 )) / PADDING;
	bool buttsechs = false;
	while(!buttsechs){
		for(size_t i = 0; i < ly; i ++){
			for(size_t j = 0; j < lx; j ++){
				for(size_t k = 0; k < lx; k ++){
					if(con >= Particles.size())
						buttsechs = true;
					else{
						float tx = PADDING * (k+1);
						float ty;

						if(i == 0)
							ty = PADDING+0.01;
						else
							ty = 0.01 + PADDING*(i+1);

						float tz = PADDING * (j+1);


						if(k == 0 && j != 0){
							//Particles[con].SetColour(Vec3f(0.0f, 1.0f, 0.0f));
							Particles[con].SetPosition(Vec3f(xmax-0.025, ymax - ty, zmin + tz));
						}else if(k == 0 && j == 0){
							Particles[con].SetPosition(Vec3f(xmax - tx, ymax - ty, zmin + tz));
							Particles[con].SetColour(Vec3f(1.0f, 0.0f, 0.0f));
							
						}else{
							if(k == lx-1)
								Particles[con].SetPosition(Vec3f(xmax - tx - 0.015, ymax - ty, zmin + tz));
							else
								Particles[con].SetPosition(Vec3f(xmax - tx, ymax - ty, zmin + tz));
						}
						
						con++;
					}
				}
			}
		}
	}
	}

	if(type == 1){ //DamBreak

	}

	if(type == 2){ //Random

	}

	UpdateVecs();
}

void ParticleSystem::BuildKD(){
	aTree = new kdtree();
	pTree->BuildKDTree(&Particles, Particles.size(), &aTree);
}

void ParticleSystem::NeighbourSearch(Particle* p){
	//aTree = new kdtree();
	//aTree = pTree->BuildKDTree(&Particles, Particles.size(), &aTree);

	std::vector<Particle> Neighbours;
	size_t c = 0;
	Vec3f ss1, ss2, pp;
	size_t j;
	float r = 0.03f; //the search radius
	pp = p->GetPosition();
	
	ss1 = Vec3f(pp.x - r, pp.y -r, pp.z - r); //space search MIN dims
	ss2 = Vec3f(pp.x + r, pp.y + r, pp.z + r);//space search MAX dims
	pTree->KDTreeSearchSpace(aTree, &aIter, ss1.x, ss2.x, ss1.y, ss2.y, ss1.z, ss2.z);
	j = pTree->kdIterGetNext(aIter);

	while(j != KD_TREE_END){
		//do something here
		//Particles.at(j).SetColour(Vec3f(1.0f, 1.0f, 1.0f));
		Neighbours.push_back(Particles.at(j));
		c++;
		j = pTree->kdIterGetNext(aIter);
	}
	
	printf(" Neighbours? %i\n", c);

}

//Same as neighbour search above but return the vector so that the neighbours can be dealt with
std::vector<Particle> ParticleSystem::NSVec(Particle* p){

	std::vector<Particle> Neighbours;
	if(Neighbours.size() > 1) //to make sure it's clear (so we don't use up loads of memory)
		Neighbours.clear();

	size_t c = 0;
	Vec3f ss1, ss2, pp;
	size_t j;
	float r = SEARCH_RADIUS; //the search radius
	pp = p->GetPosition();
	
	ss1 = Vec3f(pp.x - r, pp.y -r, pp.z - r); //space search MIN dims
	ss2 = Vec3f(pp.x + r, pp.y + r, pp.z + r);//space search MAX dims
	pTree->KDTreeSearchSpace(aTree, &aIter, ss1.x, ss2.x, ss1.y, ss2.y, ss1.z, ss2.z);


	j = pTree->kdIterGetNext(aIter);

	while(j != KD_TREE_END){
		//do something here
		//Particles.at(j).SetColour(Vec3f(1.0f, 1.0f, 1.0f));
		Neighbours.push_back(Particles.at(j));
		c++;
		j = pTree->kdIterGetNext(aIter);
	}
	return Neighbours;
}

void ParticleSystem::UpdateNeighbours(){}

void ParticleSystem::UpdateAcceleration(){}

void ParticleSystem::UpdatePressure(){}

void ParticleSystem::UpdateVecs(){
	for(size_t i = 0; i < MAX_PARTICLES; i++){
		i_Positions.at(i) = glm::vec4(Particles[i].GetPositionGLM(), 1.0f);
		i_Models.at(i) = Particles[i].GetMVM();
		i_Color.at(i) = Particles[i].GetColourGLM();
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
		p->SetBounds(false);
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

void ParticleSystem::PColCheck(Particle* p, std::vector<Particle>* pv){

	for(size_t i = 0; i < pv->size(); i ++){
		Vec3f coll = pv->at(i).GetPosition() - p->GetPosition();
		float dist = coll.Length();
		float radSum = pv->at(i).GetRadius() + p->GetRadius();
		if(dist < radSum){ //there's been a collision
			printf("collision\n");
		}
	}
}

void ParticleSystem::Run(float delta){
	//Rebuild the KDtree per frame to deal with any changes in Particles
	//BuildKD();
	//BuildOT();
	for(size_t i =0; i < Particles.size(); i++){
		//Check that the particle is in bounds
		BoundaryCheck(&Particles[i]);
		//If so, Continue
		if(Particles[i].GetBoundCheck()){
			//float gravChange = Particles[i].GetPosition().y;
			float gravChange  = Particles[i].GetPosition().y - GRAVITY * delta;
			Particles[i].SetPosition(Vec3f(Particles[i].GetPosition().x, gravChange, Particles[i].GetPosition().z));

		}
	}

	for(size_t i = 0; i < Particles.size(); i++){
			//get the neighbours of the particle i
			//std::vector<Particle>* tN = NSVec(&Particles[i]);
			//NeighbourSearch(&Particles[i]);
			//work out if there has been a collision
			//QueryOTNN(&Particles[i]);
	}

	UpdateVecs();
}

void ParticleSystem::DoThisTing(){
	size_t r = rand() % MAX_PARTICLES;
	NeighbourSearch(&Particles.at(r));
	UpdateVecs();
}

void ParticleSystem::LoadModel(){
	p_Model = m_Loader.loadModel("models/sphere.obj", true);
}

void ParticleSystem::BuildOT(){
	if(ot == NULL){
		ot = new OcTree(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
		ot->SetPoints(Particles.size(), &Particles);
	}else{
		ot = NULL;
		ot = new OcTree(Vec3f(0.0f, 0.0f, 0.0f), Vec3f(1.0f, 1.0f, 1.0f));
		ot->SetPoints(Particles.size(), &Particles);
	}


}

void ParticleSystem::QueryOTNN(Particle* p){
	Vec3f pp  = p->GetPosition();
	Vec3f min = Vec3f(pp.x - (-.2f), pp.y -(-.2f), pp.z - (-.2f));
	Vec3f max = Vec3f(pp.x + .2f, pp.y + .2f, pp.z +.2f);
	std::vector<OcPoint*> results;
	ot->GetPointsInCube(min, max, results);
	printf("found %i neighbours\n", results.size());

}