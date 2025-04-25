#include "dx_hook.hpp"

#include <assert.h>
#include "minhook/mh.h"

#include <d3d11.h>
#include <dxgi.h>

#define KIERO_INCLUDE_D3D11 1
#define KIERO_USE_MINHOOK 1

namespace dx_hook
{
	uint64_t* methods_table = NULL;
	status init( )
	{
		WNDCLASSEX windowClass;
		windowClass.cbSize = sizeof( WNDCLASSEX );
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle( NULL );
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = "imgui_hook" ;
		windowClass.hIconSm = NULL;

		::RegisterClassEx( &windowClass );

		HWND window = ::CreateWindow( windowClass.lpszClassName, "imgui dx window", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL );

		HMODULE libD3D11;
		if ( ( libD3D11 = ::GetModuleHandle( "d3d11.dll" ) ) == NULL )
		{
			::DestroyWindow( window );
			::UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );
			return status::err;
		}

		void* D3D11CreateDeviceAndSwapChain;
		if ( ( D3D11CreateDeviceAndSwapChain = ::GetProcAddress( libD3D11, "D3D11CreateDeviceAndSwapChain" ) ) == NULL )
		{
			::DestroyWindow( window );
			::UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );
			return status::err;
		}

		D3D_FEATURE_LEVEL featureLevel;
		const D3D_FEATURE_LEVEL featureLevels[ ] = { D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_11_0 };

		DXGI_RATIONAL refreshRate;
		refreshRate.Numerator = 60;
		refreshRate.Denominator = 1;

		DXGI_MODE_DESC bufferDesc;
		bufferDesc.Width = 100;
		bufferDesc.Height = 100;
		bufferDesc.RefreshRate = refreshRate;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		DXGI_SAMPLE_DESC sampleDesc;
		sampleDesc.Count = 1;
		sampleDesc.Quality = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc = bufferDesc;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = window;
		swapChainDesc.Windowed = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		IDXGISwapChain* swapChain;
		ID3D11Device* device;
		ID3D11DeviceContext* context;

		if ( ( ( long( __stdcall* )(
			IDXGIAdapter*,
			D3D_DRIVER_TYPE,
			HMODULE,
			UINT,
			const D3D_FEATURE_LEVEL*,
			UINT,
			UINT,
			const DXGI_SWAP_CHAIN_DESC*,
			IDXGISwapChain**,
			ID3D11Device**,
			D3D_FEATURE_LEVEL*,
			ID3D11DeviceContext** ) )( D3D11CreateDeviceAndSwapChain ) )( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, &featureLevel, &context ) < 0 )
		{
			::DestroyWindow( window );
			::UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );
			return status::err;
		}

		methods_table = ( uint64_t* )::calloc( 205, sizeof( uint64_t ) );
		::memcpy( methods_table, *( uint64_t** ) swapChain, 18 * sizeof( uint64_t ) );
		::memcpy( methods_table + 18, *( uint64_t** ) device, 43 * sizeof( uint64_t ) );
		::memcpy( methods_table + 18 + 43, *( uint64_t** ) context, 144 * sizeof( uint64_t ) );

		MH_Initialize( );

		swapChain->Release( );
		swapChain = NULL;

		device->Release( );
		device = NULL;

		context->Release( );
		context = NULL;

		::DestroyWindow( window );
		::UnregisterClass( windowClass.lpszClassName, windowClass.hInstance );


		return status::done;
	}
	status hook( uint16_t index, void** orig, void* fn )
	{

		assert( index >= 0 && orig != NULL && fn != NULL );

		void* target = ( void* ) methods_table[ index ];
		if ( MH_CreateHook( target, fn, orig ) != MH_OK || MH_EnableHook( target ) != MH_OK )
		{
			return status::err;
		}
		return status::done;

		return status::err;
	}
}