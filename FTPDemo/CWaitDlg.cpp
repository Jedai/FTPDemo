#include "stdafx.h"
#include "CWaitDlg.h"

CWaitDlg::CWaitDlg(wchar_t *pwszCaption,CDialog *pParent) : CDialog(CWaitDlg::IDD)
{
	pProgressBar = new CProgressCtrl();

	if (pParent)
		pParentDlg = pParent;

	if (pwszCaption)
		wcscpy(wszCaption, pwszCaption);
}

CWaitDlg::~CWaitDlg()
{
}

BOOL CWaitDlg::OnInitDialog()
{
	// create prograss ctrl
	RECT rect = { 15, 15, 280, 50 };

	pProgressBar->Create(WS_CHILD | WS_VISIBLE | PBS_MARQUEE | PBS_SMOOTH, rect, this, IDC_PROGRESS1);
	pProgressBar->SetMarquee(TRUE, 20);

	SetWindowText(wszCaption);

	if (pParentDlg)
		SetParent(pParentDlg);

	return TRUE;
}
