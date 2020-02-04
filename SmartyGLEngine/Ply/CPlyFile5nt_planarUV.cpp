#include "CPlyFile5nt.h"

	
// Newer
void CPlyFile5nt::GenTextureCoordsLinear( enumTEXCOORDBIAS uBias, float scale )
{
	// Check to see if there's an invalid uBias
	switch ( uBias )
	{
	case enumTEXCOORDBIAS::PLANAR_XY:
	case enumTEXCOORDBIAS::PLANAR_XZ:
	case enumTEXCOORDBIAS::PLANAR_YZ:
	case enumTEXCOORDBIAS::PLANAR_ON_WIDEST_AXES:
		// It's OK...
		break;
	default:
		// Likely passed an invalid enum
		return;
		break;
	};

	// If it's PLANAR_ON_WIDEST_AXES, then determine the right option:
	if ( uBias == enumTEXCOORDBIAS::PLANAR_ON_WIDEST_AXES )
	{
		// Assume it's XZ. I mean, why not? 
		uBias = enumTEXCOORDBIAS::PLANAR_XZ;

		// Is is actually XY (so deltaZ is the smallest?)
		if ( ( this->getDeltaX() < this->getDeltaZ() ) && 
			 ( this->getDeltaY() < this->getDeltaZ() ) )
		{	
			uBias = enumTEXCOORDBIAS::PLANAR_XY;
		}
		// Or is it YZ? (so deltaX is the smallest?)
		else if ( ( this->getDeltaY() < this->getDeltaX() ) && 
				  ( this->getDeltaZ() < this->getDeltaX() ) )
		{
			uBias = enumTEXCOORDBIAS::PLANAR_YZ;
		}
	}//if(uBias==PLANAR_ON_WIDEST_AXES...

	for ( unsigned int vertIndex = 0; vertIndex != this->GetNumberOfVerticies(); vertIndex++ )
	{
		PlyVertex &vert = this->m_verticies[vertIndex];

		float u = 0.0f;
		float v = 0.0f;

		switch ( uBias )
		{
		case enumTEXCOORDBIAS::PLANAR_XY:
			u = ( vert.xyz.x - this->getMinX() ) / (this->getDeltaX() / scale);
			v = ( vert.xyz.y - this->getMinY() ) / (this->getDeltaY() / scale);
			break;
		case enumTEXCOORDBIAS::PLANAR_XZ:
			u = ( vert.xyz.x - this->getMinX() ) / (this->getDeltaX() / scale);
			v = ( vert.xyz.z - this->getMinZ() ) / (this->getDeltaZ() / scale);
			break;
		case enumTEXCOORDBIAS::PLANAR_YZ:
			u = ( vert.xyz.y - this->getMinY() ) / (this->getDeltaY() / scale);
			v = ( vert.xyz.z - this->getMinZ() ) / (this->getDeltaZ() / scale);
			break;
		};

		vert.tex0u = u;
		vert.tex0v = v;

	}//for(unsigned int vertIndex...

	return;
}
