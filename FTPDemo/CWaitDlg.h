#pragma once


class CWaitDlg : public CDialog
{
	CProgressCtrl *pProgressBar = nullptr;
	wchar_t wszCaption[32];
	CDialog *pParentDlg = nullptr;

public:

	enum { IDD = IDD_WAIT_DLG };

	CWaitDlg(wchar_t*,CDialog*);
	~CWaitDlg();

	BOOL OnInitDialog();
};
