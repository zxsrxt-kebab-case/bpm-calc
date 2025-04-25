#include "il2cpp_method.hpp"

#include "il2cpp_header.hpp"

std::string il2cpp_method::get_name( )
{
	return std::string( reinterpret_cast< const char* ( * ) ( il2cpp_method* ) >( IAPI( "il2cpp_method_get_name" ) )( this ) );
}
uint32_t il2cpp_method::get_param_count( )
{
	return reinterpret_cast< uint32_t ( * ) ( il2cpp_method* ) >( IAPI( "il2cpp_method_get_param_count" ) )( this );
}
std::string il2cpp_method::get_param_name( uint32_t idx )
{
	return std::string( reinterpret_cast< const char* ( * ) ( il2cpp_method*, uint32_t ) >( IAPI( "il2cpp_method_get_param_name" ) )( this, idx ) );
}