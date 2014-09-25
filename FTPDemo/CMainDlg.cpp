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
	if (pListBox)
		delete pListBox;
	if (ftp)
		delete ftp;
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	pListBox = new CListCtrl();

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
		wchar_t wszFTP[64] = L"node0.net2ftp.ru";                   // node0.net2ftp.ru
		wchar_t *login = L"IL.job@yandex.ru"; // L"anonymous"
		wchar_t *pass = L"af856f9c5ba5";      // L"anonymous"

		//CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT1);

		//pEdit->GetWindowTextW(wszFTP, 63);

		if (ftp->ConnectServer(wszFTP, login, pass))
		{
			// start notification thread

			hNotificationThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckDirectoryChanges, 0, 0, 0);

			// update list
			OnRefreshButtonClick();
		}
		else
			delete ftp;
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

		ftp->OpenFile(wszRemoteFile, wszRemoteFile);

		// shell execute to open file?
	}
}


void CMainDlg::OnExitButtonClick()
{
	if (hNotificationThread)
		TerminateThread(hNotificationThread, 0);

	if (ftp)
		delete ftp;
}