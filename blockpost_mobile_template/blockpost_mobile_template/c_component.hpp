#pragma once
#include "mem.hpp"

#include "il2cpp.hpp"
#include "c_transform.hpp"
#include "c_object.hpp"

class c_component : public c_object
{
public:
    c_transform* get_transform( )
    {
        return reinterpret_cast< c_transform * ( * )( c_component* ) >( il2cpp::resolve_icall( "UnityEngine.Component::get_transform" ) )( this );
    }
};