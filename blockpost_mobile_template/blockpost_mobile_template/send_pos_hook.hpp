#pragma once
#include "vec3.hpp"

namespace send_pos_hook
{
	extern vec3_t xxx;
	extern bool cancel;

	extern void ( *send_pos_o )( void* client, float xx, float xy, float xz, float lx, float ly, unsigned char bitmask );
	extern void hook();
}