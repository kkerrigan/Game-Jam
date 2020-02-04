#pragma once
#include <string>
#include <Windows.h>

namespace SmartyGL
{
	/*!	\class	cDllLoader
		\brief
		\author Daniel Maclam
		\date	2019-02-01 2:14 PM
		*/
	class cDllLoader
	{
	private:
		HINSTANCE h_dll;
	public:
		cDllLoader();
		bool load_dll(const std::string& dll_file);
		void unload();

		template<typename T>
		T get_proc_address(const std::string& function_name)
		{
			return reinterpret_cast<T>(GetProcAddress(this->h_dll, function_name.c_str()));
		}
	};
}

