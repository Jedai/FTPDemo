#include "CFTPDemo.h"
#include "CMainDlg.h"

CFTPDemo::CFTPDemo()
{
}


CFTPDemo::~CFTPDemo()
{
}

BOOL CFTPDemo::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CMainDlg dlg;
	
	dlg.DoModal();
	//m_pMainWnd = dlg;
	
	return FALSE;
}

CFTPDemo demoApp;
