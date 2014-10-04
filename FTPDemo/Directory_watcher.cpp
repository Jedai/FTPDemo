#include "stdafx.h"
#include "data.h"
#include "CMainDlg.h"
#include "CFTPWorker.h"

DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter)
{
	DWORD ret_bytes = 0;
	DWORD file_length = 0;
	DWORD offset_cur = 0;
	DWORD size_of_dir = 0x1000;

	int nIndex = -1; // must be INT, function can return with -1 in error case

	FILE_NOTIFY_INFORMATION* current_file_change_inf = 0;
	SYSTEMTIME sysTime = { 0 };

	wchar_t *pwszFileDate = nullptr;
	wchar_t *buf_file_name = nullptr;
	wchar_t buff[20];

	BOOL bWait = TRUE;

	PWATCHER_PARAM watch_str = (PWATCHER_PARAM)lpParameter;
	CMainDlg *pGUI = ((CMainDlg*)watch_str->dlg);


	// file create without errors ???
	HANDLE watch_dir = CreateFileW(watch_str->wszCurrentDir, FILE_LIST_DIRECTORY, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS,NULL );
	
	FILE_NOTIFY_INFORMATION* file_change_inf = (FILE_NOTIFY_INFORMATION*)VirtualAlloc(0, size_of_dir, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		
	while (bWait)
	{
		if (ReadDirectoryChangesW(watch_dir, file_change_inf, 0x1000, 0, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME, &ret_bytes, NULL, NULL))
		{
			// need to do do while for all files that can be in FILE_NOTIFY_INFORMATION
			current_file_change_inf = file_change_inf;
			do
			{
				file_length = current_file_change_inf->FileNameLength;
				buf_file_name = new wchar_t[file_length / sizeof(wchar_t)+1 + 1]; // COUNTOFWCHAR ?
				memcpy(buf_file_name, current_file_change_inf->FileName, file_length);
				buf_file_name[file_length / sizeof(wchar_t)] = 0;

				//EnterCriticalSection(&watch_str->csListLock);

				if ((nIndex = pGUI->GetFTPConnection()->GetIndexByName(buf_file_name)) > 0)
				{		// need lock ?
					// file is present in list and was downloaded from server
					if (pGUI->GetFTPConnection()->IsItemReceived(nIndex))
					{

						switch (current_file_change_inf->Action)
						{
						case FILE_ACTION_REMOVED:

							// mark it as 'not downloaded' with normal font
							EnterCriticalSection(&watch_str->csListLock);
							pGUI->GetFTPConnection()->SetItemReceived(nIndex, FALSE);
							LeaveCriticalSection(&watch_str->csListLock);

							pGUI->SetListItemStyle(nIndex, ~LIS_BOLD);

							// remove '*' if marked
							pGUI->SetListItemText(nIndex, 0, buf_file_name);

							break;

						case FILE_ACTION_MODIFIED:
											
							if (pGUI->GetFTPConnection()->IsItemChanged(nIndex))
							{
								wcscat(buf_file_name, L"*");
								pGUI->SetListItemText(nIndex, 0, buf_file_name);

								if ((pwszFileDate = pGUI->GetFTPConnection()->GetFileInfoByIndex(nIndex)->wszFileDate))
								{
									EnterCriticalSection(&watch_str->csListLock);

									memset(pwszFileDate, 0, wcslen(pwszFileDate)*sizeof(wchar_t));

									GetSystemTime(&sysTime);

									_itow(sysTime.wDay, buff, 10);
									wcscpy(pwszFileDate, buff);
									wcscat(pwszFileDate, L"/");
									_itow(sysTime.wMonth, buff, 10);
									wcscat(pwszFileDate, buff);
									wcscat(pwszFileDate, L"/");
									_itow(sysTime.wYear, buff, 10);
									wcscat(pwszFileDate, buff);
									wcscat(pwszFileDate, L"<->");
									_itow(sysTime.wHour, buff, 10);
									wcscat(pwszFileDate, buff);
									wcscat(pwszFileDate, L":");
									_itow(sysTime.wMinute, buff, 10);
									wcscat(pwszFileDate, buff);

									pGUI->SetListItemText(nIndex, 2, pwszFileDate);
									LeaveCriticalSection(&watch_str->csListLock);
								}
							}
							else
								pGUI->GetFTPConnection()->SetItemChanged(nIndex, TRUE);

							break;
						}
					}
				} // if file found

				//LeaveCriticalSection(&watch_str->csListLock);

				offset_cur = current_file_change_inf->NextEntryOffset;
				current_file_change_inf = (FILE_NOTIFY_INFORMATION*)((DWORD)current_file_change_inf + (DWORD)current_file_change_inf->NextEntryOffset);
				delete buf_file_name;
			} while (offset_cur);

		}
		else
		{
		}
	}

	VirtualFree((LPVOID)file_change_inf, size_of_dir, MEM_RELEASE);

	return 0;
}