#include "send_attackv2_hook.hpp"
#include "send_pos_hook.hpp"
#include "minhook/mh.h"
#include "mem.hpp"
#include "aim.hpp"
#include "globals.hpp"
#include "vars.hpp"
#include "vec4.hpp"
#include "il2cpp.hpp"
#include "c_controll.hpp"

namespace send_attackv2_hook
{
	void* inst;

	void ( *o_send_attack )( void* m );
	void h_send_attack( void* m )
	{
		inst = m;

		g_ctx.callbacks.call( callback_type::attack );
		g_ctx.callbacks.erase_all( callback_type::attack );

		o_send_attack( m );
	}

	void hook( )
	{
		auto addr = reinterpret_cast< LPVOID >( mem::game_assembly + 0xFCA610 );
		MH_CreateHook( addr, h_send_attack, reinterpret_cast< LPVOID* >( &o_send_attack ) );
		MH_EnableHook( addr );
	}
}