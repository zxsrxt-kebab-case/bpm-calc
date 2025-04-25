#pragma once
#include "mem.hpp"
#include "string.hpp"
#include "il2cpp.hpp"
class c_shader
{
public:
	static c_shader* find( const char* name )
	{
		static auto method = il2cpp_assembly::open( "UnityEngine.CoreModule" )->image( )->get_class( "UnityEngine", "Shader" )->get_method( "Find", 1 );
		return reinterpret_cast< c_shader * ( * )( ustring* ) >( method->get_method_pointer( ) )( il2cpp::string_new( name ) );
	}
	int32_t property_to_id( const char* prop )
	{
		return reinterpret_cast< int32_t( * )( c_shader*, ustring* ) >( il2cpp::resolve_icall( "UnityEngine.Shader::PropertyToID" ) )( this, il2cpp::string_new( prop ) );
	}
};