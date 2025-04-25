#pragma once

#include "il2cpp_header.hpp"
#include "string.hpp"

#include "il2cpp_assembly.hpp"
#include "il2cpp_icall.hpp"

namespace il2cpp
{
	extern void init( );

	extern Il2CppDomain* domain_get( );
	extern Il2CppAssembly* domain_assembly_open( Il2CppDomain* domain, const char* name );
	extern Il2CppClass* class_from_name( Il2CppImage* image, const char* namespaze, const char* name );
	extern FieldInfo* class_get_field_from_name( Il2CppClass* klass, const char* name );
	extern MethodInfo* class_get_method_from_name( Il2CppClass* klass, const char* name, int params );

	extern void field_static_get_value( FieldInfo* field, void* val );
	extern void field_static_set_value( FieldInfo* field, void* val );
	extern uintptr_t resolve_icall( const char* name );
	extern ustring* string_new( const char* str );
	extern void thread_attach( Il2CppDomain* dom );
	extern void* object_new( Il2CppClass* kls );
}
