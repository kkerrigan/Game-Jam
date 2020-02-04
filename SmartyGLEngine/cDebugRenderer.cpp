#include "cDebugRenderer.h"

#include "Global.h"
#include "COpenGLError.h"
#include "cShaderManager.h"

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <sstream>		
#include <fstream>

// Used to hold the default sphere shape
extern float default_sphere_array [];
extern unsigned int default_sphere_array_size;

/*static*/ 
const std::string cDebugRenderer::DEFALUT_VERT_SHADER_SOURCE = "\
        #version 420                                    \n \
		                                                \n \
        uniform mat4 mModel;                            \n \
        uniform mat4 mView;                             \n \
        uniform mat4 mProjection;                       \n \
		                                                \n \
        in vec4 vPosition;                              \n \
        in vec4 vColour;                                \n \
		                                                \n \
        out vec4 gVertColour;                           \n \
		                                                \n \
        void main()                                     \n \
        {                                               \n \
            gl_Position = vPosition;                    \n \
		                                                \n \
            gVertColour = vColour;                      \n \
        }\n";

///*static*/ 
//const std::string cDebugRenderer::DEFALUT_VERT_SHADER_SOURCE = "\
//        #version 420                                    \n \
//		                                                \n \
//        uniform mat4 mModel;                            \n \
//        uniform mat4 mView;                             \n \
//        uniform mat4 mProjection;                       \n \
//		                                                \n \
//        in vec4 vPosition;                              \n \
//        in vec4 vColour;                                \n \
//		                                                \n \
//        out vec4 gVertColour;                           \n \
//		                                                \n \
//        void main()                                     \n \
//        {                                               \n \
//            mat4 MVP = mProjection * mView * mModel;    \n \
//            gl_Position = MVP * vPosition;              \n \
//		                                                \n \
//            vertColour = vColour;                       \n \
//        }\n";

// Default geometry shader is a pass through
const std::string cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_LINES = "\
        #version 400                                     \n \
                                                         \n \
        layout(lines)               in;                  \n \
        layout(line_strip)          out;                 \n \
        layout(max_vertices = 2)    out;		         \n \
	                                                     \n \
	    in vec4 gVertColour[];                           \n \
        out vec4 fVertColour;                            \n \
                                                         \n \
        uniform mat4 mModel;                             \n \
        uniform mat4 mView;                              \n \
        uniform mat4 mProjection;                        \n \
                                                         \n \
        void main()                                      \n \
        {                                                \n \
           mat4 MVP = mProjection * mView * mModel;      \n \
                                                         \n \
           gl_Position = MVP * gl_in[0].gl_Position;     \n \
           fVertColour = gVertColour[0];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           gl_Position = MVP * gl_in[1].gl_Position;     \n \
           fVertColour = gVertColour[1];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           EndPrimitive();                               \n \
        } \n";

// Default geometry shader is a pass through
const std::string cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_TRIANGLES = "\
        #version 400                                     \n \
                                                         \n \
        layout(triangles)               in;              \n \
        layout(triangle_strip)          out;             \n \
        layout(max_vertices = 3)        out;             \n \
	                                                     \n \
	    in vec4 gVertColour[];                           \n \
        out vec4 fVertColour;                            \n \
                                                         \n \
        uniform mat4 mModel;                             \n \
        uniform mat4 mView;                              \n \
        uniform mat4 mProjection;                        \n \
                                                         \n \
        void main()                                      \n \
        {                                                \n \
           mat4 MVP = mProjection * mView * mModel;      \n \
                                                         \n \
           gl_Position = MVP * gl_in[0].gl_Position;     \n \
           fVertColour = gVertColour[0];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           gl_Position = MVP * gl_in[1].gl_Position;     \n \
           fVertColour = gVertColour[1];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           gl_Position = MVP * gl_in[2].gl_Position;     \n \
           fVertColour = gVertColour[2];                 \n \
           EmitVertex();                                 \n \
                                                         \n \
           EndPrimitive();                               \n \
        } \n";

/*static*/ 
const std::string cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE = "\
        #version 420                                \n \
                                                    \n \
        in vec4 fVertColour;                        \n \
                                                    \n \
	    out vec4 outputColour;                      \n \
                                                    \n \
        void main()                                 \n \
        {                                           \n \
            outputColour.rgb = fVertColour.rgb;     \n \
            outputColour.a = fVertColour.a;         \n \
        }\n	";

