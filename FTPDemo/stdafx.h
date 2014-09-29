#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define _WIN32_WINNT 0x0600


#include <afxwin.h>

#ifndef RC_INVOKED
#include <afxcmn.h>
#include "CStyledListCtrl.h"
#endif

#include <vector>

#include <WinInet.h>
#pragma comment(lib,"wininet.lib")


#include "definitions.h"


typedef struct _FILE_INFO
{
	wchar_t wszFileName[256];
	wchar_t wszFileDate[32];
	wchar_t wszFileSize[32];
	bool bReceived;
}FILE_INFO,*PFILE_INFO;


typedef struct _ITEM_DATA
{
	FILE_INFO fInfo;
	DWORD dwItemStyle;
}ITEM_DATA,*PITEM_DATA;
