// Sleep
#include <windows.h>

#include <winsock.h> 


#include <cstdio>
//ReadTemperature
#include "realtemp.h"
#include "telnet.h"





int main( void )
{
	//ShowWindow( GetConsoleWindow(), SW_HIDE );

	HANDLE hThread =( HANDLE ) _beginthread( TelnetThread, 0, NULL );

	bool telnet = true;

	unsigned int temp;
	unsigned int tempHigh = 80;
	unsigned int tempLow = 65;

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

		if (temp > tempHigh)
		{
			// w tym miejsciu ustawic cpu 10

			Sleep(10000);
			continue;
		}
		if (temp < tempLow)
		{
			// w tym miejsciu ustawic cpu 100

			Sleep(1000);
			continue;
		}
		

	}

	
    return 0;
}
