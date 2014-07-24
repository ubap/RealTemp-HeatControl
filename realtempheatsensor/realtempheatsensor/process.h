#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>


TCHAR* _GetProcessName( DWORD processID, TCHAR* szProcessName);

DWORD _GetProcessId( TCHAR* processName );
