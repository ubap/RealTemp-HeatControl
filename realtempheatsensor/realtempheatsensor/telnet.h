#pragma once
#include <winsock.h> 
// beginthread
#include <process.h>

//debug
#include <cstdio>

#define MYPORT 3434   // port, z kt�rym u�ytkownicy b�d� si� ��czyli

#define BACKLOG 1     // jak du�o mo�e by� po��cze� oczekuj�cych

struct CriticalData_T
{
	unsigned int tempHigh;
	unsigned int tempLow;
	unsigned int temp;
	char* versionInfo;
	CRITICAL_SECTION cs;
};

// // wzkaznik na globalne dane
// sekcja krytyczna
static CriticalData_T *Data;

// socket z otwaortym polaczeniem
static int new_fd;

enum cmdret {KONIEC, POMOC, USTAW, BLAD, NIEZNANE, POKAZ, UKRYJ, TEMP};

void __cdecl TelnetThread( void * Args );

int ReadWord( SOCKET sock, char* buff, unsigned int max_len);

bool SendWord(SOCKET sock, char* buff);

cmdret ProcessCmd(char* cmd);