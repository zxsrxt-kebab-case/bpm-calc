#pragma once
#include "c_component.hpp"
#include "c_material.hpp"
#include "c_array.hpp"
#include "il2cpp.hpp"

class c_renderer : public c_component
{
public:
	c_material* get_material( )
	{
		return reinterpret_cast< c_material * ( * )( c_renderer* ) >( il2cpp::resolve_icall( "UnityEngine.Renderer::GetMaterial" ) )( this );
	}		
	void set_material( c_material* mat )
	{
		reinterpret_cast< void ( * )( c_renderer*, c_material* ) >( il2cpp::resolve_icall( "UnityEngine.Renderer::SetMaterial" ) )( this, mat );
	}	
	c_array<c_material*>* get_materials( )
	{
		return reinterpret_cast< c_array<c_material*>* ( * )( c_renderer* ) >( il2cpp::resolve_icall( "UnityEngine.Renderer::GetMaterialArray" ) )( this );
	}	
	void set_materials( c_array<c_material*>* mats )
	{
		reinterpret_cast< void ( * )( c_renderer*, c_array<c_material*>* ) >( il2cpp::resolve_icall( "UnityEngine.Renderer::SetMaterialArray" ) )( this, mats );
	}
	void set_allow_occlusion( bool state )
	{
		reinterpret_cast< void( * )( c_renderer*, bool ) >( il2cpp::resolve_icall( "UnityEngine.Renderer::set_allowOcclusionWhenDynamic" ) )( this, state );
	}
};