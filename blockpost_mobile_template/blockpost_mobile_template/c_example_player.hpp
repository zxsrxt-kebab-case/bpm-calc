#pragma once
#include "c_component.hpp"
#include "c_example_character.hpp"
#include "c_example_camera.hpp"

class c_example_player : public c_component
{
public:
	c_example_character* character( )
	{
		return *reinterpret_cast< c_example_character** >( ( uintptr_t ) this + 0x18 );
	}	
	c_example_camera* camera( )
	{
		return *reinterpret_cast< c_example_camera** >( ( uintptr_t ) this + 0x20 );
	}
};