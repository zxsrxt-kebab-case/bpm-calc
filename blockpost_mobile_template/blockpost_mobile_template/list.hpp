#pragma once
#include <vector>
#include "c_array.hpp"

template<typename T>
class ulist
{
public:
    inline c_array<T>* unity_array( )
    {
        return *reinterpret_cast< c_array<T>* * >( ( uintptr_t ) this + 0x10 );
    }
    inline void _c_array( T* data )
    {
        uintptr_t arr = *reinterpret_cast< uintptr_t* >( ( uintptr_t ) this + 0x10 );
        data = *reinterpret_cast< T** >( arr + 0x20 );
    }
    inline int count( )
    {
        return *reinterpret_cast< int* >( ( uintptr_t ) this + 0x18 );
    }
};