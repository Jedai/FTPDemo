#include "stdafx.h"
#include "CMainDlg.h"
#include "Directory_watcher.h"
#include "CFTPWorker.h"

DWORD WINAPI Directory_watcher_thread(LPVOID lpParameter)
{

	Watcher_str* watch_str = (Watcher_str*)lpParameter;
	HANDLE chng;
	bool bWait = true;

	HANDLE watch_dir = CreateFileW(watch_str->Current_dir, FILE_LIST_DIRECTORY, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS,NULL );
	DWORD size_of_dir = 0x1000;
	FILE_NOTIFY_INFORMATION* file_change_inf = (FILE_NOTIFY_INFORMATION*)VirtualAlloc(0,size_of_dir,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	DWORD ret_bytes = 0;
//	ReadDirectoryChangesW(watch_dir, file_change_inf, 0x1000, 0, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |FILE_NOTIFY_CHANGE_FILE_NAME, &ret_bytes,NULL,NULL);

	DWORD file_length = 0;
	FILE_NOTIFY_INFORMATION* current_file_change_inf = 0;

	DWORD index_count = 0;
	DWORD index_in_map = 0;
			//	std::map<CString,DWORD>* list_cache;;
			//	std::map<CString,DWORD>::iterator it;;

///////////////////////////
	std::vector<ITEM_DATA>* file_names_cache; 
	std::vector<ITEM_DATA>::iterator it; 
////////////////////////
	wchar_t* buf_file_name;

	DWORD offset_cur;

	while(bWait)
	{
		if( ReadDirectoryChangesW(watch_dir, file_change_inf, 0x1000, 0, FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_CREATION, &ret_bytes,NULL,NULL) )
		{
			// need to do do while for all files that can be in FILE_NOTIFY_INFORMATION
			current_file_change_inf = file_change_inf;	
			do
			{
				file_length = current_file_change_inf->FileNameLength;
				buf_file_name = new wchar_t [file_length/sizeof(wchar_t) + 1 + 1];
				memcpy(buf_file_name,current_file_change_inf->FileName,file_length);
				buf_file_name[file_length/sizeof(wchar_t)] = 0;



		//			list_cache = &((CMainDlg*)watch_str->dlg)->List_cache;

						/////////////////NEW CODE///////////////
				file_names_cache = &((CMainDlg*)watch_str->dlg)->List_cache;
//////////////////////////
				index_count = ((CMainDlg*)watch_str->dlg)->pListBox->GetItemCount();	
			//			it = list_cache->find(buf_file_name);

					switch(current_file_change_inf->Action)
					{
					case FILE_ACTION_REMOVED:
/////////////////NEW CODE///////////////
			//	for(DWORD i =0; i < file_names_cache->size();i++)
				for(it  = file_names_cache->begin(); it != file_names_cache->end();it++)
				{
					if( !wcscmp((*it).File_name,buf_file_name) )
					{
						// need lock
						 (*it).Downloaded = FILE_DOWNLOADED_DELETED_FLAG;
						((CMainDlg*)watch_str->dlg)->pListBox->DeleteItem((*it).index);
						((CMainDlg*)watch_str->dlg)->pListBox->InsertItem((*it).index,buf_file_name);	//Change font
					}

				}

						break;
					case FILE_ACTION_MODIFIED:
					case	FILE_ACTION_ADDED:
						/////////////////NEW CODE///////////////
///////////////////////////
			//	for(DWORD i =0; i < file_names_cache.size();i++)
				for(it  = file_names_cache->begin(); it != file_names_cache->end();it++)
				{
						if((*it).Downloaded == FILE_DOWNLOADED_DELETED_FLAG || (*it).Downloaded == FILE_DOWNLOADED_CHANGED_FLAG)
							break;

						if( !wcscmp((*it).File_name,buf_file_name) )
						{
							if(  (*it).Downloaded  == FILE_DOWNLOADING_FLAG && current_file_change_inf->Action  == FILE_ACTION_ADDED )
							{
								((CMainDlg*)watch_str->dlg)->pListBox->DeleteItem((*it).index);
								((CMainDlg*)watch_str->dlg)->pListBox->InsertItem((*it).index,buf_file_name);	//Change font
								// need lock
							//	if( file_names_cache[i].Downloaded  == FILE_DOWNLOADED_FLAG )
									 (*it).Downloaded = FILE_DOWNLOADING_FONT_CHANGED_FLAG;

							}
							else
							{
								if( (*it).Downloaded = FILE_DOWNLOADED_FLAG )
								{
									buf_file_name[file_length/sizeof(wchar_t)] = L'*';
									buf_file_name[file_length/sizeof(wchar_t) + 1] = 0;
									((CMainDlg*)watch_str->dlg)->pListBox->DeleteItem((*it).index);
									((CMainDlg*)watch_str->dlg)->pListBox->InsertItem((*it).index,buf_file_name);
									(*it).Downloaded = FILE_DOWNLOADED_CHANGED_FLAG;
								}
							}
						}

				}
////////////////////////

						break;
					}
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