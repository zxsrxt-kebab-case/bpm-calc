#pragma once
#include "common.hpp"

class il2cpp_field
{
public:
	std::string get_name( );
	template<typename T>
	T get_static_value( )
	{
		T value;
		reinterpret_cast< void ( * )( il2cpp_field*, T* ) >( IAPI( "il2cpp_field_static_get_value" ) )( this, &value );
		return value;
	}
	template<typename T>
	void set_static_value( T val )
	{		
		reinterpret_cast< void ( * )( il2cpp_field*, T* ) >( IAPI( "il2cpp_field_static_set_value" ) )( this, &val );
	}
};

