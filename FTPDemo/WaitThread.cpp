#include "stdafx.h"
#include "data.h"
#include "CWaitDlg.h"


DWORD WINAPI WaitThread(LPVOID pParam)
{
	PWAIT_PARAM pWaitParam = (PWAIT_PARAM)pParam;
	
	CWaitDlg pDlg(pWaitParam->wszDlgCaption,pWaitParam->parent);

	pDlg.DoModal();
		
	return 0;
}