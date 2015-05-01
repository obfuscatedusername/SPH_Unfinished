#ifndef OcTree_h
#define OcTree_h
#include "Vec3f.h"
#include "Particle.h"
#include <vector>

struct OcPoint{
	Vec3f Pos;
	size_t idx;
	Vec3f GetPos(){return Pos;}
	void SetPos(Vec3f& p){Pos = p;}
	void SetIDX(size_t& i){idx = i;}
};

class OcTree{
private: 
	Vec3f Origin, hDim;
	OcTree* Children[8];
	OcPoint* Data;
public:
	OcTree(const Vec3f&, const Vec3f&);
	OcTree(const OcTree&);
	~OcTree();	
	int GetOctantContainingPoint(const Vec3f&);
	bool IsLeafNode();
	void Insert(OcPoint*);
	void GetPointsInCube(const Vec3f&, const Vec3f&, std::vector<OcPoint*>&);
	void SetPoints(size_t, std::vector<Particle>*);
};
#endif