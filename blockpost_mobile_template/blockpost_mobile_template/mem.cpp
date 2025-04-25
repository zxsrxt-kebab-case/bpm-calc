#include "mem.hpp"

namespace mem
{
	DWORD64 game_assembly = 0;
	void init( )
	{
		game_assembly = reinterpret_cast< DWORD64 >( GetModuleHandle( "GameAssembly.dll" ) );
	}
}