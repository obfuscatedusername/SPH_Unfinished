#ifndef Light_h
#define Light_h
#include "Vec3f.h"

#define MAX_LIGHTS 10

enum l_Type{
	AMBIENT,
	DIRECTIONAL,
	SPOTLIGHT
};

class Light{
private:
	Vec3f l_Pos, l_Int;
	float l_Att, l_AmbCoef;
	int type;
public:
	Light(int, Vec3f, Vec3f, float, float);
	~Light();
	
	//	Getters
	Vec3f GetPos(){return l_Pos;}
	Vec3f GetInt(){return l_Int;}
	float GetAtt(){return l_Att;}
	float GetAC(){return l_AmbCoef;}
	//	Setters
	void SetPos(Vec3f p){l_Pos = p;}
	void SetInt(Vec3f i){l_Int = i;}
	void SetAtt(float a){l_Att = a;}
	void SetAC(float ac){l_AmbCoef = ac;}
};
#endif