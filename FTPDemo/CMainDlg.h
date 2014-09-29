#pragma once


#include "stdafx.h"
#include "Directory_watcher.h"

class FTPWorker;

class CMainDlg : public CDialog
{
	
public:
	//std::map<CString,DWORD> List_cache;
	std::vector<ITEM_DATA> List_cache;
	CListCtrl *pListBox;
	CEdit *pEdit_FTP_server;	//; = ; (CEdit*)GetDlgItem(IDC_EDIT1);
	CButton *pButton_Refresh;
	CButton *pButton_Update;
	CButton *pButton_Open;
	CButton *pButton_Exit;
	CButton *pButton_Connect;
	HANDLE hNotificationThread;
	Watcher_str struct_for_watcher;
	FTPWorker *ftp;

public:

	enum { IDD = IDD_MAIN_DLG  };

	CMainDlg();
	~CMainDlg();

	virtual BOOL OnInitDialog();

	afx_msg void OnConnectButtonClick();
	afx_msg void OnRefreshButtonClick();
	afx_msg void OnOpenButtonClick();
	afx_msg void OnUpdateButtonClick();
	afx_msg void OnExitButtonClick();

	DECLARE_MESSAGE_MAP()
};

