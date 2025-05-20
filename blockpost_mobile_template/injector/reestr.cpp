#include "reestr.hpp"

bool delete_reg_key( HKEY hKeyRoot, const std::wstring& subKey )
{
    return RegDeleteTreeW( hKeyRoot, subKey.c_str( ) ) == ERROR_SUCCESS;
}