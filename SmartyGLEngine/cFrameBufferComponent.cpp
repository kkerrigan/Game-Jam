/*
* Name:		cFrameBufferComponent.h
* Author:	Daniel Maclam
* Date		2019-02-24 9:05 AM
*/

#include "cFrameBufferComponent.h"
#include <iostream>

cFrameBufferComponent::cFrameBufferComponent() : cComponent(9), pFbo(nullptr)
{
}

cFrameBufferComponent::~cFrameBufferComponent()
{
}

void cFrameBufferComponent::inititFrameBuffer(int texNumber)
{
	this->mFboTextureUnit = texNumber;

	// Create a new cFBO object
	this->pFbo = new cFBO();

	// Try to initialize the FBO
	std::string errorString;
	if (this->pFbo->init(1920, 1080, errorString)) {

		std::cout << "Frame Buffer good to go!" << std::endl;
	}
	else {
		std::cout << "One of your frame buffers failed to initialize!" << std::endl;
	}

	return;
}

int cFrameBufferComponent::getFboTextureUnit() const
{
	return this->mFboTextureUnit;
}
