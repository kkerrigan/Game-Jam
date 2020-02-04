#pragma once
#include "cComponent.h"
#include "cFBO.h"

/*!	\class	cFrameBufferComponent
		\brief
		\author Daniel Maclam
		\date	2019-02-24 9:05 AM
		*/
class cFrameBufferComponent : public cComponent
{
public:
	cFrameBufferComponent();
	virtual ~cFrameBufferComponent();

	cFBO* pFbo;

	void inititFrameBuffer(int texNumber);
	int getFboTextureUnit() const;

private:
	int mFboTextureUnit;
};

