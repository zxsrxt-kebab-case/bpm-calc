#pragma once
#include "Windows.h"
#include <string>

extern bool delete_reg_key( HKEY hKeyRoot, const std::wstring& subKey );