#ifndef _cBasicTextureManager_HG_
#define _cBasicTextureManager_HG_

#include <string>
#include <map>
#include "CTextureFromBMP.h"

class cBasicTextureManager
{
private:
	cBasicTextureManager() {}
	~cBasicTextureManager() {}

public:

	static cBasicTextureManager* getInstance() {
		static cBasicTextureManager instance;
		
		return &instance;
	}
	bool Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap );

	// Picks a random texture from the textures loaded
	std::string PickRandomTexture(void);

	bool CreateCubeTextureFromBMPFiles( std::string cubeMapName, 
		                                std::string posX_fileName, std::string negX_fileName, 
		                                std::string posY_fileName, std::string negY_fileName, 
										std::string posZ_fileName, std::string negZ_fileName, 
										bool bIsSeamless, std::string &errorString );


	// returns 0 on error
	GLuint getTextureIDFromName( std::string textureFileName );

	void SetBasePath(std::string basepath);

	//HACK: post load cleanup (stuff we don't need on the c++ side)
	void post_load_cleanup();

	std::map< std::string, CTextureFromBMP* > m_map_TexNameToTexture;

private:
	std::string m_basePath;
	std::string m_lastError;
	void m_appendErrorString( std::string nextErrorText );
	void m_appendErrorStringLine( std::string nextErrorTextLine );

	


};

#endif
