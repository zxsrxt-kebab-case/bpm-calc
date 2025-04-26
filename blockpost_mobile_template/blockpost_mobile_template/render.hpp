#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <string>
#include <functional>
#include <algorithm>
#include "ordered_map.hpp"
namespace render
{
	namespace fun
	{
		extern void box( ImRect rect, ImU32 color, bool outline = true );
		extern void nickname( ImRect rect, std::string name, ImU32 color, bool outline = true );
		extern void weapon( ImRect rect, std::string name, ImU32 color, bool outline = true );
		extern void health( ImRect rect, int hp, ImU32 color, bool outline = true );
	}
	class c_draw_queue
	{
	private:
		ordered_map<int, std::function<void( )>> draw_data;

	public:
		void add( std::function<void( )> fn )
		{
			draw_data.insert( draw_data.size() + 1, fn );
		}
		void draw( )
		{
			for ( auto element : draw_data )
			{
				element.second( );
			}
		}
	};
	class c_draw
	{
	private:
		std::vector<c_draw_queue> draw_queue;

	public:
		void add( c_draw_queue task )
		{
			draw_queue.push_back( task );
		}
		void send( )
		{
			for ( auto que : draw_queue )
			{
				que.draw( );
			}
			flush( );
		}
		void flush( )
		{
			draw_queue.clear( );
		}
	};
}