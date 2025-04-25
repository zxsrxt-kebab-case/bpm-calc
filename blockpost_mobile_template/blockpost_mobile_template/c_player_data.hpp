#pragma once

#include "string.hpp"
#include "c_player_object.hpp"
#include "c_base_item_info.hpp"


class c_player_weapon
{
public:
    c_base_item_info* it( )
    {
        return *reinterpret_cast< c_base_item_info** >( ( uintptr_t ) this + 0x18 );
    }
};

class c_player_data
{
public:
    bool islocal( )
    {
        return *reinterpret_cast< bool* >( ( uintptr_t ) this + 0x20 );
    }
    int health( )
    {
        return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x50 );
    }
    int id( )
    {
        return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x10 );
    }
    int armor( )
    {
        return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x54 );
    }
    int team( )
    {
        return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x38 );
    }
    c_player_object* object( )
    {
        return *reinterpret_cast< c_player_object** >( ( uintptr_t ) this + 0x28 );
    }
    ustring* get_name( )
    {
        return *reinterpret_cast< ustring** >( ( uintptr_t ) this + 0x18 );
    }
    vec3_t& prev_pos( )
    {
        return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0x90 );
    }
    vec3_t& pos( )
    {
        return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0x9C );
    }
    vec3_t& lerp_pos( )
    {
        return *reinterpret_cast< vec3_t* >( ( uintptr_t ) this + 0xA8 );
    }
    c_player_weapon* curwpn( )
    {
        return *reinterpret_cast< c_player_weapon** >( ( uintptr_t ) this + 0x130 );
    }
};