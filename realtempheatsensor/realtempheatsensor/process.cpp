#include "process.h"


// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

TCHAR* _GetProcessName( DWORD processID, TCHAR* szProcessName)
{
    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, 
                               MAX_PATH );
        }
    }

    // Release the handle to the process.

    CloseHandle( hProcess );

	return( szProcessName );
}


DWORD _GetProcessId( TCHAR* processName )
{
	// Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
	// * try to match

	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");


    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
			_GetProcessName(aProcesses[i], szProcessName);
			if (_tcscmp(szProcessName, processName) == 0)
			{
				return aProcesses[i];
			}
        }
    }

	// jezeli nie znaleziono
	return NULL;
}