///*static*/ 
//const std::string cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE = "\
//        #version 420                                \n \
//                                                    \n \
//        in vec4 vertColour;                         \n \
//                                                    \n \
//        void main()                                 \n \
//        {                                           \n \
//            gl_FragColor.rgb = vertColour.rgb;      \n \
//            gl_FragColor.a = vertColour.a;          \n \
//        }\n	";



class cDebugRenderer::cShaderProgramInfo
{
public:
	cShaderProgramInfo() :
		shaderProgramID(0), 
		matProjectionUniformLoc(-1), 
		matViewUniformLoc(-1),
		matModelUniformLoc(-1)
	{};
	cShaderManager::cShader vertShader;
	cShaderManager::cShader geomShader;
	cShaderManager::cShader fragShader;
	unsigned int shaderProgramID;
	// Uniforms in the shader
	int matProjectionUniformLoc;
	int matViewUniformLoc;
	int matModelUniformLoc;
};

#include <iostream>


bool cDebugRenderer::initialize(void)
{
	// Create our own, local, shader manager:
	// (this is deleted once it's used, but that's OK since we only 
	//  need it to get the shader 'name' or ID)
	cShaderManager* shaderManager = cShaderManager::getInstance();

//    _    _            ___ _            _         
//   | |  (_)_ _  ___  / __| |_  __ _ __| |___ _ _ 
//   | |__| | ' \/ -_) \__ \ ' \/ _` / _` / -_) '_|
//   |____|_|_||_\___| |___/_||_\__,_\__,_\___|_|  
//                                                 
	this->m_pShaderProg_Lines->vertShader.parseStringIntoMultiLine(this->m_vertexShaderSource);
	this->m_pShaderProg_Lines->geomShader.parseStringIntoMultiLine(this->m_geometryShaderSource_Lines);
	this->m_pShaderProg_Lines->fragShader.parseStringIntoMultiLine(this->m_fragmentShaderSource);

	if ( ! shaderManager->createProgramFromSource( "debugShaderLines", 
	                                              this->m_pShaderProg_Lines->vertShader, 
		                                          this->m_pShaderProg_Lines->geomShader,
	                                              this->m_pShaderProg_Lines->fragShader ) )
	{
		this->m_lastError = "Could not create the debug shader program.\n" + shaderManager->getLastError();
		return false;
	}

	//shaderManager.setBasePath( "assets/shaders/" );
	//this->m_pShaderProg->vertShader.fileName = "debugVert.glsl";
	//this->m_pShaderProg->geomShader.fileName = "debugGeom.glsl";
	//this->m_pShaderProg->fragShader.fileName = "debugFrag.glsl";
//
    //if ( ! shaderManager.createProgramFromFile( "debugShader", 
	//											this->m_pShaderProg->vertShader,
	//											this->m_pShaderProg->geomShader,
	//											this->m_pShaderProg->fragShader) )
	//{
	//	error = "Could not create the debug shader program.\n" + shaderManager.getLastError();
	//	this->m_lastError = error;
	//	return false;
	//}

	// The shader was compiled, so get the shader program number
	this->m_pShaderProg_Lines->shaderProgramID = shaderManager->getIDFromFriendlyName("debugShaderLines");

	// Get the uniform variable locations 
	glUseProgram(this->m_pShaderProg_Lines->shaderProgramID);
	this->m_pShaderProg_Lines->matModelUniformLoc = glGetUniformLocation(this->m_pShaderProg_Lines->shaderProgramID, "mModel");
	this->m_pShaderProg_Lines->matViewUniformLoc = glGetUniformLocation(this->m_pShaderProg_Lines->shaderProgramID, "mView");
	this->m_pShaderProg_Lines->matProjectionUniformLoc = glGetUniformLocation(this->m_pShaderProg_Lines->shaderProgramID, "mProjection");

	std::cout << "Lines::mModel = " << this->m_pShaderProg_Lines->matModelUniformLoc << std::endl;
	std::cout << "Lines::mView = " << this->m_pShaderProg_Lines->matViewUniformLoc << std::endl;
	std::cout << "Lines::mProjection = " << this->m_pShaderProg_Lines->matProjectionUniformLoc << std::endl;

	glUseProgram(0);

//    _____    _                _       ___ _            _         
//   |_   _| _(_)__ _ _ _  __ _| |___  / __| |_  __ _ __| |___ _ _ 
//     | || '_| / _` | ' \/ _` | / -_) \__ \ ' \/ _` / _` / -_) '_|
//     |_||_| |_\__,_|_||_\__, |_\___| |___/_||_\__,_\__,_\___|_|  
//                        |___/                                    
	this->m_pShaderProg_Triangles->vertShader.parseStringIntoMultiLine(this->m_vertexShaderSource);
	this->m_pShaderProg_Triangles->geomShader.parseStringIntoMultiLine(this->m_geometryShaderSource_Triangles);
	this->m_pShaderProg_Triangles->fragShader.parseStringIntoMultiLine(this->m_fragmentShaderSource);

	if ( ! shaderManager->createProgramFromSource( "debugShaderTriangles",
	                                              this->m_pShaderProg_Triangles->vertShader, 
		                                          this->m_pShaderProg_Triangles->geomShader,
	                                              this->m_pShaderProg_Triangles->fragShader ) )
	{
		this->m_lastError = "Could not create the debug shader program.\n" + shaderManager->getLastError();
		return false;
	}

	// The shader was compiled, so get the shader program number
	this->m_pShaderProg_Triangles->shaderProgramID = shaderManager->getIDFromFriendlyName("debugShaderTriangles");

	// Get the uniform variable locations 
	glUseProgram(this->m_pShaderProg_Triangles->shaderProgramID);
	this->m_pShaderProg_Triangles->matModelUniformLoc = glGetUniformLocation(this->m_pShaderProg_Triangles->shaderProgramID, "mModel");
	this->m_pShaderProg_Triangles->matViewUniformLoc = glGetUniformLocation(this->m_pShaderProg_Triangles->shaderProgramID, "mView");
	this->m_pShaderProg_Triangles->matProjectionUniformLoc = glGetUniformLocation(this->m_pShaderProg_Triangles->shaderProgramID, "mProjection");

	std::cout << "Triangles::mModel = " << this->m_pShaderProg_Triangles->matModelUniformLoc << std::endl;
	std::cout << "Triangles::mView = " << this->m_pShaderProg_Triangles->matViewUniformLoc << std::endl;
	std::cout << "Triangles::mProjection = " << this->m_pShaderProg_Triangles->matProjectionUniformLoc << std::endl;

	glUseProgram(0);


	// Set up the VBOs...
	if ( ! this->resizeBufferForTriangles(cDebugRenderer::DEFAULTNUMBEROFTRIANGLES) )
	{
		return false;
	}
	if ( ! this->resizeBufferForLines(cDebugRenderer::DEFAULTNUMBEROFLINES) )
	{
		return false;
	}
	if ( ! this->resizeBufferForPoints(cDebugRenderer::DEFAULTNUMBEROFPOINTS) )
	{
		return false;
	}
	if ( ! this->resizeBufferForMeshes(cDebugRenderer::DEFAULT_MESHBUFFERSIZE_PADDING_RATIO) )
	{
		return false;
	}
	
	// Load the default sphere
	std::vector<sDebugTri> vecSphereTris;
//	this->QnD_loadHeaderArrayInto_vecTri( default_sphere_array, default_sphere_array_size, vecSphereTris);
	this->generateIsoSphere( 1, 1.0f,  vecSphereTris );
	this->loadDebugMesh( iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME, vecSphereTris );
	this->loadDebugMesh("sphere", vecSphereTris);


	return true;
}

