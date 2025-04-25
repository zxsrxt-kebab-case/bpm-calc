#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <string>

namespace esp
{
	namespace draw
	{
		extern void box( ImRect rect, ImU32 color, bool outline = true );
		extern void nickname( ImRect rect, std::string name, ImU32 color, bool outline = true );
		extern void weapon( ImRect rect, std::string name, ImU32 color, bool outline = true );
		extern void health( ImRect rect, int hp, int armor, ImU32 color, bool outline = true );
	}
}