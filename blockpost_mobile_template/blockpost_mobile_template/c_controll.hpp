#pragma once

#include "il2cpp.hpp"
#include "constants.hpp"
#include "c_example_player.hpp"

class c_controll
{
public:
	static c_example_player* get_ex_player( )
	{
		static auto field = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "Controll" )->get_field( constants::controll::ex_player );
		return field->get_static_value<c_example_player*>( );
	}	
	static void set_in_zoom( bool state )
	{
		static auto field = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "Controll" )->get_field( constants::controll::in_zoom );
		field->set_static_value<bool>( state );
	}	
	static bool get_in_zoom( )
	{
		static auto field = il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "Controll" )->get_field( constants::controll::in_zoom );
		return field->get_static_value<bool>( );
	}

};