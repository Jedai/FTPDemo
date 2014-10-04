#pragma once

#include <afxwin.h>
#include <vector>

typedef struct _FILE_INFO
{
	wchar_t wszFileName[256];
	wchar_t wszFileDate[32];
	wchar_t wszFileSize[32];
	BOOL bReceived;
	BOOL bChanged;
}FILE_INFO, *PFILE_INFO;


typedef struct _ITEM_DATA
{
	DWORD dwItemStyle;
}ITEM_DATA, *PITEM_DATA;


// thread function params

typedef struct _WAIT_PARAM
{
	wchar_t wszDlgCaption[32];
	CDialog *parent;
}WAIT_PARAM,*PWAIT_PARAM;


typedef struct _WATCHER_PARAM
{
	wchar_t wszCurrentDir[256];
	CDialog* dlg;
	CRITICAL_SECTION csListLock;
} WATCHER_PARAM, *PWATCHER_PARAM;


// thread functions

DWORD WINAPI WaitThread(LPVOID);
DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter);

