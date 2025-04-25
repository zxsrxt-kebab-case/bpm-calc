#pragma once
#include "mem.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "il2cpp.hpp"

class c_transform
{
public:
    vec3_t get_position( )
    {
        vec3_t ret;
        reinterpret_cast< void( * )( c_transform*, vec3_t* ) >( il2cpp::resolve_icall( "UnityEngine.Transform::get_position_Injected" ) )( this, &ret );
        return ret;
    }
    vec4_t get_rotation( )
    {
        vec4_t ret;
        reinterpret_cast< void( * )( c_transform*, vec4_t* ) >( il2cpp::resolve_icall( "UnityEngine.Transform::get_rotation_Injected" ) )( this, &ret );
        return ret;
    }
    vec3_t get_forward( )
    {
        static vec3_t forward = { 0, 0, 1 };
        return get_rotation( ) * forward;
    }    
    vec3_t get_right( )
    {
        static vec3_t right = { 1, 0, 0 };
        return get_rotation( ) * right;
    }    
    vec3_t get_up( )
    {
        static vec3_t up = { 0, 1, 0 };
        return get_rotation( ) * up;
    }
};