#include "stdafx.h"
#include "CMainDlg.h"
#include "CFTPWorker.h"

BEGIN_MESSAGE_MAP(CMainDlg,CDialog)
	ON_COMMAND(IDC_CONNECT, OnConnectButtonClick)
	ON_COMMAND(IDC_REFRESH, OnRefreshButtonClick)
	ON_COMMAND(IDC_OPEN, OnOpenButtonClick)
	ON_COMMAND(IDC_DOWNLOAD, OnDownloadButtonClick)
	ON_COMMAND(IDC_UPDATE, OnUpdateButtonClick)
	ON_COMMAND(IDC_EXIT, OnExitButtonClick)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnSelectionChanged)
END_MESSAGE_MAP()

CMainDlg::CMainDlg() : CDialog(CMainDlg::IDD)
{	
}

CMainDlg::~CMainDlg()
{
	// terminate notification thread here
	if (hNotificationThread)
		TerminateThread(hNotificationThread, 0);

	if (pListBox)
		delete pListBox;
	if (ftp)
		delete ftp;

	DeleteCriticalSection(&watcherParam.csListLock);
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	pConnectButton = (CButton*)GetDlgItem(IDC_CONNECT);
	
	pRefreshButton = (CButton*)GetDlgItem(IDC_REFRESH);
	pRefreshButton->EnableWindow(FALSE);
	
	pUpdateButton = (CButton*)GetDlgItem(IDC_UPDATE);
	pUpdateButton->EnableWindow(FALSE);
	
	pDownloadButton = (CButton*)GetDlgItem(IDC_DOWNLOAD);
	pDownloadButton->EnableWindow(FALSE);

	pOpenButton = (CButton*)GetDlgItem(IDC_OPEN);
	pOpenButton->EnableWindow(FALSE);

	pExitButton = (CButton*)GetDlgItem(IDC_EXIT);

	pEditBox = (CEdit*)GetDlgItem(IDC_EDIT1);

	pListBox = new CStyledListCtrl();

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

	InitializeCriticalSection(&watcherParam.csListLock);

	return FALSE;  
}



void CMainDlg::OnConnectButtonClick()
{
	if (!ftp)
		ftp = new FTPWorker();

	if (ftp && !ftp->IsConnected()) 
	{
		wchar_t wszFTP[64] = L"node0.net2ftp.ru";                  
		wchar_t login[64] = L"IL.job@yandex.ru"; 
		wchar_t pass[64] = L"af856f9c5ba5";      
		
		WAIT_PARAM waitParam = { 0 };

		waitParam.parent = this;
		wcscpy(waitParam.wszDlgCaption, L"Connecting...");

		hWaitThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitThread, &waitParam, 0, 0);

		if (ftp->ConnectServer(wszFTP, login, pass))
		{
			TerminateThread(hWaitThread, 0);

			// start notification thread here
			watcherParam.dlg = this;
			DWORD dwLen = 255;

			GetCurrentDirectory(dwLen, watcherParam.wszCurrentDir);
			hNotificationThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Directory_watcher_thread, &watcherParam, 0, 0);

			// update list
			OnRefreshButtonClick();

			pRefreshButton->EnableWindow(TRUE);
		}
		else
		{
			delete ftp;
			ftp = nullptr;
		}
	}
}


void CMainDlg::OnRefreshButtonClick()
{
	BOOL bFirst = TRUE;
	DWORD dwIndex = 0;
	
	if (ftp && ftp->IsConnected())
	{		
		WAIT_PARAM waitParam = { 0 };
		waitParam.parent = this;
		wcscpy(waitParam.wszDlgCaption, L"Refreshing directory...");

		hWaitThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitThread, &waitParam, 0, 0);

		pListBox->DeleteAllItems();
		ftp->ClearFileList();

		//EnterCriticalSection(&watcherParam.csListLock);

		while (ftp->EnumerateFiles(bFirst))
		{
			bFirst = FALSE;

			wchar_t *pFile = ftp->GetCurrentFileName();

			if (pFile)
			{
				pListBox->InsertItem(dwIndex,pFile);

				FILE_INFO fInfo = { 0 };
				wcscpy_s(fInfo.wszFileName, COUNTOFWCHAR(fInfo.wszFileName) / 2 - 1, pFile);

				if ((pFile = ftp->GetCurrentFileDate()))
				{
					wcscpy_s(fInfo.wszFileDate, COUNTOFWCHAR(fInfo.wszFileDate) / 2 - 1, pFile);
					pListBox->SetItemText(dwIndex, 2, pFile);
				}
				if ((pFile = ftp->GetCurrentFileSize()))
				{
					wcscpy_s(fInfo.wszFileSize, COUNTOFWCHAR(fInfo.wszFileSize) / 2 - 1, pFile);
					pListBox->SetItemText(dwIndex, 1, pFile);
				}

				ftp->SetFileInfo(dwIndex, &fInfo);
								
				dwIndex++;
			}
		}

		//LeaveCriticalSection(&watcherParam.csListLock);
		
		if (ftp->GetItemCount() > 0)
		{
			pDownloadButton->EnableWindow(TRUE);
			//pUpdateButton->EnableWindow(TRUE);
			//pOpenButton->EnableWindow(TRUE);
		}
		else
			pDownloadButton->EnableWindow(FALSE);

		TerminateThread(hWaitThread, 0);
	}
}


