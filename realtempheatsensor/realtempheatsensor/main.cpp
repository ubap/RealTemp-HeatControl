// Sleep
#include <windows.h>

#include <winsock.h> 


#include <cstdio>
//ReadTemperature
#include "realtemp.h"
#include "telnet.h"


int main( void )
{
	ShowWindow( GetConsoleWindow(), SW_HIDE );

	CriticalData_T Data;
	
	Data.tempHigh = 80;
	Data.tempLow = 65;
	Data.versionInfo = "Wersja 1.1\n\r";

	HANDLE hThread = (HANDLE)_beginthread(TelnetThread, 0, &Data);

	unsigned int temp;

	while(1)
	{
		Sleep(1000);
		//system("cls");
		if (!ReadTemperature(&temp))
		{
			Sleep(5000);
			continue;
		}
		printf("max: %d C\n", temp);

		if (temp > Data.tempHigh)
		{
			// w tym miejsciu ustawic cpu 10
			system("powercfg -setacvalueindex SCHEME_CURRENT SUB_PROCESSOR PROCTHROTTLEMAX 10");
			system("powercfg -setactive SCHEME_CURRENT");
			Sleep(10000);
			continue;
		}
		if (temp < Data.tempLow)
		{
			// w tym miejsciu ustawic cpu 100
			system("powercfg -setacvalueindex SCHEME_CURRENT SUB_PROCESSOR PROCTHROTTLEMAX 100");
			system("powercfg -setactive SCHEME_CURRENT");
			Sleep(1000);
			continue;
		}
		

	}

	
    return 0;
}
