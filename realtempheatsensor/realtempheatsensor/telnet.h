#pragma once
#include <winsock.h> 
// beginthread
#include <process.h>
//mutex
//#include <thread.h>
//debug
#include <cstdio>

#define MYPORT 3490    // port, z którym u¿ytkownicy bêd¹ siê ³¹czyli

#define BACKLOG 1     // jak du¿o mo¿e byæ po³¹czeñ oczekuj¹cych

void __cdecl TelnetThread( void * Args );

unsigned int ReadWord( SOCKET sock, char* buff, unsigned int max_len);

bool SendWord(SOCKET sock, char* buff);