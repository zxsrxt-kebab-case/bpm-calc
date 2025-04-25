#pragma once
#include <string>

class ustring 
{
public:
    void* klass;
    void* monitor;
    int length;
    char chars[ 1 ];

    std::string get_string( );
};

std::string utf16_to_utf8( const std::u16string& u16str );

std::string utf16le_to_utf8( const std::u16string& u16str );

std::string utf16be_to_utf8( const std::u16string& u16str );

