#pragma once

#include "stdafx.h"
//#include <map>

typedef struct _Watcher_str
{
	CDialog* dlg;
	wchar_t* Current_dir;
	CRITICAL_SECTION List_lock;

} Watcher_str;

typedef struct _ITEM_DATA
{
	wchar_t File_name[256];
	DWORD Downloaded;
	DWORD index;
} ITEM_DATA;



DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);