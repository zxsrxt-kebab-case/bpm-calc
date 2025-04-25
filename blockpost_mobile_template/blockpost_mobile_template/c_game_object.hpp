#pragma once
#include "c_component.hpp"
#include "c_object.hpp"
#include "c_type.hpp"
#include "c_array.hpp"

class c_game_object : public c_object
{
public:
	template< class _Ty >
	c_array< _Ty >* get_components_in_children( c_type* type, bool inactive )
	{
		static auto method = il2cpp_assembly::open( "UnityEngine.CoreModule" )->image( )->get_class( "UnityEngine", "GameObject" )
			->get_method( "GetComponentsInChildren", 2 );
		return reinterpret_cast< c_array< _Ty >*( * )( c_game_object*, c_type*, bool ) >( method->get_method_pointer( ) )( this, type, inactive );
	}
	void set_active( bool state )
	{
		reinterpret_cast< void ( * )( c_game_object*, bool ) >( il2cpp::resolve_icall( "UnityEngine.GameObject::SetActive" ) )( this, state );
	}
};