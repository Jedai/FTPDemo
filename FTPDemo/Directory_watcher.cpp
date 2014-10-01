#include "stdafx.h"
#include "CMainDlg.h"
#include "Directory_watcher.h"


DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter)
{

	PWATCHER_PARAM watch_str = (PWATCHER_PARAM)lpParameter;
	bool bWait = true;

	HANDLE watch_dir = CreateFileW(watch_str->wszCurrentDir, FILE_LIST_DIRECTORY, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS,NULL );
	DWORD size_of_dir = 0x1000;
	FILE_NOTIFY_INFORMATION* file_change_inf = (FILE_NOTIFY_INFORMATION*)VirtualAlloc(0,size_of_dir,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	DWORD ret_bytes = 0;
	ReadDirectoryChangesW(watch_dir, file_change_inf, 0x1000, 0, FILE_NOTIFY_CHANGE_LAST_WRITE, &ret_bytes,NULL,NULL);


	DWORD file_length = 0;
	FILE_NOTIFY_INFORMATION* current_file_change_inf = 0;

	DWORD index_count = 0;
	DWORD index_in_map = 0;
	std::map<CString,DWORD>* list_cache;;
	std::map<CString,DWORD>::iterator it;;
	wchar_t* buf_file_name;

	DWORD offset_cur;

	while(bWait)
	{
		if( ReadDirectoryChangesW(watch_dir, file_change_inf, 0x1000, 0, FILE_NOTIFY_CHANGE_LAST_WRITE, &ret_bytes,NULL,NULL) )
		{
			current_file_change_inf = file_change_inf;
			list_cache = &((CMainDlg*)watch_str->dlg)->List_cache;
			do
			{
				file_length = current_file_change_inf->FileNameLength;
				buf_file_name = new wchar_t [file_length/sizeof(wchar_t) + 1 + 1];
				memcpy(buf_file_name,current_file_change_inf->FileName,file_length);
				buf_file_name[file_length/sizeof(wchar_t)] = 0;

				EnterCriticalSection(&watch_str->csListLock);

				//index_count = ((CMainDlg*)watch_str->dlg)->GetListCtrl()->GetItemCount();	
				it = list_cache->find(buf_file_name);
				if (it != list_cache->end())
				{
					index_in_map = (*it).second;
					if(index_in_map <= index_count)
					{
						buf_file_name[file_length/sizeof(wchar_t)] = '*';
						buf_file_name[file_length/sizeof(wchar_t) + 1*sizeof(wchar_t)] = 0;
						((CMainDlg*)watch_str->dlg)->SetListItemText(index_in_map, 0, buf_file_name); // remove before ????
					}
				}
				else
				{
					// error;
				}
				LeaveCriticalSection(&watch_str->csListLock);

				offset_cur = current_file_change_inf->NextEntryOffset;
				current_file_change_inf = (FILE_NOTIFY_INFORMATION*)((DWORD)current_file_change_inf + (DWORD)current_file_change_inf->NextEntryOffset);
				delete buf_file_name;
			}
			while ( offset_cur );

		}
		else
		{

		}
	}

	VirtualFree((LPVOID)file_change_inf,size_of_dir,MEM_RELEASE);

	return 0;
}