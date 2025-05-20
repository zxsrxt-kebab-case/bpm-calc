#pragma once
#include <vector>
#include "il2cpp_array.hpp"

template<typename T>
class il2cpp_list
{
public:
    char _pad[ sizeof( void* ) * 2 ];

    il2cpp_array<T>* m_it;
    int m_size;
    int m_ver;
    char __pad[ sizeof( void* ) * 2 ];

    T* begin( )
    {
        return m_it->begin( );
    }    
    T* end( )
    {
        return m_it->end( );
    }
    std::vector<T> to_vec( )
    {
        return m_it->to_vec( );
    }
};