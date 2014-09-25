#include "stdafx.h"



void CheckDirectoryChanges(LPVOID pParam)
{
	bool bWait = true;

	if (!pParam)
		return;

	HANDLE chng = FindFirstChangeNotification((wchar_t*)pParam, false, FILE_NOTIFY_CHANGE_LAST_WRITE);

	while (bWait)
	{
		switch (WaitForSingleObject(chng, INFINITE))
		{
		case WAIT_OBJECT_0:
			// notify UI ?

			if (!FindNextChangeNotification(chng))
				bWait = false;
			break;
		}
	}

	FindCloseChangeNotification(chng);
}

