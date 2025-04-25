#pragma once

#include "il2cpp.hpp"
#include "constants.hpp"
#include "vec3.hpp"

class c_weapon_object
{
public:
	float& recoil_pattern( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x1CC );
	}	
	float& recoil_vertical( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x1BC );
	}	
	float& recoil_horizontal( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x1C0 );
	}	
	float& spread_pattern( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x1D0 );
	}	
	float& spread( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x1C4 );
	}	
	float& stability( )
	{
		return *reinterpret_cast< float* >( ( uintptr_t ) this + 0x1C8 );
	}
	int& slot( )
	{
		return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x320 );
	}
};

class c_shooter
{
public:
	static c_shooter* cs( )
	{
		static auto fld = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "Shooter" )->get_field( constants::shooter::cs );
		return fld->get_static_value<c_shooter*>( );
	}

	vec3_t& hands_pos( )
	{
		return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0x1D0 );
	}

	c_weapon_object* curwpn( )
	{
		return *reinterpret_cast< c_weapon_object** >( ( uintptr_t ) this + 0x20 );
	}
};