#include "string.hpp"
#include <cstring>

std::string ustring::get_string( ) {
    std::u16string ss( ( char16_t* ) chars, 0, length );
    std::string str = utf16le_to_utf8( ss );
    return str;
}


static inline uint16_t byteswap_ushort( uint16_t number ) {
    return _byteswap_ushort( number );
}

std::string utf16_to_utf8( const std::u16string& u16str ) {
    if ( u16str.empty( ) ) {
        return std::string( );
    }
    char16_t bom = u16str[ 0 ];
    switch ( bom ) {
        case 0xFEFF:
            return utf16le_to_utf8( u16str );
            break;
        case 0xFFFE:
            return utf16be_to_utf8( u16str );
            break;
        default:
            return std::string( );
    }
}


std::string utf16le_to_utf8( const std::u16string& u16str ) {
    if ( u16str.empty( ) ) {
        return std::string( );
    }
    const char16_t* p = u16str.data( );
    std::u16string::size_type len = u16str.length( );
    if ( p[ 0 ] == 0xFEFF ) {
        p += 1;
        len -= 1;
    }

    std::string u8str;
    u8str.reserve( len * 3 );

    char16_t u16char;
    for ( std::u16string::size_type i = 0; i < len; ++i ) {
        u16char = p[ i ];

        if ( u16char < 0x0080 ) {
            u8str.push_back( ( char ) ( u16char & 0x00FF ) );
            continue;
        }
        if ( u16char >= 0x0080 && u16char <= 0x07FF ) {
            u8str.push_back( ( char ) ( ( ( u16char >> 6 ) & 0x1F ) | 0xC0 ) );
            u8str.push_back( ( char ) ( ( u16char & 0x3F ) | 0x80 ) );
            continue;
        }
        if ( u16char >= 0xD800 && u16char <= 0xDBFF ) {
            uint32_t highSur = u16char;
            uint32_t lowSur = p[ ++i ];
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            u8str.push_back( ( char ) ( ( codePoint >> 18 ) | 0xF0 ) );
            u8str.push_back( ( char ) ( ( ( codePoint >> 12 ) & 0x3F ) | 0x80 ) );
            u8str.push_back( ( char ) ( ( ( codePoint >> 06 ) & 0x3F ) | 0x80 ) );
            u8str.push_back( ( char ) ( ( codePoint & 0x3F ) | 0x80 ) );
            continue;
        }
        {
            u8str.push_back( ( char ) ( ( ( u16char >> 12 ) & 0x0F ) | 0xE0 ) );
            u8str.push_back( ( char ) ( ( ( u16char >> 6 ) & 0x3F ) | 0x80 ) );
            u8str.push_back( ( char ) ( ( u16char & 0x3F ) | 0x80 ) );
            continue;
        }
    }

    return u8str;
}


std::string utf16be_to_utf8( const std::u16string& u16str ) {
    if ( u16str.empty( ) ) {
        return std::string( );
    }
    const char16_t* p = u16str.data( );
    std::u16string::size_type len = u16str.length( );
    if ( p[ 0 ] == 0xFEFF ) {
        p += 1;
        len -= 1;
    }


    std::string u8str;
    u8str.reserve( len * 2 );
    char16_t u16char;
    for ( std::u16string::size_type i = 0; i < len; ++i ) {
        u16char = p[ i ];
        u16char = byteswap_ushort( u16char );

        if ( u16char < 0x0080 ) {
            u8str.push_back( ( char ) ( u16char & 0x00FF ) );
            continue;
        }
        if ( u16char >= 0x0080 && u16char <= 0x07FF ) {
            u8str.push_back( ( char ) ( ( ( u16char >> 6 ) & 0x1F ) | 0xC0 ) );
            u8str.push_back( ( char ) ( ( u16char & 0x3F ) | 0x80 ) );
            continue;
        }
        if ( u16char >= 0xD800 && u16char <= 0xDBFF ) {
            uint32_t highSur = u16char;
            uint32_t lowSur = byteswap_ushort( p[ ++i ] );
            uint32_t codePoint = highSur - 0xD800;
            codePoint <<= 10;
            codePoint |= lowSur - 0xDC00;
            codePoint += 0x10000;
            u8str.push_back( ( char ) ( ( codePoint >> 18 ) | 0xF0 ) );
            u8str.push_back( ( char ) ( ( ( codePoint >> 12 ) & 0x3F ) | 0x80 ) );
            u8str.push_back( ( char ) ( ( ( codePoint >> 06 ) & 0x3F ) | 0x80 ) );
            u8str.push_back( ( char ) ( ( codePoint & 0x3F ) | 0x80 ) );
            continue;
        }
        {
            u8str.push_back( ( char ) ( ( ( u16char >> 12 ) & 0x0F ) | 0xE0 ) );
            u8str.push_back( ( char ) ( ( ( u16char >> 6 ) & 0x3F ) | 0x80 ) );
            u8str.push_back( ( char ) ( ( u16char & 0x3F ) | 0x80 ) );
            continue;
        }
    }
    return u8str;
}