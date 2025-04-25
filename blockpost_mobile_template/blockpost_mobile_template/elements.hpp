#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace elements
{
    bool checkbox( const char* label, bool* v );
    bool slider_float( const char* label, float* v, float v_min, float v_max, bool otstup = false, const char* format = "%.3f", float power = 1.0f );
    bool slider_int( const char* label, int* v, int v_min, int v_max, bool otstup = false, const char* format = "%d", float power = 1.0f );
    void custom_separator( unsigned int color, float thickness = 1.f );
    void custom_separator_text( const char* label, unsigned int color );
    enum sep_mode
    {
        sep_mode_vertical,
        sep_mode_horizontal
    };
    void custom_separator_text_two_color( const char* label, unsigned int col_upr, unsigned int col_bot, sep_mode = sep_mode_vertical );
    void custom_separator_multi_color( unsigned int col_upr_left, unsigned int col_upr_right, unsigned int col_bot_right, unsigned int col_bot_left, float thickness = 1.f );
    void custom_separator_two_color( unsigned int col_upr, unsigned int col_bot, float thickness = 1.f );
    bool combo( const char* label, int* current_item, std::vector<std::string> items, bool otstup = false );
    bool mcombo( const char* label, std::unordered_map<std::string, bool*> data, bool otstup = false );
    bool color_picker( const char* name, float* color, bool alpha );
    bool button( const char* label, const ImVec2& size_arg = ImVec2( 0, 0 ) );
    bool tab_button( const char* label, bool selected, const ImVec2& size_arg = ImVec2( 0, 0 ) );
}


#pragma once
