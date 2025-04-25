#include "menu.hpp"
#include "vars.hpp"
#include "esp.hpp"
#include "aim.hpp"
#include "mem.hpp"
#include "info.hpp"
#include "thirdperson.hpp"
#include "il2cpp.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "c_camera.hpp"

#include <Windows.h>

#include <mutex>
#include <thread>

#include "c_material.hpp"

#include "elements.hpp"

il2cpp_method* mthd228 = nullptr;

namespace menu
{
	bool menu_opened = false;
	
	void handle_open( )
	{
		if ( ImGui::IsKeyPressed( ImGuiKey_Insert ) )
			menu_opened = !menu_opened;
	}

	
	void run( )
	{

		esp::run( );
		silent::run( );

		if(variables::silent || variables::psilent) ImGui::GetBackgroundDrawList( )->AddCircle( { info::screen.x / 2, info::screen.y / 2 }, static_cast<float>(variables::aim_fov), ImColor( 255, 255, 255 ), 999 );
		
		handle_open( );

		if ( !menu_opened )return;

		ImGui::SetNextWindowSize( { 300, 400 }, ImGuiCond_Once );

		ImGui::Begin( "blockpost mobile template", nullptr );
		if ( ImGui::BeginTabBar( "cheat" ) )
		{
			if ( ImGui::BeginTabItem( "visual" ) )
			{
				if ( ImGui::BeginTabBar( "visual settings" ) )
				{
					if ( ImGui::BeginTabItem( "esp" ) )
					{
						elements::checkbox( "esp", &variables::esp );
						elements::checkbox( "team check", &variables::teamcheck );
						ImGui::EndTabItem( );
					}
					if ( ImGui::BeginTabItem( "chams" ) )
					{
						elements::checkbox( "enemy chams", &variables::chams::enemy::enable );
						elements::checkbox( "local chams", &variables::chams::local::enable );
						ImGui::EndTabItem( );
					}				
					if ( ImGui::BeginTabItem( "world" ) )
					{
						elements::checkbox( "thirdperson", &variables::thirdperson );
						elements::slider_float( "distance", &variables::third_distance, 0, 10 );
						ImGui::EndTabItem( );
					}
					ImGui::EndTabBar( );
				}
				ImGui::EndTabItem( );
			}
			if ( ImGui::BeginTabItem( "aim" ) )
			{
				elements::checkbox( "silent aimbot", &variables::silent );
				elements::checkbox( "perfect silent", &variables::psilent );
				elements::checkbox( "autofire", &variables::autofire );
				elements::checkbox( "autostop", &variables::autostop );
				elements::checkbox( "autoscope", &variables::autoscope );
				elements::slider_int( "fov", &variables::aim_fov, 0, 360, "%dut" );
				ImGui::EndTabItem( );
			}
			if ( ImGui::BeginTabItem( "movement" ) )
			{
				elements::checkbox( "fly", &variables::flyhack );
				elements::checkbox( "bhop", &variables::bunnyhop );
				elements::checkbox( "autostrafe", &variables::autostrafe );
				ImGui::EndTabItem( );
			}
			if ( ImGui::BeginTabItem( "weapon" ) )
			{
				elements::checkbox( "no recoil", &variables::weapons::no_recoil );
				elements::checkbox( "no spread", &variables::weapons::no_spread );
				ImGui::EndTabItem( );
			}
			if ( ImGui::BeginTabItem( "antiaims" ) )
			{
				namespace var = variables::antiaims;
				namespace varv = variables::antiaims::visual;
				elements::checkbox( "enabler", &var::enable );
				elements::combo( "type", &var::type, { "visual", "real" } );

				if ( var::type == 0 )
				{
					if ( ImGui::CollapsingHeader( "selectors" ) )
					{
						elements::combo( "yaw type", ( int* ) &varv::yaw_type, { "static", "jitter", "spin" } );
						if ( varv::yaw_type == varv::yaw_type_en::yaw_static )
						{
							elements::slider_int( "yaw", &varv::static_yaw, -180, 180 );
						}
						if ( varv::yaw_type == varv::yaw_type_en::yaw_jitter )
						{
							elements::slider_int( "yaw", &varv::jitter_yaw, -90, 90 );
						}
						elements::combo( "pitch type", ( int* ) &varv::pitch_type, { "static", "jitter" } );
						if ( varv::pitch_type == varv::pitch_type_en::pitch_static )
						{
							elements::slider_int( "pitch", &varv::static_pitch, -90, 90 );
						}
						if ( varv::pitch_type == varv::pitch_type_en::pitch_jitter )
						{
							elements::slider_int( "pitch", &varv::jitter_pitch, -90, 90 );
						}
					}
				}
				elements::checkbox( "fakeduck", &var::fakeduck );
				ImGui::EndTabItem( );
			}
			ImGui::EndTabBar( );
		}

		ImGui::End( );
	}
}