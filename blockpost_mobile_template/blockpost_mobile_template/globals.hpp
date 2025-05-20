#pragma once
#include "render.hpp"
#include "callbacks.hpp"

class g_ctx_t
{
public:
	render::c_draw draw; //deprecated but ok
	callback_system callbacks;
};

extern g_ctx_t g_ctx;