bool cDebugRenderer::resizeBufferForPoints(unsigned int newNumberOfPoints)
{
	//TODO
	return true;
}

//static
const float cDebugRenderer::DEFAULT_MESHBUFFERSIZE_PADDING_RATIO = 1.10f; //or 10 %

bool cDebugRenderer::resizeBufferForMeshes(float meshBufferPaddingRatio /*=DEFAULT_MESHBUFFERSIZE_PADDING_RATIO*/)
{
	//TODO


	return true;
}


bool cDebugRenderer::resizeBufferForLines(unsigned int newNumberOfLines)
{
	// Erase any exisiting buffers 
	if ( this->m_VAOBufferInfoLines.bIsValid )
	{	// Assume it exists, so delete it
		delete [] this->m_VAOBufferInfoLines.pLocalVertexArray;

		glDeleteBuffers(1, &(this->m_VAOBufferInfoLines.vertex_buffer_ID) );

		glDeleteVertexArrays( 1, &(this->m_VAOBufferInfoLines.VAO_ID) );
	}//if...

	// Add a buffer of 10% to the size, because I'm a pessimist...
	// (Written this way to avoid a type conversion warning)
	newNumberOfLines = (unsigned int)(newNumberOfLines * 1.1);	

	this->m_VAOBufferInfoLines.bufferSizeObjects = newNumberOfLines;
	this->m_VAOBufferInfoLines.bufferSizeVertices = newNumberOfLines * 2;
	this->m_VAOBufferInfoLines.bufferSizeBytes = 0;
	this->m_VAOBufferInfoLines.numberOfObjectsToDraw = 0;
	this->m_VAOBufferInfoLines.numberOfVerticesToDraw = 0;
	this->m_VAOBufferInfoLines.shaderID = this->m_pShaderProg_Lines->shaderProgramID;
	return this->m_InitBuffer(this->m_VAOBufferInfoLines);

	return true;
}

