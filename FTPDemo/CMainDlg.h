#pragma once

#include "stdafx.h"

class FTPWorker;

class CMainDlg : public CDialog
{
	CListCtrl *pListBox = nullptr;

	FTPWorker *ftp = nullptr;

	HANDLE hNotificationThread = 0;

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