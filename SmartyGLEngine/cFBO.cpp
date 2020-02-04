/*
* Name:		cFBO.h
* Author:	Daniel Maclam
* Date		2019-02-24 8:39 AM
*/

#include "cFBO.h"

cFBO::cFBO() : ID(0),
colourTexture_0_ID(0),
normalTexture_1_ID(1),
depthTexture_ID(0),
width(-1), height(-1) {}

bool cFBO::reset(int width, int height, std::string& error) {

	if (!this->shutdown()) {

		error = "Could not shutdown";
		return false;
	}

	return this->init(width, height, error);
}

bool cFBO::shutdown(void) {

	glDeleteTextures(1, &(this->colourTexture_0_ID));
	glDeleteTextures(1, &(this->normalTexture_1_ID));
	glDeleteTextures(1, &(this->vertexWorldPos_2_ID));
	glDeleteTextures(1, &(this->depthTexture_ID));

	glDeleteFramebuffers(1, &(this->ID));

	return true;
}


bool cFBO::init(int width, int height, std::string& error) {

	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &(this->ID));
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

	//************************************************************
		// Create the colour buffer (texture)
	glGenTextures(1, &(this->colourTexture_0_ID));
	glBindTexture(GL_TEXTURE_2D, this->colourTexture_0_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,	// 8 bits per colour
		this->width,
		this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//***************************************************************

	//************************************************************
	// Create the NORMAL buffer (texture)
	glGenTextures(1, &(this->normalTexture_1_ID));
	glBindTexture(GL_TEXTURE_2D, this->normalTexture_1_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,	// 32 bits per "colour"
		this->width,
		this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//***************************************************************

	//************************************************************
	// Create the Vertex World position buffer (texture)
	glGenTextures(1, &(this->vertexWorldPos_2_ID));
	glBindTexture(GL_TEXTURE_2D, this->vertexWorldPos_2_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,
		this->width,
		this->height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//***************************************************************


		// Create the depth buffer (texture)
	glGenTextures(1, &(this->depthTexture_ID));
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);

	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, ]

	// Note that, unless you specifically ask for it, the stencil buffer
	// is NOT present... i.e. GL_DEPTH_COMPONENT32F DOESN'T have stencil

	// These are:
	// - GL_DEPTH32F_STENCIL8, which is 32 bit float depth + 8 bit stencil
	// - GL_DEPTH24_STENCIL8,  which is 24 bit float depth + 8 bit stencil (more common?)
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8,
		this->width,
		this->height);

	//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT );
//	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_COMPONENT );
//	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0, GL_EXT_packe

//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH24_STENCIL8, GL_TEXTURE_2D, this->depthTexture_ID, 0);

// ***************************************************************

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,			// Colour goes to #0
		this->colourTexture_0_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT1,			// Normal goes to #1
		this->normalTexture_1_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT2,			// Vertex world position #2
		this->vertexWorldPos_2_ID, 0);

	//	glFramebufferTexture(GL_FRAMEBUFFER,
	//						 GL_DEPTH_ATTACHMENT,
	//						 this->depthTexture_ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		this->depthTexture_ID, 0);

	static const GLenum draw_bufers[] =
	{
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
	};
	glDrawBuffers(3, draw_bufers);

	// ***************************************************************

		// ADD ONE MORE THING...
	bool bFrameBufferIsGoodToGo = true;

	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
	case GL_FRAMEBUFFER_COMPLETE:
		bFrameBufferIsGoodToGo = true;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		bFrameBufferIsGoodToGo = false;
		break;
		//	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	case GL_FRAMEBUFFER_UNSUPPORTED:
	default:
		bFrameBufferIsGoodToGo = false;
		break;
	}

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFrameBufferIsGoodToGo;
}

void cFBO::clearColourBuffer(int bufferindex) {

	glViewport(0, 0, this->width, this->height);

	GLfloat	zero = 0.0f;
	glClearBufferfv(GL_COLOR, bufferindex, &zero);

	return;
}


void cFBO::clearBuffers(bool bClearColour, bool bClearDepth) {

	glViewport(0, 0, this->width, this->height);

	GLfloat	zero = 0.0f;
	GLfloat one = 1.0f;
	if (bClearColour) {

		glClearBufferfv(GL_COLOR, 0, &zero);
		glClearBufferfv(GL_COLOR, 1, &zero);
		glClearBufferfv(GL_COLOR, 2, &zero);
	}

	if (bClearDepth) {

		glClearBufferfv(GL_DEPTH, 0, &one);		// Depth is normalized 0.0 to 1.0f
	}

	// If buffer is GL_STENCIL, drawbuffer must be zero, and value points to a 
	//  single value to clear the stencil buffer to. Masking is performed in the 
	//  same fashion as for glClearStencil. Only the *iv forms of these commands 
	//  should be used to clear stencil buffers; be used to clear stencil buffers; 
	//  other forms do not accept a buffer of GL_STENCIL.
	glStencilMask(0xFF);

	{	// Clear stencil
		//GLint intZero = 0;
		//glClearBufferiv(GL_STENCIL, 0, &intZero );
		glClearBufferfi(GL_DEPTH_STENCIL,
			0,		// Must be zero
			1.0f,	// Clear value for depth
			0);	// Clear value for stencil
	}

	return;
}


int cFBO::getMaxColourAttachments(void) {

	int maxColourAttach = 0;
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColourAttach);

	return maxColourAttach;
}

int cFBO::getMaxDrawBuffers(void) {

	int maxDrawBuffers = 0;
	glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxDrawBuffers);

	return maxDrawBuffers;
}