bool cDebugRenderer::resizeBufferForTriangles(unsigned int newNumberOfTriangles)
{
	// Erase any exisiting buffers 
	if ( this->m_VAOBufferInfoTriangles.bIsValid )
	{	// Assume it exists, so delete it
		delete [] this->m_VAOBufferInfoTriangles.pLocalVertexArray;

		glDeleteBuffers(1, &(this->m_VAOBufferInfoTriangles.vertex_buffer_ID) );

		glDeleteVertexArrays( 1, &(this->m_VAOBufferInfoTriangles.VAO_ID) );
	}//if...

	// Add a buffer of 10% to the size, because I'm a pessimist...
	// (Written this way to avoid a type conversion warning)
	newNumberOfTriangles = (unsigned int)(newNumberOfTriangles * 1.1);	

	this->m_VAOBufferInfoTriangles.bufferSizeObjects = newNumberOfTriangles;
	this->m_VAOBufferInfoTriangles.bufferSizeVertices = newNumberOfTriangles * 3;
	this->m_VAOBufferInfoTriangles.bufferSizeBytes = 0;
	this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw = 0;
	this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw = 0;
	this->m_VAOBufferInfoTriangles.shaderID = this->m_pShaderProg_Triangles->shaderProgramID;
	return this->m_InitBuffer(this->m_VAOBufferInfoTriangles);
}

