#pragma once
#include "c_shader.hpp"
#include "il2cpp.hpp"
#include "mem.hpp"
#include "imgui/imgui.h"

extern il2cpp_method* mthd228;

class c_material
{
public:
	void set_shader( c_shader* shader )
	{
		reinterpret_cast< void( * )( c_material*, c_shader* ) >( il2cpp::resolve_icall( "UnityEngine.Material::set_shader" ) )( this, shader );
	}
	c_shader* get_shader( )
	{
		return reinterpret_cast< c_shader * ( * )( c_material* ) >( il2cpp::resolve_icall( "UnityEngine.Material::get_shader" ) )( this );
	}
	void set_texture( const char* prop, void* tex )
	{
		static auto method = il2cpp_assembly::open( "UnityEngine.CoreModule" )->image( )->get_class( "UnityEngine", "Material" )->get_method( "SetTexture", { "name", "value" } );
		if ( method )reinterpret_cast< void( * )( c_material*, ustring*, void* ) >( method->get_method_pointer( ) )( this, il2cpp::string_new( prop ), tex );
	}	
	void set_color( const char* prop, ImColor col )
	{
		static auto method = il2cpp_assembly::open( "UnityEngine.CoreModule" )->image( )->get_class( "UnityEngine", "Material" )->get_method( "SetColor", { "name", "value" } );
		if ( method )reinterpret_cast< void( * )( c_material*, ustring*, ImColor ) >( method->get_method_pointer( ) )( this, il2cpp::string_new( prop ), col );
	}
	void set_float( const char* prop, float data )
	{
		static auto method = il2cpp_assembly::open( "UnityEngine.CoreModule" )->image( )->get_class( "UnityEngine", "Material" )->get_method( "SetFloat", { "name", "value" } );
		if ( method )reinterpret_cast< void( * )( c_material*, ustring*, float ) >( method->get_method_pointer( ) )( this, il2cpp::string_new( prop ), data );
	}
	void set_int( const char* prop, int data )
	{
		set_float( prop, ( float ) data );
	}
	static c_material* create( c_shader* shader )
	{
		static auto domain = il2cpp::domain_get( );
		static auto assembly = il2cpp::domain_assembly_open( domain, "UnityEngine.CoreModule" );
		static auto mat_kls = il2cpp::class_from_name( assembly->image, "UnityEngine", "Material" );
		auto obj = reinterpret_cast< c_material* >( il2cpp::object_new( mat_kls ) );
		reinterpret_cast< void( * )( c_material*, c_shader* ) >( il2cpp::resolve_icall( "UnityEngine.Material::CreateWithShader" ) )( obj, shader );
		return obj;
	}
};