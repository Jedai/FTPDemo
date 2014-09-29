#include "stdafx.h"
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
}

CMainDlg::~CMainDlg()
{
	// terminate notification thread here

	if (pListBox)
		delete pListBox;
	if (ftp)
		delete ftp;
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	pConnectButton = (CButton*)GetDlgItem(IDC_CONNECT);
	pRefreshButton = (CButton*)GetDlgItem(IDC_REFRESH);
	if (pRefreshButton)
		pRefreshButton->EnableWindow(false);
	pUpdateButton = (CButton*)GetDlgItem(IDC_UPDATE);
	if (pUpdateButton)
		pUpdateButton->EnableWindow(false);
	pOpenButton = (CButton*)GetDlgItem(IDC_OPEN);
	if (pOpenButton)
		pOpenButton->EnableWindow(false);
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

	return FALSE;  
}


//

void CMainDlg::OnConnectButtonClick()
{
	if (!ftp)
		ftp = new FTPWorker();

	if (ftp && !ftp->IsConnected()) 
	{
		wchar_t wszFTP[64] = L"node0.net2ftp.ru";                  
		wchar_t login[64] = L"IL.job@yandex.ru"; 
		wchar_t pass[64] = L"af856f9c5ba5";      
		
		if (ftp->ConnectServer(wszFTP, login, pass))
		{
			// start notification thread here

			// update list
			OnRefreshButtonClick();

			pRefreshButton->EnableWindow(true);
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
				pListBox->InsertItem(index,pFile);

				ITEM_DATA iData = { 0 };
				wcscpy_s(iData.fInfo.wszFileName, COUNTOFWCHAR(iData.fInfo.wszFileName) / 2 - 1, pFile);

				if ((pFile = ftp->GetCurrentFileDate()))
				{
					wcscpy_s(iData.fInfo.wszFileDate, COUNTOFWCHAR(iData.fInfo.wszFileDate) / 2 - 1, pFile);
					pListBox->SetItemText(index, 2, pFile);
				}
				if ((pFile = ftp->GetCurrentFileSize()))
				{
					wcscpy_s(iData.fInfo.wszFileSize, COUNTOFWCHAR(iData.fInfo.wszFileSize) / 2 - 1, pFile);
					pListBox->SetItemText(index, 1, pFile);
				}

				pListBox->SetItemData(index, &iData);
								
				index++;

				if (!pOpenButton->IsWindowEnabled())
					pOpenButton->EnableWindow(true);
				if (!pUpdateButton->IsWindowEnabled())
					pOpenButton->EnableWindow(true);
			}
		}
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

		ftp->UpdateFile(wszRemoteFile, wszRemoteFile);
	}
}

void CMainDlg::OnOpenButtonClick()
{
	wchar_t wszRemoteFile[128];
	//wchar_t wszLocalFile[128];

	if (ftp && ftp->IsConnected())
	{
		int index = pListBox->GetSelectionMark();

		wcscpy_s(wszRemoteFile, COUNTOFWCHAR(wszRemoteFile) - 1, pListBox->GetItemText(index, 0));

		if (ftp->OpenFile(wszRemoteFile, wszRemoteFile))
		{
			// mark it as "received"
			pListBox->SetItemReceived(index, true);
			pListBox->SetItemStyle(index, LIS_BOLD);

			// shell execute to open file?
			ShellExecute(NULL, L"open", wszRemoteFile, NULL, NULL, SW_SHOW);
		}
	}
}


void CMainDlg::OnExitButtonClick()
{
	EndDialog(0);
}


/*void CMainDlg::SetFileInfo(DWORD index,FILE_INFO *pFileInfo)
{
	if (pFileInfo->wszFileName && pFileList.size() > index)
	{
		pFileList[index].bDownloaded = pFileInfo->bDownloaded;
		wcscpy_s(pFileList[index].wszFileName, COUNTOFWCHAR(FILE_INFO::wszFileName) / 2 - 1, pFileInfo->wszFileName);
	}
}*/


CStyledListCtrl* CMainDlg::GetListCtrl()
{
	return pListBox;
}