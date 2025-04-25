#pragma once

#include <vector>
#include <string>
#include <Windows.h>
#include <initializer_list>
#include <ranges>

#define IAPI(str) GetProcAddress( GetModuleHandle( "GameAssembly.dll" ), str )
