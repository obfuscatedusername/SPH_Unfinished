#include "OctTree.h"

//Initialise the object and set all pertinent data.
OcTree::OcTree(const Vec3f& o, const Vec3f& hd){
	Origin	= o;
	hDim	= hd;
	Data	= NULL;
	for(size_t i = 0; i < 8; i++){
		Children[i] = NULL;
	}
}

OcTree::OcTree(const OcTree& c){
	Origin	= c.Origin;
	hDim	= c.hDim;
	Data	= c.Data;
}

OcTree::~OcTree(){
	for(size_t i = 0; i < 8; i ++){
		delete Children[i];
	}
}

int OcTree::GetOctantContainingPoint(const Vec3f& p){
	int oct = 0;
	if(p.x > Origin.x) oct |= 4;
	if(p.y > Origin.y) oct |= 2;
	if(p.z > Origin.z) oct |= 1;
	return oct;
}

bool OcTree::IsLeafNode(){
	return Children[0] == NULL;
}

void OcTree::Insert(OcPoint* ocp){

	if(IsLeafNode()){
		if(Data == NULL){
		Data = ocp;
		return;
	}else{
		OcPoint* oldPoint	= Data;
		Data				= NULL;
		for(size_t i = 0; i < 8; i++){
			Vec3f nOrigin	 = Origin;
			nOrigin.x		+= hDim.x * (i&4 ? .5f : -.5f);
			nOrigin.y		+= hDim.y * (i&2 ? .5f : -.5f);
			nOrigin.z		+= hDim.z * (i&1 ? .5f : -.5f);
			Children[i]		 = new OcTree(nOrigin, hDim * .5f);
		}

		Children[GetOctantContainingPoint(oldPoint->GetPos())]->Insert(oldPoint);
		Children[GetOctantContainingPoint(ocp->GetPos())]->Insert(ocp);
		}
	}else{
		int octant = GetOctantContainingPoint(ocp->GetPos());
		Children[octant]->Insert(ocp);
	}

}

void OcTree::GetPointsInCube(const Vec3f& bmin, const Vec3f& bmax, std::vector<OcPoint*>& results){

	if(IsLeafNode()){
		if(Data != NULL){
			const Vec3f& p = Data->GetPos();
			if(p.x > bmax.x || p.y > bmax.y || p.z > bmax.z) return;
			if(p.x < bmin.x || p.y < bmin.y || p.z < bmin.z) return;
			results.push_back(Data);
		}
	} else {
		for(size_t i = 0; i < 8; i++){

			Vec3f cmax = Children[i]->Origin + Children[i]->hDim;
			Vec3f cmin = Children[i]->Origin + Children[i]->hDim;

			if(cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z) continue;
			if(cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z) continue;
			
			Children[i]->GetPointsInCube(bmin, bmax, results);
		}
	}

}


void OcTree::SetPoints(size_t it, std::vector<Particle>* p){
	for(size_t i = 0; i < it; i++){
		OcPoint* temp = new OcPoint();
		temp->SetPos(p->at(i).GetPosition());
		temp->SetIDX(i);
		Insert(temp);
		delete temp;
	}
}