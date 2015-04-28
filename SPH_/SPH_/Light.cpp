#include "Light.h"

Light::Light(int t, Vec3f p, Vec3f i, float ac, float at){
	type		= t;
	l_Pos		= p;
	l_Int		= i;
	l_Att		= at;
	l_AmbCoef	= ac;
}
