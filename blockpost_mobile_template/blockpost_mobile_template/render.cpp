#include "render.hpp"
#include <vector>
#include <utility>
#include <algorithm>
#include "fonts.hpp"

//#include "utils.hpp"

namespace esp
{
	namespace draw
	{
        void draw_text( ImFont* font, float size, const ImVec2& pos, ImU32 textColor, const char* text, bool outline = false, float outlineThickness = 1.0f, ImDrawList* drawList = ImGui::GetBackgroundDrawList( ) ) {
            if ( outline )
            {
                for ( float x = -outlineThickness; x <= outlineThickness; x += 1.0f ) {
                    for ( float y = -outlineThickness; y <= outlineThickness; y += 1.0f ) {
                        if ( x != 0.0f || y != 0.0f ) {
                            drawList->AddText( font, size, ImVec2( pos.x + x, pos.y + y ), ImColor( 0, 0, 0 ), text );
                        }
                    }
                }
            }

            drawList->AddText( font, size, pos, textColor, text );
        }

        ImColor color_lerp( ImColor a, ImColor b, float f ) {
            return { a.Value.x + f * ( b.Value.x - a.Value.x ), a.Value.y + f * ( b.Value.y - a.Value.y ), a.Value.z + f * ( b.Value.z - a.Value.z ), a.Value.w + f * ( b.Value.w - a.Value.w ) };
        }

		void box( ImRect rect, ImU32 color, bool outline )
		{
			auto draw = ImGui::GetBackgroundDrawList( );
            draw->AddRect( rect.Min, rect.Max, color );
            if ( outline )
            {
                draw->AddRect( rect.Min - ImVec2( 1, 1 ),
                               rect.Max + ImVec2( 1, 1 ),
                               ImColor( 0, 0, 0, 200 ) );
                draw->AddRect( rect.Min + ImVec2( 1, 1 ),
                               rect.Max - ImVec2( 1, 1 ),
                               ImColor( 0, 0, 0, 200 ) );
            }
		}
        void nickname( ImRect rect, std::string name, ImU32 color, bool outline )
        {
            auto textsize = fonts::tahoma_bold->CalcTextSizeA( 12, FLT_MAX, 0.f, name.c_str( ) );
            draw_text( fonts::tahoma_bold, 12, { rect.GetCenter( ).x - textsize.x / 2, rect.Min.y - 2 - textsize.y }, color, name.c_str( ), outline );
        }        
        void weapon( ImRect rect, std::string name, ImU32 color, bool outline )
        {
            auto textsize = fonts::tahoma_bold->CalcTextSizeA( 12, FLT_MAX, 0.f, name.c_str( ) );
            draw_text( fonts::tahoma_bold, 12, { rect.GetCenter( ).x - textsize.x / 2, rect.Max.y }, color, name.c_str( ), outline );
        }
        void health( ImRect rect, int hp, int armor, ImU32 color, bool outline ) {
            auto draw = ImGui::GetBackgroundDrawList( );
            float mult = std::clamp( hp, 0, 100 ) / 100.f;
            float armor_mult = std::clamp( armor, 0, 100 ) / 100.f;

            ImColor hpcolor = color_lerp( ImColor( 255, 0, 0 ), ImColor( 0, 255, 0 ), mult );

            ImVec2 bg_min = ImVec2(
                roundf( rect.Min.x - 5 ),
                roundf( rect.Min.y )
            );
            ImVec2 bg_max = ImVec2(
                roundf( rect.Min.x - 3 ),
                roundf( rect.Max.y )
            );

            draw->AddRectFilled( bg_min, bg_max, ImColor( 0, 0, 0, 200 ) );

            if ( outline ) {
                ImVec2 outline_min = ImVec2( bg_min.x - 1, bg_min.y - 1 );
                ImVec2 outline_max = ImVec2( bg_max.x + 1, bg_max.y + 1 );
                draw->AddRect( outline_min, outline_max, ImColor( 0, 0, 0, 200 ), 0.0f, 0, 1.0f );
            }

            float height = roundf(std::min( rect.GetHeight( ) * mult, bg_max.y - bg_min.y ));
            ImVec2 gradient_min = ImVec2( roundf(bg_min.x), roundf( bg_max.y - height ) );

            float armor_height = roundf( std::min( rect.GetHeight( ) * armor_mult, bg_max.y - bg_min.y ) );
            ImVec2 armor_min = ImVec2( roundf(bg_min.x), roundf( bg_max.y - armor_height ) );


            draw->AddRectFilled(
                armor_min,
                bg_max,
                ImColor( 0, 0, 255, 255 )
            );

            draw->AddRectFilledMultiColor(
                gradient_min,
                bg_max,
                hpcolor,
                hpcolor,
                ImColor( 255, 0, 0 ),
                ImColor( 255, 0, 0 )
            );

            if ( hp < 90 )
            {
                auto text = std::to_string( hp );
                auto text_size = fonts::pixel->CalcTextSizeA( 8, FLT_MAX, 0.f, text.c_str( ) );
                draw_text( fonts::pixel, 8, { bg_max.x - text_size.x / 2, gradient_min.y - text_size.y / 2 }, hpcolor, text.c_str( ), outline );
            }
        }
	}
}