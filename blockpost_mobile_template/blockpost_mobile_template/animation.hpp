#pragma once


#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

class animation
{
public:
    animation( float i = 0.f ) : value( i )
    { }

    auto update( float to_value, float duration = 0.10f ) -> void;
    float value = 0.f;
};

class animation_vec2
{
public:
    animation_vec2( ImVec2 value = ImVec2( 0, 0 ) ) : value( value )
    { }

    auto update( ImVec2 to_value, float duration = 0.10f ) -> void;
    struct ImVec2 value = { 0, 0 };
};

class animation_vec4
{
public:
    animation_vec4( ImVec4 value = ImVec4( 0, 0, 0, 0 ) ) : value( value )
    { }

    auto interpolate( ImVec4 first_value, ImVec4 second_value, bool interp, float duration = 0.10f ) -> void;
    auto update( ImVec4 to_value, float duration = 0.15f ) -> void;
    struct ImVec4 value = { 0, 0, 0, 0 };
};

