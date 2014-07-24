#include "telnet.h"

void __cdecl TelnetThread(void *Args)
{
	Data = (CriticalData_T*) Args;

	// glowna petla starajaca sie postawic server telnet
	while(1)
	{
		WSADATA wsaData;
		if( WSAStartup( MAKEWORD( 1, 1 ), & wsaData ) != 0 )
		{
			Sleep(1000);
			continue;
		}
		int sockfd;// nas³uchuj na sock_fd, nowe po³¹czenia na new_fd
		struct sockaddr_in my_addr; // informacja o moim adresie
		struct sockaddr_in their_addr; // informacja o adresie osoby ³¹cz¹cej siê
		int sin_size;

		sockfd = socket( AF_INET, SOCK_STREAM, 0 ); // zrób sprawdzanie b³êdów!

		my_addr.sin_family = AF_INET; // host byte order
		my_addr.sin_port = htons( MYPORT ); // short, network byte order
		my_addr.sin_addr.s_addr = INADDR_ANY; // uzupe³nij moim adresem IP
		memset( &( my_addr.sin_zero ), '\0', 8 ); // wyzeruj resztê struktury

		// nie zapomnij o sprawdzeniu b³êdów dla poni¿szych wywo³añ!
		if (bind( sockfd,( struct sockaddr * ) & my_addr, sizeof( struct sockaddr ) ) != 0 ||
			listen( sockfd, BACKLOG ) != 0)
		{
			Sleep(1000);
			continue;
		}

#define MAX_CMD_LEN 1024
#define TELNET_PASSWORD "123"

		// przyjmowanie klientow 1 by 1
		while(1)
		{
			sin_size = sizeof( struct sockaddr_in );
			new_fd = accept( sockfd,( struct sockaddr * ) &their_addr, &sin_size );
			printf( "server: got connection from %s\n", inet_ntoa( their_addr.sin_addr ) );

			char buff[MAX_CMD_LEN];
			SendWord(new_fd, "System kontrolowania mocy CPU.\n\rPodaj haslo:\n\r");

			ReadWord(new_fd, buff, MAX_CMD_LEN);
			if (strcmp(TELNET_PASSWORD, buff) != 0)
			{
				SendWord(new_fd, "Zle haslo!\n\r");
				closesocket(new_fd);
				Sleep(1000);
				continue;
			}

			// zalogowano
			SendWord(new_fd, "Zalogowano!\n\r");

			while(1)
			{
				int len = ReadWord(new_fd, buff, MAX_CMD_LEN);
				// error
				if (len == -1)
					break;
				printf("%d: [%s]\n", len, buff);
				ProcessCmd(buff);
			}

		}
	}

	_endthread();
	return;
}

// zwraca dlugosc wczytanego wyrazu, badz -1 dla bledu
int ReadWord( SOCKET sock, char* buff, unsigned int max_len)
{
	unsigned int i = 0;
	do
	{
		if (recv( sock, &buff[i], 1, 0) != 1)
			return -1;
		buff[i] = tolower(buff[i]);
		if (buff[i] != '\r')
		{
			++i;
		}
	} while(buff[i-1] != '\n' && buff[i-1] != ' ' && i <= max_len);
	buff[i-1] = 0;
	return i-1;
}

bool SendWord(SOCKET sock, char* buff)
{
	unsigned int len = strlen(buff);
	if (send( sock, buff, len, 0 ) != len)
		return false;

	return true;
}

cmdret ProcessCmd(char* cmd)
{
	// tutaj przetwarzanie polecen
	if (strcmp("koniec", cmd) == 0)
	{
		SendWord(new_fd, "Do widzenia!\n\r");
		closesocket(new_fd);
		return KONIEC;
	} else if (strcmp("pomoc", cmd) == 0)
	{
		SendWord(new_fd, "Dostepne polecenia:\n\rkoniec - konczy sesje telnet\n\rpomoc - wyswietla pomoc\n\rustaw temp_high/temp_low wartosc - ustawia wysoka/niska temperature\n\r");
		SendWord(new_fd, Data->versionInfo);
		return POMOC;
	} else if (strcmp("ustaw", cmd) == 0)
	{
		// odczytanie parametru
		if (ReadWord(new_fd, cmd, MAX_CMD_LEN) == -1)
			return BLAD;
		if (strcmp("temp_high", cmd) == 0)
		{
			if (ReadWord(new_fd, cmd, MAX_CMD_LEN) == -1)
				return BLAD;
			Data->tempHigh = atoi(cmd);
			char buff[10];
			SendWord(new_fd, "Temperatura wysoka ustawiona na: ");
			SendWord(new_fd, _itoa(Data->tempHigh, buff, 10));
			SendWord(new_fd, " C\n\r");

		} else if (strcmp("temp_low", cmd) == 0)
		{
			if (ReadWord(new_fd, cmd, MAX_CMD_LEN) == -1)
				return BLAD;
			Data->tempLow = atoi(cmd);
			char buff[10];
			SendWord(new_fd, "Temperatura wysoka ustawiona na: ");
			SendWord(new_fd, _itoa(Data->tempLow, buff, 10));
			SendWord(new_fd, " C\n\r");

		} else
			return BLAD;
		return USTAW;
	}
	
	return NIEZNANE;
}