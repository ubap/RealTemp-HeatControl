// Sleep
#include <windows.h>

#include <winsock.h> 
#include <cstdio>
//ReadTemperature
#include "realtemp.h"
#include "telnet.h"

#define VERINFO "Wersja 1.2\n\r"

int main( void )
{
	ShowWindow( GetConsoleWindow(), SW_HIDE );

	CriticalData_T Data;
	
	Data.tempHigh = 80;
	Data.tempLow = 65;
	Data.versionInfo = VERINFO;

	InitializeCriticalSection(&Data.cs);

	HANDLE hThread = (HANDLE)_beginthread(TelnetThread, 0, &Data);
	int temp;
	bool fullThrottle = true;
	while(1)
	{
		Sleep(1000);

		// tylko 1 watek na raz
		EnterCriticalSection(&Data.cs);
		int success = ReadTemperature(&Data.temp);
		temp = Data.temp;
		int tempHigh = Data.tempHigh;
		int tempLow = Data.tempLow;
		LeaveCriticalSection(&Data.cs);

		printf("%d\n", temp);

		if (!success)
		{
			Sleep(5000);
			continue;
		}

		if (temp > tempHigh && fullThrottle)
		{
			// w tym miejsciu ustawic cpu 10
			system("powercfg -setacvalueindex SCHEME_CURRENT SUB_PROCESSOR PROCTHROTTLEMAX 10");
			system("powercfg -setactive SCHEME_CURRENT");
			fullThrottle = false;
			printf("przekroczono high (%d C)\n", tempHigh);
			Sleep(10000);
			continue;
		}
		if (temp < tempLow && !fullThrottle)
		{
			// w tym miejsciu ustawic cpu 100
			system("powercfg -setacvalueindex SCHEME_CURRENT SUB_PROCESSOR PROCTHROTTLEMAX 100");
			system("powercfg -setactive SCHEME_CURRENT");
			fullThrottle = true;
			printf("przekroczono low (%d C)\n", tempLow);
			continue;
		}
	}

    return 0;
}
