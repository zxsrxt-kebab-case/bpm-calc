#pragma once
#include "vec3.hpp"
#include "imgui/imgui.h"

namespace math
{
	float dot( const vec3_t& left, const vec3_t& right );
	bool in_fov( ImVec2 pss, int fov );
	float distance_center( ImVec2 pss );
}
