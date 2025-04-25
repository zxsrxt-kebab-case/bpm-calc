#pragma once

#include "il2cpp.hpp"

class c_controll_touch
{
public:
	static bool get_in_shoot( )
	{
		static auto field = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "ControllTouch" )->get_field( "inAttackKey" );

		return field->get_static_value<bool>( );
	}
	static void set_in_shoot( bool state )
	{
		static auto field = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "ControllTouch" )->get_field( "inAttackKey" );

		field->set_static_value<bool>( state );
	}
};