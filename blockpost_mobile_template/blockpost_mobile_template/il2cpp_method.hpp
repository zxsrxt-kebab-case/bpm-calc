#pragma once
#include "common.hpp"

class il2cpp_method
{
public:
	std::string get_name( );
	uint32_t get_param_count( );
	std::string get_param_name( uint32_t idx );
	uintptr_t& get_method_pointer( )
	{
		return *reinterpret_cast<uintptr_t*>( uintptr_t(this) );
	}

	template< auto hk, typename orig_t>
	void hook( orig_t* orig )
	{
		*orig = reinterpret_cast< orig_t >( get_method_pointer( ) );
		get_method_pointer( ) = reinterpret_cast<uintptr_t>( hk );
	}
};