bool cDebugRenderer::m_InitBuffer(sVAOInfoDebug &VAOInfo)
{
//	glUseProgram(this->m_pShaderProg->shaderProgramID);
	glUseProgram(VAOInfo.shaderID);

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays( 1, &(VAOInfo.VAO_ID) );
	glBindVertexArray( VAOInfo.VAO_ID );


    glGenBuffers(1, &(VAOInfo.vertex_buffer_ID) );
    glBindBuffer(GL_ARRAY_BUFFER, VAOInfo.vertex_buffer_ID);

	VAOInfo.pLocalVertexArray = new sVertex_xyzw_rgba[VAOInfo.bufferSizeVertices];
	
	// Clear buffer
	VAOInfo.bufferSizeBytes = sizeof(sVertex_xyzw_rgba) * VAOInfo.bufferSizeVertices;
	memset(VAOInfo.pLocalVertexArray, 0, VAOInfo.bufferSizeBytes);

	// Copy the local vertex array into the GPUs memory
    glBufferData(GL_ARRAY_BUFFER, 
				 VAOInfo.bufferSizeBytes,		
				 VAOInfo.pLocalVertexArray,
				 GL_DYNAMIC_DRAW);

	// **DON'T** Get rid of local vertex array!
	// (We will need to copy the debug objects into this later!!)
	//delete [] pVertices;

	// Now set up the vertex layout (for this shader):
	//
    //	in vec4 vPosition;                           
    //	in vec4 vColour;                               
	//
	GLuint vpos_location = glGetAttribLocation(VAOInfo.shaderID, "vPosition");		// program, "vPos");	// 6
    GLuint vcol_location = glGetAttribLocation(VAOInfo.shaderID, "vColour");		// program, "vCol");	// 13

	// Size of the vertex we are using in the array.
	// This is called the "stride" of the vertices in the vertex buffer
	const unsigned int VERTEX_SIZE_OR_STRIDE_IN_BYTES = sizeof(sVertex_xyzw_rgba);

    glEnableVertexAttribArray(vpos_location);
	const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof(sVertex_xyzw_rgba, x );
    glVertexAttribPointer(vpos_location, 
						  4,					//	in vec4 vPosition; 	
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X_IN_CVERTEX)) );	// 64-bit


    glEnableVertexAttribArray(vcol_location);
	const unsigned int OFFSET_TO_R_IN_CVERTEX = offsetof(sVertex_xyzw_rgba, r );
    glVertexAttribPointer(vcol_location, 
						  4,					//	in vec4 vColour; 
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,		
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R_IN_CVERTEX)) );	// 64-bit
	// *******************************************************************

	VAOInfo.bIsValid = true;

	// CRITICAL 
	// Unbind the VAO first!!!!
	glBindVertexArray( 0 );	// 

	// Unbind (release) everything
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
//	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vpos_location);

	glUseProgram(0);

	return true;
}

bool cDebugRenderer::IsOK(void)
{
	//TODO
	return true;
}


cDebugRenderer::cDebugRenderer()
{
	this->m_vertexShaderSource = cDebugRenderer::DEFALUT_VERT_SHADER_SOURCE;
	this->m_fragmentShaderSource = cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE;

	this->m_geometryShaderSource_Lines = cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_LINES;
	this->m_geometryShaderSource_Triangles = cDebugRenderer::DEFAULT_GEOM_SHADER_SOURCE_TRIANGLES;

	this->m_pShaderProg_Lines = new cShaderProgramInfo();
	this->m_pShaderProg_Triangles = new cShaderProgramInfo();
	return;
}

cDebugRenderer::~cDebugRenderer()
{
	if (this->m_pShaderProg_Lines)
	{
		delete this->m_pShaderProg_Lines;
	}

	if (this->m_pShaderProg_Triangles)
	{
		delete this->m_pShaderProg_Triangles;
	}
	return;
}


void cDebugRenderer::RenderDebugObjects(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	this->m_copyTrianglesIntoRenderBuffer(deltaTime);
	this->m_RenderDebugTriangles(matCameraView, matProjection, deltaTime);

	this->m_copyLinesIntoRenderBuffer(deltaTime);
	this->m_RenderDebugLines(matCameraView, matProjection, deltaTime);

	this->m_copyPointsIntoRenderBuffer(deltaTime);
	this->m_RenderDebugPoints(matCameraView, matProjection, deltaTime);

	return;
}


void cDebugRenderer::m_RenderDebugTriangles(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	// Start rendering 
	glUseProgram(this->m_pShaderProg_Triangles->shaderProgramID);

	glUniformMatrix4fv( this->m_pShaderProg_Triangles->matViewUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matCameraView) );
	glUniformMatrix4fv( this->m_pShaderProg_Triangles->matProjectionUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matProjection) );
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv( this->m_pShaderProg_Triangles->matModelUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(glm::mat4(1.0f)) );
	

	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(50.0f);

	// Draw triangles
	glBindVertexArray( this->m_VAOBufferInfoTriangles.VAO_ID );
	glDrawArrays( GL_TRIANGLES, 
				  0,		// 1st vertex
				  this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw );
	glBindVertexArray( 0 );

	glUseProgram(0);

	// Put everything back as it was 
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// Default
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);		

	return;
}

