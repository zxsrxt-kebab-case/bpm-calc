#pragma once
#include "il2cpp.hpp"


class c_type
{
public:
	static c_type* get( const char* type )
	{
		static auto method = il2cpp_assembly::open( "mscorlib.dll" )->image( )->get_class( "System", "Type" )->get_method( "GetType", 1 );
		return reinterpret_cast< c_type * ( * )( ustring* ) >( method->get_method_pointer( ) )( il2cpp::string_new( type ) );
	}
};