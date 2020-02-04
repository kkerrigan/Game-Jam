#ifndef _sPlyVertex_HG_
#define _sPlyVertex_HG_

struct sPlyVertex 
{
//	sPlyVertex() : x(0.0f), y(0.0f), z(0.0f) {};
	float x;
	float y;
	float z;
	// Now has normals (this is FROM THE FILE)
	float nx;
	float ny;
	float nz;

	float u, v;
};

#endif 