void CMainDlg::OnUpdateButtonClick()
{
	wchar_t wszRemoteFile[128];
	//wchar_t wszLocalFile[128];

	if (ftp && ftp->IsConnected())
	{
		int index = pListBox->GetSelectionMark();

		wcscpy_s(wszRemoteFile, COUNTOFWCHAR(wszRemoteFile) - 1, pListBox->GetItemText(index, 0));

		WAIT_PARAM waitParam = { 0 };
		wcscpy(waitParam.wszDlgCaption, L"Uploading file...");
		hWaitThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitThread, &waitParam, 0, 0);

		if (ftp->UpdateFile(wszRemoteFile, wszRemoteFile))
		{
			pListBox->SetItemStyle(index, ~LIS_BOLD);
		}
		TerminateThread(hWaitThread, 0);
	}
}

void CMainDlg::OnDownloadButtonClick()
{
	wchar_t wszRemoteFile[128];
	//wchar_t wszLocalFile[128];

	if (ftp && ftp->IsConnected())
	{
		int index = pListBox->GetSelectionMark();

		wcscpy_s(wszRemoteFile, COUNTOFWCHAR(wszRemoteFile) - 1, pListBox->GetItemText(index, 0));

		WAIT_PARAM waitParam = { 0 };				
		wcscpy(waitParam.wszDlgCaption, L"Downloading file...");
		hWaitThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitThread, &waitParam, 0, 0);
		
		if (ftp->OpenFile(wszRemoteFile, wszRemoteFile))
		{
			// mark it as "received"
			pListBox->SetItemStyle(index, LIS_BOLD);
			ftp->SetItemReceived(index, TRUE);
		}
		else
		{
			dwError = ftp->GetErrorCode();
		}

		TerminateThread(hWaitThread, 0);
	}
}

void CMainDlg::OnOpenButtonClick()
{
	wchar_t *wszLocalFile = nullptr;

	int index = pListBox->GetSelectionMark();

	if (index > 0)
	{
		wszLocalFile = ftp->GetFileInfoByIndex(index)->wszFileName;

		if (wszLocalFile)
		{
			ShellExecute(NULL, L"open", wszLocalFile, NULL, NULL, SW_SHOW);
		}
	}
}

void CMainDlg::OnExitButtonClick()
{
	EndDialog(0);
}


CStyledListCtrl* CMainDlg::GetListCtrl()
{
	return pListBox;
}

void CMainDlg::SetListItemText(DWORD dwItem, DWORD dwSubItem, wchar_t *pwszText)
{
	pListBox->SetItemText(dwItem, dwSubItem, pwszText);
}

void CMainDlg::SetListItemStyle(DWORD dwItem, DWORD dwStyle)
{
	pListBox->SetItemStyle(dwItem, dwStyle);
}

FTPWorker* CMainDlg::GetFTPConnection()
{
	return ftp;
}

void CMainDlg::OnSelectionChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int item = pNMListView->iItem;

	if ((pNMListView->uChanged & LVIF_STATE) && (pNMListView->uNewState & LVNI_SELECTED))
	{
		if (item > 0)
		{
			if (ftp->IsItemReceived(item))
			{
				pOpenButton->EnableWindow(TRUE);
				pUpdateButton->EnableWindow(TRUE);
			}
			else
			{
				pOpenButton->EnableWindow(FALSE);
				pUpdateButton->EnableWindow(FALSE);
			}
		}
	}
}