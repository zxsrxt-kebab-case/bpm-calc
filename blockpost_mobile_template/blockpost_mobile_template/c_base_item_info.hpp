#pragma once

#include "string.hpp"

class c_base_item_info
{
public:
	int id( )
	{
		return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x10 );
	}	
	ustring* codename( )
	{
		return *reinterpret_cast< ustring** >( ( uintptr_t ) this + 0x18 );
	}
};