#pragma once
#include "vec3.hpp"
#include <cstdint>
#include "c_kinematic_character.hpp"
#include "list.hpp"

class c_example_character : public c_component
{
public:
	float& max_stable_move_speed( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x20 );
	}	
	float& stable_movement_sharpness( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x24 );
	}
	float& max_air_move_speed( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x30 );
	}	
	float& air_acceleration_speed( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x34 );
	}		
	float& jump_scalable_forward( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x44 );
	}	
	float& drag( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x38 );
	}
	vec3_t& gravity( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0x74 );
	}	
	vec3_t& look_input_vec( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0xFC );
	}	
	vec3_t& _look_input_vec( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0xAC );
	}	
	vec3_t& move_input_vec( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0xF0 );
	}	
	vec3_t& _move_input_vec( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0xA0 );
	}
	c_kinematic_character* motor( )
	{
		return *reinterpret_cast< c_kinematic_character** >( ( uintptr_t ) this + 0x18 );
	}
	ulist<vec3_t>* storepos( )
	{
		return *reinterpret_cast< ulist<vec3_t>** >( ( uintptr_t ) this + 0x148 );
	}
	int& bitmask( )
	{
		return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x130 );
	}
	bool& send_in_air( )
	{
		return *reinterpret_cast< bool* >( ( uintptr_t ) this + 0x134 );
	}	
	bool& send_in_duck( )
	{
		return *reinterpret_cast< bool* >( ( uintptr_t ) this + 0x135 );
	}	
	bool& jumped_this_frame( )
	{
		return *reinterpret_cast< bool* >( ( uintptr_t ) this + 0xBA );
	}	
	bool& jump_requested( )
	{
		return *reinterpret_cast< bool* >( ( uintptr_t ) this + 0xB8 );
	}
};