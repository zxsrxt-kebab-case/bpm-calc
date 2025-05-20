#pragma once
#include "c_player_data.hpp"
#include "il2cpp_array.hpp"
#include "c_camera.hpp"
namespace esp
{
	extern void run( );
	extern void push( c_player_data* local, il2cpp_array<c_player_data*>* players, c_camera* camera );
}

