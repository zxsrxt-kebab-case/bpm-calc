#include "il2cpp.hpp"

#include <Windows.h>

namespace il2cpp
{
	HMODULE game = nullptr;

	void init( )
	{
		game = GetModuleHandle( "GameAssembly.dll" );
	}
	Il2CppDomain* domain_get( )
	{
		return reinterpret_cast< Il2CppDomain * ( * )( ) >( GetProcAddress( game, "il2cpp_domain_get" ) )( );
	}	
	Il2CppAssembly* domain_assembly_open( Il2CppDomain* domain, const char* name )
	{
		return reinterpret_cast< Il2CppAssembly * ( * )( Il2CppDomain*, const char* ) >( GetProcAddress( game, "il2cpp_domain_assembly_open" ) )( domain, name );
	}	
	Il2CppClass* class_from_name( Il2CppImage* image, const char* namespaze, const char* name )
	{
		return reinterpret_cast< Il2CppClass * ( * )( Il2CppImage*, const char*, const char* ) >( GetProcAddress( game, "il2cpp_class_from_name" ) )( image, namespaze, name );
	}	
	FieldInfo* class_get_field_from_name( Il2CppClass* klass, const char* name )
	{
		return reinterpret_cast< FieldInfo * ( * )( Il2CppClass*, const char* ) >( GetProcAddress( game, "il2cpp_class_get_field_from_name" ) )( klass, name );
	}		
	MethodInfo* class_get_method_from_name( Il2CppClass* klass, const char* name, int params )
	{
		return reinterpret_cast< MethodInfo * ( * )( Il2CppClass*, const char*, int ) >( GetProcAddress( game, "il2cpp_class_get_method_from_name" ) )( klass, name, params );
	}	
	void field_static_get_value( FieldInfo* field, void* val )
	{
		reinterpret_cast< void ( * )( FieldInfo*, decltype( val ) )>( GetProcAddress( game, "il2cpp_field_static_get_value" ) )( field, val );
	}	
	void field_static_set_value( FieldInfo* field, void* val )
	{
		reinterpret_cast< void ( * )( FieldInfo*, void* ) >( GetProcAddress( game, "il2cpp_field_static_set_value" ) )( field, val );
	}
	uintptr_t resolve_icall( const char* name )
	{
		return reinterpret_cast< uintptr_t ( * )( const char* ) >( GetProcAddress( game, "il2cpp_resolve_icall" ) )( name );
	}	
	ustring* string_new( const char* str )
	{
		return reinterpret_cast< ustring* ( * )( const char* ) >( GetProcAddress( game, "il2cpp_string_new" ) )( str );
	}
	void thread_attach( Il2CppDomain* dom )
	{
		reinterpret_cast< void ( * )( Il2CppDomain* ) >( GetProcAddress( game, "il2cpp_thread_attach" ) )( dom );
	}
	void* object_new( Il2CppClass* kls )
	{
		return reinterpret_cast< void* ( * )( Il2CppClass* ) >( GetProcAddress( game, "il2cpp_object_new" ) )( kls );
	}
}