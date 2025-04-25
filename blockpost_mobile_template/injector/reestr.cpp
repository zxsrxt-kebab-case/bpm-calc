#include "reestr.hpp"

bool delete_reg_key( HKEY hKeyRoot, const std::wstring& subKey )
{
    LONG result = RegDeleteTreeW( hKeyRoot, subKey.c_str( ) );
    if ( result == ERROR_SUCCESS )
        return true;
    
    return false;
}