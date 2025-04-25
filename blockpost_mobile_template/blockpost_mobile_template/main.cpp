#include <Windows.h>

#include "dx_hook.hpp"
#include "raycast_hook.hpp"
#include "plh_update_hook.hpp"
#include "plh_move_hook.hpp"
#include "send_pos_hook.hpp"

#include "menu.hpp"
#include "il2cpp.hpp"
#include "info.hpp"
#include "mem.hpp"
#include "vars.hpp"
#include "vec3.hpp"

#include "fonts.hpp"
#include "fonts_data.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx11.h"
#include "imgui/impl/imgui_impl_win32.h"

#include <d3d11.h>
#include <dxgi.h>
#include <thread>

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

typedef HRESULT( __stdcall* Present ) ( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags );
typedef LRESULT( CALLBACK* WNDPROC )( HWND, UINT, WPARAM, LPARAM );

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void init_imgui( )
{
	ImGui::CreateContext( );
	ImGuiIO& io = ImGui::GetIO( );
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

	io.Fonts->AddFontDefault( );

	fonts::tahoma_bold = io.Fonts->AddFontFromMemoryTTF( tahoma_bold_data, sizeof tahoma_bold_data, 60, NULL, io.Fonts->GetGlyphRangesCyrillic( ) );
	fonts::pixel = io.Fonts->AddFontFromMemoryTTF( pixel_font, sizeof pixel_font, 60, NULL, io.Fonts->GetGlyphRangesCyrillic( ) );

	ImGui_ImplWin32_Init( window );
	ImGui_ImplDX11_Init( pDevice, pContext );
}

LRESULT __stdcall WndProc( const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {

	if ( true && ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
		return true;

	return CallWindowProc( oWndProc, hWnd, uMsg, wParam, lParam );
}

HRESULT __stdcall hkPresent( IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags )
{

	//Flags &= ~DXGI_PRESENT_DO_NOT_SEQUENCE;

	static bool init = false;
	if ( !init )
	{
		if ( SUCCEEDED( pSwapChain->GetDevice( __uuidof( ID3D11Device ), ( void** ) &pDevice ) ) )
		{
			pDevice->GetImmediateContext( &pContext );
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc( &sd );
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &pBackBuffer );
			pDevice->CreateRenderTargetView( pBackBuffer, NULL, &mainRenderTargetView );
			pBackBuffer->Release( );
			oWndProc = ( WNDPROC ) SetWindowLongPtr( window, GWLP_WNDPROC, ( LONG_PTR ) WndProc );
			init_imgui( );
			init = true;
		}

		else
			return oPresent( pSwapChain, SyncInterval, Flags );
	}

	ImGui_ImplDX11_NewFrame( );
	ImGui_ImplWin32_NewFrame( );
	ImGui::NewFrame( );

	menu::run( );
	ImGui::Render( );

	pContext->OMSetRenderTargets( 1, &mainRenderTargetView, NULL );
	ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
	return oPresent( pSwapChain, SyncInterval, Flags );
}

void init_all( )
{
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	mem::init( );
	il2cpp::init( );
	info::init( );
	std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	raycast_hook::hook( );
	plh_update_hook::hook( );
	plh_move_hook::hook( );
	send_pos_hook::hook( );
}

DWORD WINAPI MainThread( LPVOID lpReserved )
{
	bool init_hook = false;
	do
	{
		if ( dx_hook::init( ) == dx_hook::status::done )
		{
			dx_hook::hook( 8, ( void** ) &oPresent, hkPresent );
			std::thread( init_all ).detach( );

			MH_EnableHook( MH_ALL_HOOKS );
			init_hook = true;
		}
	} while ( !init_hook );
	return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
		DisableThreadLibraryCalls( hModule );
		CreateThread( nullptr, 0, MainThread, hModule, 0, nullptr );
		return TRUE;
    }

	return TRUE;
}

