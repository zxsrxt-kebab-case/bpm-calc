#pragma once
#include <vector>

template<typename T>
struct c_array
{
    void* klass;
    void* monitor;
    void* bounds;
    int32_t cap;
    T m_it[24];

    T* begin( )
    {
        return m_it;
    }

    T* end( )
    {
        return m_it + cap;
    }

    std::vector<T> to_vec( )
    {
        return std::vector<T>( begin( ), end( ) );
    }
};