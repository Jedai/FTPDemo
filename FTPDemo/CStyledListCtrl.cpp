#include "stdafx.h"
#include "CStyledListCtrl.h"


BEGIN_MESSAGE_MAP(CStyledListCtrl,CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

CStyledListCtrl::CStyledListCtrl()
{
	txtFont.CreateFont(14, 10, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, L"Arial");
}

CStyledListCtrl::~CStyledListCtrl()
{
	pItems.clear();
}

void CStyledListCtrl::SetItemStyle(DWORD dwItem, DWORD dwStyle)
{
	if (pItems.size() > dwItem)
		pItems[dwItem].dwItemStyle = dwStyle;
}

int CStyledListCtrl::InsertItem(int nItem, LPWSTR pwszItemText)
{
	ITEM_DATA iData = { 0 };

	pItems.push_back(iData); // default style 0

	return CListCtrl::InsertItem(nItem, pwszItemText);
}

void CStyledListCtrl::SetItemData(DWORD index, ITEM_DATA *pData)
{
	if (index < pItems.size())
	{
		pItems[index] = *pData;
	}
}

void CStyledListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch (lplvcd->nmcd.dwDrawStage) 
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		
		if (pItems.size() && (pItems[lplvcd->nmcd.dwItemSpec].dwItemStyle & LIS_BOLD))
		{
			SelectObject(lplvcd->nmcd.hdc, txtFont.GetSafeHandle());
			//lplvcd->clrText = RGB(255, 0, 0);

			*pResult = CDRF_NEWFONT;
		}
		else
			*pResult = CDRF_DODEFAULT;
		break;
	
	/*case CDDS_SUBITEM | CDDS_ITEMPREPAINT:
		break;*/
	}
}


BOOL CStyledListCtrl::DeleteAllItems()
{
	pItems.clear();

	return CListCtrl::DeleteAllItems();
}


