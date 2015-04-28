#ifndef Grid_h
#define Grid_h

#include <vector>

#include "Vec3f.h"
#include "ParticleSystem.h"
#include "Particle.h"

using namespace std;

struct Cell{
	Vec3f Min, Max;
};

class Grid{
private:
	unsigned int xDim, yDim, zDim;
	GLuint VBO, VAO, CCO, VNO;
	int c_Size, c_SizeSq;
	Vec3f Verts[50];
	Vec3f Norm[50];
	vector<Cell> Cells;
public:
	Grid();
	Grid(Vec3f, Vec3f, int, vector<Particle>&);
	~Grid();
	void GetNeighbourCells();
	void UpdateParticleLocation(Particle&);
	void MakeVAO();
	void Render();

	
};
#endif