void cDebugRenderer::m_RenderDebugLines(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	// Start rendering 
	glUseProgram(this->m_pShaderProg_Lines->shaderProgramID);

	glUniformMatrix4fv( this->m_pShaderProg_Lines->matViewUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matCameraView) );
	glUniformMatrix4fv( this->m_pShaderProg_Lines->matProjectionUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matProjection) );
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv( this->m_pShaderProg_Lines->matModelUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(glm::mat4(1.0f)) );
	

	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer

	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(50.0f);

	// Draw Lines
	glBindVertexArray( this->m_VAOBufferInfoLines.VAO_ID );
	glDrawArrays( GL_LINES, 
				  0,		// 1st vertex
				  this->m_VAOBufferInfoLines.numberOfVerticesToDraw );

	glBindVertexArray( 0 );

	glUseProgram(0);

	// Put everything back as it was 
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// Default
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);		

	return;
}

void cDebugRenderer::m_RenderDebugMeshes(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime) {

}

void cDebugRenderer::m_RenderDebugPoints(glm::mat4 matCameraView, glm::mat4 matProjection, double deltaTime)
{
	// TODO:
	
	return;
}


void cDebugRenderer::m_copyTrianglesIntoRenderBuffer(double deltaTime)
{
	// Used to keep the "persistent" ones...
	std::vector<drTri> vecTriTemp;

	this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw = (unsigned int)this->m_vecTriangles.size();

	// Is the draw buffer big enough? 
	if ( this->m_VAOBufferInfoTriangles.bufferSizeObjects < this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw )
	{
		// Resize the buffer
		this->resizeBufferForTriangles( this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw );
	}

	this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw
		= this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw * 3;	// Triangles

	unsigned int vertexIndex = 0;	// index of the vertex buffer to copy into 
	unsigned int triIndex = 0;		// index of the triangle buffer
	for (; triIndex != this->m_VAOBufferInfoTriangles.numberOfObjectsToDraw; 
		   triIndex++, vertexIndex += 3)
	{
		drTri& curTri = this->m_vecTriangles[triIndex];
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].x = curTri.v[0].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].y = curTri.v[0].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].z = curTri.v[0].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+0].a = 1.0f;

		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].x = curTri.v[1].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].y = curTri.v[1].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].z = curTri.v[1].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+1].a = 1.0f;

		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].x = curTri.v[2].x;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].y = curTri.v[2].y;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].z = curTri.v[2].z;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].w = 1.0f;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].r = curTri.colour.r;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].g = curTri.colour.g;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].b = curTri.colour.b;
		this->m_VAOBufferInfoTriangles.pLocalVertexArray[vertexIndex+2].a = 1.0f;

		curTri.lifeTime -= static_cast<float>(deltaTime);

		// Keep this one? (i.e. is persistent?)
		if (curTri.lifeTime > 0.0f)
		{
			vecTriTemp.push_back(curTri);
		}
	}//for (; 


	// Clear the triangle list and push back the persistent ones
	this->m_vecTriangles.clear();
	for (std::vector<drTri>::iterator itTri = vecTriTemp.begin(); itTri != vecTriTemp.end(); itTri++)
	{
		this->m_vecTriangles.push_back(*itTri);
	}

	// Copy the new vertex information to the vertex buffer
	// Copy the local vertex array into the GPUs memory
	unsigned int numberOfBytesToCopy =
		this->m_VAOBufferInfoTriangles.numberOfVerticesToDraw *
		sizeof(sVertex_xyzw_rgba);

	GLenum err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VAOBufferInfoTriangles.vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER,
		numberOfBytesToCopy,
		this->m_VAOBufferInfoTriangles.pLocalVertexArray,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	err = glGetError();
	std::string error;
	std::string errDetails;
	if (err != GL_NO_ERROR)
	{
//		error = decodeGLErrorFromEnum(err, errDetails);
		error = COpenGLError::TranslateErrorEnum(err);
	}

	//	numberOfBytesToCopy,
	//	this->m_VAOBufferInfoTriangles.pLocalVertexArray,
	//	GL_DYNAMIC_DRAW);
	
	//	void* pGPUBuff = glMapBuffer( this->m_VAOBufferInfoTriangles.vertex_buffer_ID,
     //                             GL_COPY_WRITE_BUFFER);
	//memcpy(
	//	this->m_VAOBufferInfoTriangles.pLocalVertexArray,
	//	pGPUBuff,
	//	numberOfBytesToCopy);

//	glUnmapBuffer(this->m_VAOBufferInfoTriangles.vertex_buffer_ID);

	return;
}


