#ifndef _sVertex_xyz_rgba_n_uv2_bt_4Bones_HG_
#define _sVertex_xyz_rgba_n_uv2_bt_4Bones_HG_

// This represents the vertex as used in the vertex layout.
// It's also the vertex representation used in cMesh
// This matches the shader layout for the vertex

#include <cstring>		// for memset

static const unsigned int NUMBEROFBONES = 4;

struct sVertex_xyz_rgba_n_uv2_bt_4Bones {
	
	sVertex_xyz_rgba_n_uv2_bt_4Bones() :
		x(0.0f), y(0.0f), z(0.0f),w(0.0f),
		r(0.0f), g(0.0f), b(0.0f), a(1.0f),
		nx(0.0f), ny(0.0f), nz(0.0f), nw(0.0f),
		u0(0.0f), v0(0.0f), u1(0.0f), v1(0.0f),
		tx(0.0f), ty(0.0f), tz(0.0f), tw(0.0f),
		bx(0.0f), by(0.0f), bz(0.0f), bw(0.0f) {

//#ifdef _DEBUG
		memset(this->boneID, 0, sizeof(unsigned int) * NUMBEROFBONES);
		memset(this->boneWeights, 0, sizeof(float) * NUMBEROFBONES);
//#endif // DEBUG
	}	

	float x, y, z, w;								// 16
	float r, g, b, a;								// 32
	float nx, ny, nz, nw;							// 48
	float u0, v0, u1, v1;							// 60
	float tx, ty, tz, tw;	// tangent					
	float bx, by, bz, bw;	// bi-normal			
	
	// For the 4 bone skinned mesh information
	float boneID[NUMBEROFBONES]; 		
	float boneWeights[NUMBEROFBONES];
};

#endif
