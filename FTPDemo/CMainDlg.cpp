#include "CMainDlg.h"
#include "CFTPWorker.h"



BEGIN_MESSAGE_MAP(CMainDlg,CDialog)
	ON_COMMAND(IDC_CONNECT, OnConnectButtonClick)
	ON_COMMAND(IDC_REFRESH, OnRefreshButtonClick)
	ON_COMMAND(IDC_OPEN, OnOpenButtonClick)
	ON_COMMAND(IDC_UPDATE, OnUpdateButtonClick)
	ON_COMMAND(IDC_EXIT, OnExitButtonClick)
END_MESSAGE_MAP()

CMainDlg::CMainDlg() : CDialog(CMainDlg::IDD)
{	
	ftp = 0;
}

CMainDlg::~CMainDlg()
{
	if (pListBox)
		delete pListBox;
	if (ftp)
		delete ftp;
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitializeCriticalSection(&struct_for_watcher.List_lock);

	pListBox = new CListCtrl();

	pEdit_FTP_server = (CEdit*)GetDlgItem(IDC_EDIT1);
	pButton_Refresh = (CButton*)GetDlgItem(IDC_REFRESH);
	pButton_Update = (CButton*)GetDlgItem(IDC_UPDATE);
	pButton_Open = (CButton*)GetDlgItem(IDC_OPEN);
	pButton_Exit = (CButton*)GetDlgItem(IDC_EXIT);
	pButton_Connect = (CButton*)GetDlgItem(IDC_CONNECT);

	pButton_Refresh->EnableWindow(FALSE);
	pButton_Update->EnableWindow(FALSE);
	pButton_Open->EnableWindow(FALSE);

	RECT rect;
	rect.left = 30;
	rect.top = 80;
	rect.right = 345;
	rect.bottom = 300;

	pListBox->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LBS_HASSTRINGS | LBS_MULTICOLUMN, rect, this, IDC_LIST1);
	pListBox->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	LVCOLUMN pColumn = { 0 };

	pColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	pColumn.fmt = LVCFMT_LEFT;
	pColumn.cx = 100;

	pColumn.pszText = L"Name";
	pListBox->InsertColumn(0, &pColumn);

	pColumn.pszText = L"Size";
	pListBox->InsertColumn(1, &pColumn);

	pColumn.pszText = L"Modified";
	pListBox->InsertColumn(2, &pColumn);

	pListBox->EnableWindow(FALSE);


	return FALSE;  
}



void CMainDlg::OnConnectButtonClick()
{
	if (!ftp)
		ftp = new FTPWorker();

	if (ftp && !ftp->IsConnected()) 
	{
		struct_for_watcher.Current_dir = new wchar_t[256];
		GetCurrentDirectoryW(256,struct_for_watcher.Current_dir);
		struct_for_watcher.dlg = this;
		wchar_t wszFTP[64] = L"node0.net2ftp.ru";                   // node0.net2ftp.ru
		wchar_t *login = L"IL.job@yandex.ru"; // L"anonymous"
		wchar_t *pass = L"af856f9c5ba5";      // L"anonymous"
		//pEdit->GetWindowTextW(wszFTP, 63);
		if (ftp->ConnectServer(wszFTP, login, pass))
		{
			pButton_Refresh->EnableWindow(TRUE);
			pButton_Update->EnableWindow(TRUE);
			pButton_Open->EnableWindow(TRUE);
			pListBox->EnableWindow(TRUE);
			hNotificationThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Directory_watcher_thread, &struct_for_watcher, 0, 0);
			// update list
			OnRefreshButtonClick();
 
		}
		else
		{
			CString err_msg;
			err_msg.AppendFormat(L"Error = %d", ftp->GetErrorCode());
			delete ftp;
			MessageBoxW(err_msg.GetBuffer(), L"Can't connect to a ftp server", MB_ICONWARNING);
		}

	}
}


void CMainDlg::OnRefreshButtonClick()
{
	bool bFirst = true;

	static int index = 0;

	if (ftp && ftp->IsConnected())
	{		
		pListBox->DeleteAllItems();

		while (ftp->EnumerateFiles(bFirst))
		{
			bFirst = false;

			wchar_t *pFile = ftp->GetCurrentFileName();

			if (pFile)
			{
				EnterCriticalSection(&struct_for_watcher.List_lock);

//				memcpy(List_cache[index].File_name,pFile,wcslen(pFile)) ;
//				List_cache[index].Downloaded = false;
				pListBox->InsertItem(index,pFile);

				LeaveCriticalSection(&struct_for_watcher.List_lock);
				index++;
			}

			// update buttons
		}
	}
}


void CMainDlg::OnUpdateButtonClick()
{
	wchar_t wszRemoteFile[128];
	wchar_t wszLocalFile[128];

	if (ftp && ftp->IsConnected())
	{
		int index = pListBox->GetSelectionMark();

		wcscpy(wszRemoteFile,pListBox->GetItemText(index, 0));

		ftp->UpdateFile(wszRemoteFile, wszRemoteFile);
	}
}

void CMainDlg::OnOpenButtonClick()
{
	wchar_t wszRemoteFile[128];
	wchar_t wszLocalFile[128];

	if (ftp && ftp->IsConnected())
	{
		int index = pListBox->GetSelectionMark();

		wcscpy(wszRemoteFile, pListBox->GetItemText(index, 0));
		static DWORD vec_index =0;
		ftp->OpenFile(wszRemoteFile, wszRemoteFile);
		memcpy(List_cache[vec_index].File_name,wszRemoteFile,wcslen(wszRemoteFile)) ;
		List_cache[vec_index].Downloaded = true;
		List_cache[vec_index].index = index;
		vec_index++;
		// shell execute to open file?
	}
}


void CMainDlg::OnExitButtonClick()
{
	if (hNotificationThread)
		TerminateThread(hNotificationThread, 0);
	TerminateProcess((HANDLE)-1,0);
//	if (ftp)
//		delete ftp;

}