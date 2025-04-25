#include <iostream>
#include <Windows.h>
#include <thread>
#include <string>

#include "injector.hpp"
#include "reestr.hpp"

#define ERR(str)	std::cout << str << std::endl << std::endl \
					<< "closing after 2sec" << std::endl; \
					std::this_thread::sleep_for( std::chrono::seconds( 2 ) ); \
					return FALSE;
#define SUC(str)	std::cout << str << std::endl << std::endl \
					<< "closing after 2sec" << std::endl; \
					std::this_thread::sleep_for( std::chrono::seconds( 2 ) ); \
					return TRUE;

std::string get_cur_path( ) {
	TCHAR buffer[ MAX_PATH ] = { 0 };
	GetModuleFileName( NULL, buffer, MAX_PATH );
	std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
	return std::string( buffer ).substr( 0, pos );
}

int main( )
{
	setlocale( LC_ALL, "" );
	SetConsoleTitle( "zxsrxt injector" );
	int hr = -1;

	std::cout << "select action... inject( 1 - launcher, 2 - steam ) unban pc - 3:" << std::endl;
	std::cin >> hr;

	if ( hr != 1 && hr != 2 && hr != 3 )
	{
		ERR( "selected wrong key" );
	}

	if ( hr == 3 )
	{
		std::wstring reg_path = L"SOFTWARE\\Skullcap Studios\\BLOCKPOST MOBILE";
		std::cout << "\n\t" << ( delete_reg_key( HKEY_CURRENT_USER, reg_path ) ? "Done" : "Error" ) << std::endl;

		std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
		return 3;
	}
	
	std::cout << "\nWaiting for game..." << std::endl;

	DWORD pid = 0;
	while ( pid == 0 )
	{
		std::vector<DWORD> pids = injector::GetAllProcessIds( );
		for ( auto pidd : pids )
		{
			auto name = injector::GetProcessName( pidd );
			if ( strstr( name.c_str( ), hr == 1 ? "BLOCKPOSTMOBILE.exe" : "BPM.exe" ) )
			{
				pid = pidd;
				break;
			}
		}
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << "\nGame found!" << std::endl << "\nInjecting after 2 second" << std::endl;
	std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
	if ( injector::inject_dll( get_cur_path( ) + ( hr == 1 ? "\\launcher_ver.dll" : "\\steam_ver.dll" ), hr, pid ) != injector::status::success )
	{
		ERR( "error injecting" );
	}

	SUC( "\nAll done, Enjoy!" );
}
