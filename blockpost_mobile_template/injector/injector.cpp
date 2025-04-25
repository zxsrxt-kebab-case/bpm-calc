#include "injector.hpp"

#include <iostream>
#include <psapi.h>
#include <fstream>

namespace injector
{
    std::vector<DWORD> GetAllProcessIds( ) {
        DWORD processIds[ 1024 ], processCount;
        if ( !K32EnumProcesses( processIds, sizeof( processIds ), &processCount ) ) {
            std::cout << "EnumProcesses failed." << std::endl;
            return {};
        }

        processCount /= sizeof( DWORD );
        std::vector<DWORD> result( processIds, processIds + processCount );
        return result;
    }

    std::string GetProcessName( DWORD processId ) {
        HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId );
        if ( hProcess ) {
            char processName[ MAX_PATH ];
            if ( GetModuleFileNameExA( hProcess, NULL, processName, sizeof( processName ) / sizeof( char ) ) ) {
                CloseHandle( hProcess );
                return std::string( processName );
            }
            CloseHandle( hProcess );
        }
        return "";
    }

    std::wstring string_to_wstring( const std::string& str ) {
        int size_needed = MultiByteToWideChar( CP_UTF8, 0, str.c_str( ), ( int ) str.size( ), NULL, 0 );
        std::wstring wstr( size_needed, 0 );
        MultiByteToWideChar( CP_UTF8, 0, str.c_str( ), ( int ) str.size( ), &wstr[ 0 ], size_needed );

        return wstr;
    }

	status inject_dll( std::string path, int mode, DWORD pid )
	{
		if ( path.length( ) < 1 )
			return status::error;

        if ( !pid )
            return status::error;

        std::wstring dll_path = string_to_wstring( path );

        auto process = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid );
        if ( process == NULL )
            return status::error;

        const auto path_size = wcslen( dll_path.c_str( ) ) * sizeof( wchar_t );
        const auto remote_path = VirtualAllocEx( process, NULL, path_size,
                                                 MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );
        if ( remote_path == NULL )
        {
            CloseHandle( process );
            return status::error;
        }
        if ( !WriteProcessMemory( process, remote_path, dll_path.c_str( ), path_size,
             NULL ) )
        {
            VirtualFreeEx( process, remote_path, 0, MEM_RELEASE );
            CloseHandle( process );
            return status::error;
        }
        const auto kernel = GetModuleHandle( "kernel32.dll" );
        if ( kernel == NULL )
        {
            VirtualFreeEx( process, remote_path, 0, MEM_RELEASE );
            CloseHandle( process );
            return status::error;
        }
        const auto address = GetProcAddress( kernel, "LoadLibraryW" );
        if ( address == NULL )
        {
            VirtualFreeEx( process, remote_path, 0, MEM_RELEASE );
            CloseHandle( process );
            return status::error;
        }
        const auto thread = CreateRemoteThread( process, NULL, 0,
                                                ( LPTHREAD_START_ROUTINE ) address, remote_path, 0, NULL );
        if ( thread == NULL )
        {
            VirtualFreeEx( process, remote_path, 0, MEM_RELEASE );
            CloseHandle( process );
            return status::error;
        }
        WaitForSingleObject( thread, INFINITE );
        DWORD exit_code{};
        GetExitCodeThread( thread, &exit_code );
        CloseHandle( thread );
        VirtualFreeEx( process, remote_path, 0, MEM_RELEASE );
        CloseHandle( process );

		return status::success;
	}
}