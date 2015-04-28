#include "Grid.h"

Grid::Grid(){

}

Grid::Grid(Vec3f bb_Min, Vec3f bb_Max, int h, vector<Particle>& p){

	xDim		= (unsigned)(bb_Max.x - bb_Min.x) / h;
	yDim		= (unsigned)(bb_Max.y - bb_Min.y) / h;
	zDim		= (unsigned)(bb_Max.z - bb_Min.z) / h;
	c_Size		= h;
	c_SizeSq	= h*h;

}

Grid::~Grid(){

}

void Grid::MakeVAO(){

}

void Grid::Render(){

}

void Grid::GetNeighbourCells(){

}