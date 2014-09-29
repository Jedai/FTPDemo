#pragma once

//#include "stdafx.h"

class FTPWorker;
class CStyledListCtrl;

class CMainDlg : public CDialog
{
	CButton *pConnectButton = nullptr;
	CButton *pRefreshButton = nullptr;
	CButton *pUpdateButton = nullptr;
	CButton *pOpenButton = nullptr;
	CButton *pExitButton = nullptr;

	CEdit *pEditBox = nullptr;
	
	CStyledListCtrl *pListBox = nullptr;

	FTPWorker *ftp = nullptr;
	
public:

	enum { IDD = IDD_MAIN_DLG  };

	CMainDlg();
	~CMainDlg();

	virtual BOOL OnInitDialog();
	
	CStyledListCtrl* GetListCtrl();

	afx_msg void OnConnectButtonClick();
	afx_msg void OnRefreshButtonClick();
	afx_msg void OnOpenButtonClick();
	afx_msg void OnUpdateButtonClick();
	afx_msg void OnExitButtonClick();

	DECLARE_MESSAGE_MAP()
};