#include "il2cpp_field.hpp"

std::string il2cpp_field::get_name( )
{
	return std::string( reinterpret_cast< const char* ( * ) ( il2cpp_field* ) >( IAPI( "il2cpp_field_get_name" ) )( this ) );
}