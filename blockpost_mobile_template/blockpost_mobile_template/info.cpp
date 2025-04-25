#include "info.hpp"
#include "imgui/imgui.h"

namespace info
{
	screen_t screen;
	void init( )
	{
		auto io = ImGui::GetIO( );
		screen = { io.DisplaySize.x, io.DisplaySize.y };
	}
}