#pragma once
#include <winsock.h> 
// beginthread
#include <process.h>
//mutex
//#include <thread.h>
//debug
#include <cstdio>

#define MYPORT 3490    // port, z kt�rym u�ytkownicy b�d� si� ��czyli

#define BACKLOG 1     // jak du�o mo�e by� po��cze� oczekuj�cych

void __cdecl TelnetThread( void * Args );

unsigned int ReadWord( SOCKET sock, char* buff, unsigned int max_len);

bool SendWord(SOCKET sock, char* buff);