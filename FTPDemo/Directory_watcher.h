#pragma once

#include "stdafx.h"

class CMainDlg;

typedef struct _Watcher_str
{
	wchar_t* Current_dir;
	CMainDlg* dlg;
	CRITICAL_SECTION List_lock;

} Watcher_str;



DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);
