#pragma once

#define _CRT_SECURE_NO_WARNINGS


#define _WIN32_WINNT 0x0600

#include <afxwin.h>


#ifndef RC_INVOKED
#include <afxlistctrl.h>
//#include <afxcmn.h> 
#endif

#include <WinInet.h>
#pragma comment(lib,"wininet.lib")


#include "definitions.h"


void CheckDirectoryChanges(LPVOID);
