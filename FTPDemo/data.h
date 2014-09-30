#pragma once

#include <afxwin.h>
#include <vector>

typedef struct _FILE_INFO
{
	wchar_t wszFileName[256];
	wchar_t wszFileDate[32];
	wchar_t wszFileSize[32];
	bool bReceived;
}FILE_INFO, *PFILE_INFO;


typedef struct _ITEM_DATA
{
	DWORD dwItemStyle;
}ITEM_DATA, *PITEM_DATA;
