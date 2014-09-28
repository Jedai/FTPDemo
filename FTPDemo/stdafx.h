#pragma once

#define _CRT_SECURE_NO_WARNINGS


#define _WIN32_WINNT 0x0600

#include <afxwin.h>

#include <vector>

#ifndef RC_INVOKED
#include <afxcmn.h>
#include "CStyledListCtrl.h"
#endif

#include <WinInet.h>
#pragma comment(lib,"wininet.lib")


#include "definitions.h"


void CheckDirectoryChanges(LPVOID);
