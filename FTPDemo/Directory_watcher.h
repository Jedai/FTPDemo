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


#define FILE_DOWNLOADING_FLAG 0
#define FILE_DOWNLOADED_FLAG 1
#define FILE_DOWNLOADING_FONT_CHANGED_FLAG 2
#define FILE_DOWNLOADED_DELETED_FLAG 3
#define FILE_DOWNLOADED_CHANGED_FLAG 4

DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);