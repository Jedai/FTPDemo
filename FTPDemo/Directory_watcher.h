#pragma once

#include "stdafx.h"

typedef struct _Watcher_str
{
	wchar_t* Current_dir;
	CDialog* dlg;
	CRITICAL_SECTION List_lock;

} Watcher_str;



DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);
