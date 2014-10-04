#pragma once

class FTPWorker;
class CStyledListCtrl;

class CMainDlg : public CDialog
{
	CButton *pConnectButton = nullptr;
	CButton *pRefreshButton = nullptr;
	CButton *pUpdateButton = nullptr;
	CButton *pOpenButton = nullptr;
	CButton *pExitButton = nullptr;
	CButton *pDownloadButton = nullptr;

	CEdit *pEditBox = nullptr;
	
	CStyledListCtrl *pListBox = nullptr;

	WATCHER_PARAM watcherParam;
	
	HANDLE hNotificationThread = 0;
	HANDLE hWaitThread = 0;

	FTPWorker *ftp = nullptr;

	DWORD dwError = 0;
	
public:

	enum { IDD = IDD_MAIN_DLG  };

	CMainDlg();
	~CMainDlg();

	BOOL OnInitDialog();
	
	void SetListItemText(DWORD, DWORD, wchar_t*);
	void SetListItemStyle(DWORD, DWORD);
	
	CStyledListCtrl* GetListCtrl();
	FTPWorker* GetFTPConnection();

	afx_msg void OnConnectButtonClick();
	afx_msg void OnRefreshButtonClick();
	afx_msg void OnOpenButtonClick();
	afx_msg void OnDownloadButtonClick();
	afx_msg void OnUpdateButtonClick();
	afx_msg void OnExitButtonClick();
	afx_msg void OnSelectionChanged(NMHDR*, LRESULT*);
	afx_msg void OnDoubleClick(NMHDR*, LRESULT*);

	DECLARE_MESSAGE_MAP()
};