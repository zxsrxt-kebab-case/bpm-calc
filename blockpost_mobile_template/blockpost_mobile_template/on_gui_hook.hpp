#pragma once

namespace on_gui_hook
{
	extern void ( *o_on_gui )( void* );
	extern void h_on_gui ( void* obj );
	extern void hook( );
}