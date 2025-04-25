#pragma once
#include "c_component.hpp"

class c_collider : public c_component
{
public:
	void set_enabled( bool state )
	{
		reinterpret_cast< void ( * )( c_collider*, bool ) >( il2cpp::resolve_icall( "UnityEngine.Collider::set_enabled" ) )( this, state );
	}
};