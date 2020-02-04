/*
* Name:		cShadowFBO.h
* Author:	Daniel Maclam
* Date		2019-04-23 5:50 PM
*/

#include "cShadowFBO.h"

float dsm::cShadowFBO::calculate_aspect_ratio()
{
	return this->width / (float)this->height;
}

bool dsm::cShadowFBO::init(int width, int height, std::string& error)
{
	this->width = width;
	this->height = height;

	glGenFramebuffers(1, &(this->id));

	glGenTextures(1, &(this->depthTexture_ID));
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);

	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_DEPTH_COMPONENT32F,
		this->width,
		this->height,
		0,
		GL_DEPTH_COMPONENT,
		GL_FLOAT,
		NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthTexture_ID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	bool bFrameBufferIsGoodToGo = true;

	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
	case GL_FRAMEBUFFER_COMPLETE:
		bFrameBufferIsGoodToGo = true;
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		bFrameBufferIsGoodToGo = false;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	case GL_FRAMEBUFFER_UNSUPPORTED:
	default:
		bFrameBufferIsGoodToGo = false;
		break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFrameBufferIsGoodToGo;
}

bool dsm::cShadowFBO::shutdown()
{
	glDeleteTextures(1, &(this->depthTexture_ID));
	glDeleteFramebuffers(1, &(this->id));

	return true;
}

bool dsm::cShadowFBO::reset(int width, int height, std::string& error)
{
	if (!this->shutdown())
	{
		error = "Could not shutdown";
		return false;
	}

	return this->init(width, height, error);
}

void dsm::cShadowFBO::clear_buffer()
{
	glViewport(0, 0, this->width, this->height);
    glClear(GL_DEPTH_BUFFER_BIT);
}
