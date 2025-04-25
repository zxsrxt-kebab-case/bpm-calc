#pragma once
#include "c_component.hpp"
#include "vec3.hpp"
#include "imgui/imgui.h"


class c_camera : public c_component
{
public:
    struct c_matrix
    {
        float m[ 4 ][ 4 ];
        float* operator[]( int index );
    };
    struct ws_ret
    {
        bool checker;
        ImVec2 pos;
    };
    static c_camera* get_main( );
    ws_ret ws( vec3_t object );
};