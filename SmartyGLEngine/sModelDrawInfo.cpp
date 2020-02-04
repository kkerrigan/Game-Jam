#include "sModelDrawInfo.h"

sModelDrawInfo::sModelDrawInfo()
{
	this->current_state = prepared_to_read_file;
	this->VAO_ID = 0;

	this->vertexBufferID = 0;
	this->vertexBuffer_start_index = 0;
	this->numberOfVertices = 0;

	this->indexBufferID = 0;
	this->indexBuffer_start_index = 0;
	this->numberOfIndices = 0;
	this->numberOfTriangles = 0;

	// These come from the file
	this->pVerticesFromFile = NULL;
	this->pTriangles = NULL;	
	// These are copied to the GPU
	this->pVerticesToVBO = NULL;
	this->pIndices = NULL;

	this->maxX = this->maxY = this->maxZ = 0.0f;
	this->minX = this->minY = this->minZ = 0.0f;
	this->extentX = this->extentY = this->extentZ = 0.0f;
	this->maxExtent = 0.0f;

	// For basic dynamic drawing of the vertex buffer
	this->bVertexBufferIsDynamic = false;	// = false by default
	//	false = GL_DRAW_STATIC;
	//  true = GL_DRAW_DYNAIMIC;

	bool bIsIndexedModel = true;	// true by default
	// true: glDrawElements() on DrawObject()
	// false: glDrawArrays() on DrawObject()




	return;
}
