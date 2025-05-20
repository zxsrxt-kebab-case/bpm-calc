#pragma once
#include "c_component.hpp"
#include "c_game_object.hpp"
#include "il2cpp_array.hpp"

class c_player_object
{
public:
    c_transform* m_tr( )
    {
        return *reinterpret_cast< c_transform** >( ( uintptr_t ) this + 0xF8 );
    }
    il2cpp_array<c_transform*>* bones( )
    {
        return *reinterpret_cast< il2cpp_array<c_transform*>** >( ( uintptr_t ) this + 0x100 );
    }
    c_game_object* go( )
    {
        return *reinterpret_cast< c_game_object** >( ( uintptr_t ) this + 0x38 );
    }
};