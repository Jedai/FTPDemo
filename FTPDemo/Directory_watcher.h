#pragma once

#include "stdafx.h"

typedef struct _WATCHER_PARAM
{
	wchar_t wszCurrentDir[256];
	CDialog* dlg;
	CRITICAL_SECTION csListLock;
} WATCHER_PARAM,*PWATCHER_PARAM;



DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);
