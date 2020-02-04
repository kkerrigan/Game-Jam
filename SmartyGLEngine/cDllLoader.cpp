/*
* Name:		cDllLoader.h
* Author:	Daniel Maclam
* Date		2019-02-01 2:14 PM
*/

#include "cDllLoader.h"


SmartyGL::cDllLoader::cDllLoader() : h_dll(nullptr)
{
}

bool SmartyGL::cDllLoader::load_dll(const std::string& dll_file)
{
	//if this instance has a dll already loaded get rid of it
	if(this->h_dll != nullptr)
	{
		this->unload();
	}

	this->h_dll = LoadLibrary(dll_file.c_str());

	return this->h_dll != nullptr;
}

void SmartyGL::cDllLoader::unload()
{
	FreeLibrary(this->h_dll);
	this->h_dll = nullptr;
}
