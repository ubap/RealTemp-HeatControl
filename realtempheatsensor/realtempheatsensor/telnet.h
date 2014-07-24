#pragma once
#include <winsock.h> 
// beginthread
#include <process.h>
//mutex
//#include <thread.h>
//debug
#include <cstdio>

#define MYPORT 3491    // port, z kt�rym u�ytkownicy b�d� si� ��czyli

#define BACKLOG 1     // jak du�o mo�e by� po��cze� oczekuj�cych

struct CriticalData_T
{
	unsigned int tempHigh;
	unsigned int tempLow;
	char* versionInfo;
};

// // wzkaznik na globalne dane
// sekcja krytyczna
static CriticalData_T *Data;

// socket z otwaortym polaczeniem
static int new_fd;

enum cmdret {KONIEC, POMOC, USTAW, BLAD, NIEZNANE};

void __cdecl TelnetThread( void * Args );

int ReadWord( SOCKET sock, char* buff, unsigned int max_len);

bool SendWord(SOCKET sock, char* buff);

cmdret ProcessCmd(char* cmd);