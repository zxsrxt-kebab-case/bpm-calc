#pragma once

#include "vec3.hpp"

namespace plh_move_hook
{
	struct move_arg
	{
		vec3_t pos;
		float rx, ry;
		int bit;
	};
	extern move_arg mv;
	extern void ( *od_move )( void* plh, int id, vec3_t pos, float rx, float ry, int bitmask );
	extern void hook();
}
