#pragma once
#include "common.hpp"

class il2cpp_delegate
{
public:
	uintptr_t& get_method_pointer( );

	template< auto hk, typename orig_t>
	void hook( orig_t* orig )
	{
		*orig = reinterpret_cast< orig_t >( get_method_pointer( ) );
		get_method_pointer( ) = reinterpret_cast< uintptr_t >( hk );
	}
};