void cDebugRenderer::m_copyLinesIntoRenderBuffer(double deltaTime)
{
	// Used to keep the "persistent" ones...
	std::vector<drLine> vecLinesTemp;

	this->m_VAOBufferInfoLines.numberOfObjectsToDraw = (unsigned int)this->m_vecLines.size();

	// Is the draw buffer big enough? 
	if ( this->m_VAOBufferInfoLines.bufferSizeObjects < this->m_VAOBufferInfoLines.numberOfObjectsToDraw )
	{
		// Resize the buffer
		this->resizeBufferForLines( this->m_VAOBufferInfoLines.numberOfObjectsToDraw );
	}

	this->m_VAOBufferInfoLines.numberOfVerticesToDraw
		= this->m_VAOBufferInfoLines.numberOfObjectsToDraw * 2;	// Lines

	unsigned int vertexIndex = 0;	// index of the vertex buffer to copy into 
	unsigned int lineIndex = 0;		// index of the triangle buffer
	for (; lineIndex != this->m_VAOBufferInfoLines.numberOfObjectsToDraw; 
		   lineIndex++, vertexIndex += 2)
	{
		drLine& curLine = this->m_vecLines[lineIndex];
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].x = curLine.points[0].x;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].y = curLine.points[0].y;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].z = curLine.points[0].z;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].w = 1.0f;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].r = curLine.colour.r;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].g = curLine.colour.g;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].b = curLine.colour.b;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+0].a = 1.0f;

		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].x = curLine.points[1].x;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].y = curLine.points[1].y;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].z = curLine.points[1].z;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].w = 1.0f;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].r = curLine.colour.r;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].g = curLine.colour.g;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].b = curLine.colour.b;
		this->m_VAOBufferInfoLines.pLocalVertexArray[vertexIndex+1].a = 1.0f;

		curLine.lifeTime -= static_cast<float>(deltaTime);

		// Keep this one? (i.e. is persistent?)
		if (curLine.lifeTime > 0.0f)
		{
			vecLinesTemp.push_back(curLine);
		}
	}//for (; 


	// Clear the triangle list and push back the persistent ones
	this->m_vecLines.clear();
	for (std::vector<drLine>::iterator itLine = vecLinesTemp.begin(); itLine != vecLinesTemp.end(); itLine++)
	{
		this->m_vecLines.push_back(*itLine);
	}

	// Copy the new vertex information to the vertex buffer
	// Copy the local vertex array into the GPUs memory
	unsigned int numberOfBytesToCopy =
		this->m_VAOBufferInfoLines.numberOfVerticesToDraw *
		sizeof(sVertex_xyzw_rgba);

	GLenum err = glGetError();

	glBindBuffer(GL_ARRAY_BUFFER, this->m_VAOBufferInfoLines.vertex_buffer_ID);
	glBufferData(GL_ARRAY_BUFFER,
		numberOfBytesToCopy,
		this->m_VAOBufferInfoLines.pLocalVertexArray,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	err = glGetError();
	std::string error;
	std::string errDetails;
	if (err != GL_NO_ERROR)
	{
//		error = decodeGLErrorFromEnum(err, errDetails);
		error = COpenGLError::TranslateErrorEnum(err);
	}
	return;
}

void cDebugRenderer::m_copyPointsIntoRenderBuffer(double deltaTime)
{
	// TODO: 

	return;
}

void cDebugRenderer::addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drTri tempTri(v1XYZ, v2XYZ, v3XYZ, colour, lifeTime);
	this->addTriangle(tempTri);
	return;
}


void cDebugRenderer::addTriangle(drTri &tri)
{
	this->m_vecTriangles.push_back(tri);
	return;
}

void cDebugRenderer::addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	drLine tempLine(startXYZ, endXYZ, colour, lifeTime);
	this->addLine(tempLine);
	return;
}

void cDebugRenderer::addLine(drLine &line)
{
	this->m_vecLines.push_back(line);
	return;
}

void cDebugRenderer::addPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime/*=0.0f*/, float pointSize/*=1.0f*/)
{
	drPoint tempPoint(xyz, colour, lifeTime, pointSize);
	this->addPoint(tempPoint);
	return;
}

