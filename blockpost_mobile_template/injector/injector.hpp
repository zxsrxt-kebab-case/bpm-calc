#pragma once

#include <string>
#include <vector>
#include <Windows.h>

namespace injector
{
	enum class status
	{
		error,
		success
	};
	extern std::vector<DWORD> GetAllProcessIds( );
	extern std::string GetProcessName( DWORD processId );
	extern status inject_dll( std::string path, int mode, DWORD pid );
}
