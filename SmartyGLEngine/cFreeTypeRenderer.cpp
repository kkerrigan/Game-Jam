#include "cFreeTypeRenderer.h"
#include "cShaderManager.h"
#include "cEngine.h"

// c'tor
cFreeTypeRenderer::cFreeTypeRenderer(){
	
	this->sx = 2.0f / cEngine::getInstance()->get_window_settings()->width;
	this->sy = 2.0f / cEngine::getInstance()->get_window_settings()->height;
	this->yoffset = 30.0f;
	this->xoffset = 8 * sx;
	this->offset_step = 40.0f;
}

// d'tor
cFreeTypeRenderer::~cFreeTypeRenderer(){}

/*
*	getInstance()
*	This function contains the static variable for the Singleton.
*	Accepts: None
*	Returns: A pointer to the cFreeTypeRenderer class.
*/
cFreeTypeRenderer* cFreeTypeRenderer::getInstance() {
	static cFreeTypeRenderer instance;

	return &instance;
}

/*
*	initText()
*	This function initializes the Freetype text rendering.
*	Accepts: None
*	Returns: A bool representing whether the initialization was successful or not.
*/
bool cFreeTypeRenderer::initText(int fontSize) {

	if (FT_Init_FreeType(&this->ft)) {
		fprintf(stderr, "Unable to init text...\n");
		return false;
	}

	if (FT_New_Face(this->ft, "./assets/fonts/Anonymous.ttf", 0, &this->face)) {
		fprintf(stderr, "Unable to init font...\n");
		return false;
	}

	// Font size
	FT_Set_Pixel_Sizes(this->face, 0, fontSize);

	if (FT_Load_Char(this->face, 'X', FT_LOAD_RENDER)) {
		fprintf(stderr, "unable to load character\n");
		return false;
	}

	return true;
}


void cFreeTypeRenderer::getTextUniforms() {

	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("FreeTypeShader");

	this->uniform_tex = glGetUniformLocation(program, "tex");
	this->attribute_coord = glGetAttribLocation(program, "coord");
	this->uniform_color = glGetUniformLocation(program, "color");

	glGenBuffers(1, &this->dp_vbo);

	return;
}


void cFreeTypeRenderer::renderText(const char *text, float x, float y, float sx, float sy, glm::vec4 color) {

	cShaderManager::getInstance()->useShaderProgram("FreeTypeShader");
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform4f(this->uniform_color, color.r, color.g, color.b, color.a);

	const char* p;
	FT_GlyphSlot glyph = this->face->glyph;

	GLuint texture;
	glActiveTexture(10 + GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(this->uniform_tex, 10);

	// We require 1 byte alignment when uploading texture data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Clamping to edges is important to prevent artifacts when scaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Linear filtering usually looks best for text 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set up the VBO for our vertex data
	glEnableVertexAttribArray(this->attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, this->dp_vbo);
	glVertexAttribPointer(this->attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Loop through all characters 
	for (p = text; *p; p++) {
		
		// Try to load and render the character
		if (FT_Load_Char(this->face, *p, FT_LOAD_RENDER))
			continue;


		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			glyph->bitmap.width,
			glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			glyph->bitmap.buffer
		);

		// Calculate the vertex and texture coordinates 
		float x2 = x + glyph->bitmap_left * sx;
		float y2 = -y - glyph->bitmap_top * sy;
		float w = glyph->bitmap.width * sx;
		float h = glyph->bitmap.rows * sy;

		point box[4] = {
		{ x2, -y2, 0, 0 },
		{ x2 + w, -y2, 1, 0 },
		{ x2, -y2 - h, 0, 1 },
		{ x2 + w, -y2 - h, 1, 1 },
		};


		// Draw the character on the screen
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_TRUE);

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		// Advance the cursor to the start of the next character 
		x += (glyph->advance.x >> 6) * sx;
		y += (glyph->advance.y >> 6) * sy;
	}

	glDisable(GL_BLEND);
	
	glDisableVertexAttribArray(this->attribute_coord);
	glDeleteTextures(1, &texture);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	return;
}