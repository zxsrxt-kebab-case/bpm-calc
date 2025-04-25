#include "il2cpp_assembly.hpp"

il2cpp_assembly* il2cpp_assembly::open( std::string ass )
{
	static auto domain = reinterpret_cast< void* ( * )( void ) >( IAPI( "il2cpp_domain_get" ) )( );
	return reinterpret_cast< il2cpp_assembly * ( * )( void*, const char* ) >( IAPI( "il2cpp_domain_assembly_open" ) )( domain, ass.c_str( ) );
}

il2cpp_image* il2cpp_assembly::image( )
{
	return *reinterpret_cast< il2cpp_image** >( ( uintptr_t ) this );
}