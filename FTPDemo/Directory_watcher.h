#pragma once

#include "stdafx.h"

typedef struct _Watcher_str
{
	wchar_t Current_dir[256];
	CDialog* dlg;
	CRITICAL_SECTION List_lock;

} Watcher_str;



DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);
