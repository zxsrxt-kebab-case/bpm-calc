#include "callbacks.hpp"
#include <algorithm>
#include <Windows.h>

#include "callbacks.hpp"
#include <algorithm>

bool callback_t::operator==( const callback_t& other ) const noexcept
{
    return function.target_type( ) == other.function.target_type( ) &&
        type == other.type;
}

callback_t::operator bool( ) const noexcept
{
    return static_cast< bool >( function );
}

void callback_system::add( callback_t callback )
{
    if ( !callback ) return;

    std::unique_lock lock( m_mutex );
    m_callbacks.push_back( std::move( callback ) );
}

bool callback_system::erase( const callback_t& callback ) noexcept
{
    std::unique_lock lock( m_mutex );

    auto it = std::find( m_callbacks.begin( ), m_callbacks.end( ), callback );
    if ( it != m_callbacks.end( ) )
    {
        m_callbacks.erase( it );
        return true;
    }
    return false;
}

void callback_system::erase_all( callback_type type ) noexcept
{
    std::unique_lock lock( m_mutex );

    m_callbacks.erase(
        std::remove_if( m_callbacks.begin( ), m_callbacks.end( ),
        [ type ]( const callback_t& cb ) { return cb.type == type; } ),
        m_callbacks.end( )
    );
}

void callback_system::call( callback_type type ) const
{
    std::vector<std::function<void( )>> to_call;
    {
        std::shared_lock lock( m_mutex );

        for ( const auto& cb : m_callbacks )
        {
            if ( cb.type == type && cb )
            {
                to_call.push_back( cb.function );
            }
        }
    }

    for ( auto& fn : to_call )
    {
        try
        {
            fn( );
        }
        catch ( ... )
        {
            
        }
    }
}

void callback_system::clear( ) noexcept
{
    std::unique_lock lock( m_mutex );
    m_callbacks.clear( );
}

bool callback_system::empty( ) const noexcept
{
    std::shared_lock lock( m_mutex );
    return m_callbacks.empty( );
}

size_t callback_system::count( callback_type type ) const noexcept
{
    std::shared_lock lock( m_mutex );
    return std::count_if( m_callbacks.begin( ), m_callbacks.end( ),
                          [ type ]( const callback_t& cb ) { return cb.type == type; } );
}