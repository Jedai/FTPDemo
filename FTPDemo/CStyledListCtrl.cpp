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

/*int CStyledListCtrl::InsertItem(int nItem, LPWSTR pwszItemText)
{
	//pItems[nItem] = { 0 }; // default style 0

	return CListCtrl::InsertItem(nItem, pwszItemText);
}*/

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
		
		if (pItems[lplvcd->nmcd.dwItemSpec].dwItemStyle & LIS_BOLD /*&& lplvcd->iSubItem == 0*/)
		{
			SelectObject(lplvcd->nmcd.hdc, txtFont.GetSafeHandle());
			//lplvcd->clrText = RGB(255, 0, 0);

			*pResult = CDRF_NEWFONT;
		}
		else
			*pResult = CDRF_NOTIFYITEMDRAW;
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

void CStyledListCtrl::SetItemReceived(DWORD nItem,bool bReceived)
{
	if (nItem < pItems.size())
		pItems[nItem].fInfo.bReceived = bReceived;
}

bool CStyledListCtrl::IsItemReceived(DWORD nItem)
{
	if (nItem < pItems.size())
		return pItems[nItem].fInfo.bReceived;

	return false;
}
