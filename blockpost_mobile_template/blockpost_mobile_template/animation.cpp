#include "animation.hpp"

#include <cmath>

inline float ease( float time, float start_value, float change_value, float duration )
{
    return change_value * time / duration + start_value;
}

float resolve_value( float clock, float previous_value, float new_value, float duration )
{
    float value = ease( clock, previous_value, new_value - previous_value, duration );

    if ( std::abs( value ) < .001f )
    {
        value = new_value;
    }

    if ( std::fmod( value, 1 ) < .001f )
    {
        value = std::floor( value );
    }
    else if ( std::fmod( value, 1 ) > .99f )
    {
        value = std::ceil( value );
    }

    return value;
}

auto animation::update( float to_value, float duration ) -> void
{
    float frame = 1.0f / 0.016f;
    float multiplier;
    if ( frame < 60 )
        multiplier = 1;
    else
        multiplier = frame / 60.f;
    this->value = resolve_value( 0.016f, this->value, to_value, duration * multiplier );
}

auto animation_vec2::update( ImVec2 to_value, float duration ) -> void
{
    float frame = 1.0f / 0.016f;
    float multiplier;
    if ( frame < 60 )
        multiplier = 1;
    else
        multiplier = frame / 60.f;
    this->value = {
            resolve_value( 0.016f, this->value.x, to_value.x, duration * multiplier ),
            resolve_value( 0.016f, this->value.y, to_value.y, duration * multiplier )
    };
}

auto animation_vec4::interpolate( ImVec4 first_value, ImVec4 second_value, bool interp, float duration ) -> void
{
    if ( interp )
        this->update( first_value, duration );
    else
        this->update( second_value, duration );
}

auto animation_vec4::update( ImVec4 to_value, float duration ) -> void
{
    float frame = 1.0f / 0.016f;
    float multiplier;
    if ( frame < 60 )
        multiplier = 1;
    else
        multiplier = frame / 60.f;
    this->value = {
            resolve_value( 0.016f, this->value.x, to_value.x, duration * multiplier ),
            resolve_value( 0.016f, this->value.y, to_value.y, duration * multiplier ),
            resolve_value( 0.016f, this->value.z, to_value.z, duration * multiplier ),
            resolve_value( 0.016f, this->value.w, to_value.w, duration * multiplier )
    };
}
