#pragma once

#include "common.hpp"

class il2cpp_icall
{
public:
	static uintptr_t resolve( std::string name )
	{
		return reinterpret_cast< uintptr_t( * )( const char* ) >( IAPI( "il2cpp_resolve_icall" ) )( name.c_str( ) );
	}

	template< auto hk, typename orig_t>
	static void hook( uintptr_t addr, orig_t* orig )
	{
		if ( orig )
		{
			*orig = *reinterpret_cast< orig_t* > ( addr );
		}
		*reinterpret_cast< uintptr_t* > ( addr ) = reinterpret_cast< uintptr_t >( hk );
	}
};