void cDebugRenderer::addPoint(drPoint &point)
{
	this->m_vecPoints.push_back(point);
	return;
}

// Replaces the DrawDebugSphere
void cDebugRenderer::addDebugSphere(glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	iDebugRenderer::sDebugMesh sphereMesh;
	sphereMesh.name = iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME;
	sphereMesh.scale = scale;
	sphereMesh.xyz = xyz;
	sphereMesh.colour = colour;
	sphereMesh.lifeTime = lifeTime;
	this->m_vecMeshes.push_back(sphereMesh);
	return;
}

std::string cDebugRenderer::getLastError(bool bClearError /*=true*/)
{
	std::string theLastError = this->m_lastError;
	if ( bClearError )
	{
		this->m_lastError = "";
	}
	return theLastError;
}



// *********************************************************
//	 ___      _                ___ _                      
//	|   \ ___| |__ _  _ __ _  / __| |_  __ _ _ __  ___ ___
//	| |) / -_) '_ \ || / _` | \__ \ ' \/ _` | '_ \/ -_|_-<
//	|___/\___|_.__/\_,_\__, | |___/_||_\__,_| .__/\___/__/
//	                   |___/                |_|           
//

//static 
const std::string iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME = "sphere";

iDebugRenderer::sDebugTri::sDebugTri()
{
	this->v[0] = glm::vec3(0.0f); this->v[1] = glm::vec3(0.0f); this->v[2] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 1.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v[3], glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->v[0] = v[0];
	this->v[1] = v[1];
	this->v[2] = v[2];
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine()
{
	this->points[0] = glm::vec3(0.0f);
	this->points[0] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);		// white
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 start, glm::vec3 end)
{
	this->points[0] = start;
	this->points[1] = end;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}
iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->points[0] = start;
	this->points[1] = end;
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugLine::sDebugLine(glm::vec3 points[2], glm::vec3 colour, float lifeTime/*=0.0f*/)
{
	this->points[0] = points[0];
	this->points[1] = points[1];
	this->colour = colour;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

/*static*/ 
const float cDebugRendererDEFAULT_POINT_SIZE = 1.0f;


iDebugRenderer::sDebugPoint::sDebugPoint()
{
	this->xyz = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->lifeTime = 0.0f;
	this->pointSize = cDebugRendererDEFAULT_POINT_SIZE;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz)		// colour=1,1,1		lifeTime=0.0f	pointSize=1.0f
{
	this->xyz = xyz;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->pointSize = 1.0f;
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}
iDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float lifeTime/*=0.0f*/, float pointSize/*=1.0f*/)
{
	this->xyz = xyz;
	this->colour = colour;
	this->pointSize = pointSize;
	this->lifeTime = lifeTime;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh()
{
	this->name = iDebugRenderer::DEFAULT_DEBUG_SPHERE_MESH_NAME;	// = "DSPHERE";
	this->xyz = glm::vec3(0.0f,0.0f,0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

// Assumes a size of 1, colour white
iDebugRenderer::sDebugMesh::sDebugMesh(std::string name)
{
	this->name = name;
	this->xyz = glm::vec3(0.0f,0.0f,0.0f);
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->lifeTime = 0.0f;
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	this->bIgnorDepthBufferOnDraw = true;
	return;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 colour, float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(glm::vec3(0.0f,0.0f,0.0f));
	this->scale = 1.0f;
	this->colour = glm::vec3(1.0f,1.0f,1.0f);
	return;
	this->bIgnorDepthBufferOnDraw = true;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::vec3 EulerOrientation, glm::vec3 colour, 
                                       float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = glm::quat(EulerOrientation);
	this->scale = scale;
	this->colour = colour;
	return;
	this->bIgnorDepthBufferOnDraw = true;
}

iDebugRenderer::sDebugMesh::sDebugMesh(std::string name, glm::vec3 xyz, glm::quat qOrientation, glm::vec3 colour, 
                                       float scale, float lifeTime/*=0.0f*/)
{
	this->name = name;
	this->xyz = xyz;
	this->lifeTime = lifeTime;
	this->qOrientation = qOrientation;
	this->scale = scale;
	this->colour = colour;
	return;
	this->bIgnorDepthBufferOnDraw = true;
}
//
// *********************************************************


