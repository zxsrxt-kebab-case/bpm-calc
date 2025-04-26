#include "on_gui_hook.hpp"
#include "il2cpp.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_win32.h"



void on_gui_hook::hook( )
{
	il2cpp_assembly::open( "Assembly-CSharp" )->image( )->get_class( "", "GuiMain" )->get_method( "OnGui", 0 )->hook<&h_on_gui>( &o_on_gui );
}