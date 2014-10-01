#pragma once

#include "stdafx.h"
#include <map>
#include "Directory_watcher.h"

class FTPWorker;

class CMainDlg : public CDialog
{
	CButton *pConnectButton = nullptr;
	CButton *pRefreshButton = nullptr;
	CButton *pUpdateButton = nullptr;
	CButton *pOpenButton = nullptr;
	CButton *pExitButton = nullptr;

	CEdit *pEditBox = nullptr;
	
	CStyledListCtrl *pListBox = nullptr;

	WATCHER_PARAM watcherParam;
	
	HANDLE hNotificationThread = 0;
	HANDLE hWaitThread = 0;

	FTPWorker *ftp = nullptr;
	
public:
	std::map<CString, DWORD> List_cache;

	enum { IDD = IDD_MAIN_DLG  };

	CMainDlg();
	~CMainDlg();

	BOOL OnInitDialog();
	
	void SetListItemText(DWORD, DWORD, wchar_t*);
	//CStyledListCtrl* GetListCtrl();
	FTPWorker* GetFTPConnection();

	afx_msg void OnConnectButtonClick();
	afx_msg void OnRefreshButtonClick();
	afx_msg void OnOpenButtonClick();
	afx_msg void OnUpdateButtonClick();
	afx_msg void OnExitButtonClick();

	DECLARE_MESSAGE_MAP()
};