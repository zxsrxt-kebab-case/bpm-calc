#pragma once
#include "il2cpp_array.hpp"
#include "il2cpp.hpp"
#include "c_type.hpp"
class c_object
{
public:
	template<class _t>
	static il2cpp_array<_t>* find( c_type* type )
	{
		static auto ptr = il2cpp_assembly::open( "UnityEngine.CoreModule" )->image( )->get_class( "UnityEngine", "Object" )
			->get_method( "FindObjectsOfType", 1 )->get_method_pointer( );

		return reinterpret_cast< il2cpp_array<_t>*( * )( c_type* ) >( ptr )( type );
	}
};