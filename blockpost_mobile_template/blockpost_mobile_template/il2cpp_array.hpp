#pragma once
#include <vector>

template<typename T>
class il2cpp_array
{
public:

    char pad[ sizeof( void* ) * 3 ];
    int32_t m_size;
    T m_it[100];

    T* begin( )
    {
        return m_it;
    }

    T* end( )
    {
        return m_it + m_size;
    }

    std::vector<T> to_vec( )
    {
        return std::vector<T>( begin( ), end( ) );
    }
};