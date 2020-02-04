#pragma once
#include "Global.h"
#include <string>

namespace dsm
{
	/*!	\class	cShadowFBO
		\brief  
		\author Daniel Maclam
		\date	2019-04-23 5:50 PM
		*/
	class cShadowFBO
	{
	public:

	GLuint id;
	GLuint depthTexture_ID;
	GLint width;
	GLint height;

	// Takes width and height and return ratio
	float calculate_aspect_ratio(void);

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);

	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);

    void clear_buffer();

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
	};
}
