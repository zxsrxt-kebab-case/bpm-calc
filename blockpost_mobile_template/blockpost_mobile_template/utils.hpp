#pragma once
#include "imgui/imgui.h"

extern ImVec2 operator-( ImVec2 left, ImVec2 right );
extern ImVec2 operator+( ImVec2 left, ImVec2 right );
extern ImVec2 operator/( ImVec2 left, ImVec2 right );
extern ImVec2 operator*( ImVec2 left, ImVec2 right );
extern ImVec2 operator/( ImVec2 left, int right );
extern ImVec2 operator*( ImVec2 left, int right );

