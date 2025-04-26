#pragma once
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <initializer_list>

template <typename __key, typename __value>
class ordered_map
{
private:
    std::vector<__key> _order;
    std::unordered_map<__key, __value> _data;

public:
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<__key, __value>;
        using difference_type = ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        iterator( typename std::vector<__key>::const_iterator order_it,
                  const std::unordered_map<__key, __value>& data )
            : _order_it( order_it ), _data( data )
        { }

        value_type operator*( ) const
        {
            return { *_order_it, _data.at( *_order_it ) };
        }

        iterator& operator++( )
        {
            ++_order_it;
            return *this;
        }

        bool operator!=( const iterator& other ) const
        {
            return _order_it != other._order_it;
        }

    private:
        typename std::vector<__key>::const_iterator _order_it;
        const std::unordered_map<__key, __value>& _data;
    };

    ordered_map( ) = default;

    ordered_map( std::initializer_list<std::pair<__key, __value>> init_list )
    {
        for ( const auto& pair : init_list )
        {
            if ( _data.find( pair.first ) == _data.end( ) )
            {
                _order.push_back( pair.first );
                _data[ pair.first ] = pair.second;
            }
        }
    }

    iterator begin( ) const
    {
        return iterator( _order.begin( ), _data );
    }
    iterator end( ) const
    {
        return iterator( _order.end( ), _data );
    }

    void insert( const __key& key, const __value& value )
    {
        if ( !contains( key ) )
        {
            _order.push_back( key );
            _data[ key ] = value;
        }
    }

    __value& at( const __key& key )
    {
        return _data.at( key );
    }

    const __value& at( const __key& key ) const
    {
        return _data.at( key );
    }

    bool contains( const __key& key ) const
    {
        return _data.find( key ) != _data.end( );
    }

    void erase( const __key& key )
    {
        auto it = std::ranges::find( _order, key );
        if ( it != _order.end( ) )
        {
            _order.erase( it );
            _data.erase( key );
        }
    }

    const std::vector<__key>& keys( ) const
    {
        return _order;
    }

    size_t size( ) const
    {
        return _order.size( );
    }
};