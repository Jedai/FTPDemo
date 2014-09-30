#pragma once
#include "stdafx.h"
#include "data.h"

class CStyledListCtrl : public CListCtrl
{
	CFont txtFont;
	DWORD txtStyle;

	std::vector<ITEM_DATA> pItems;

public:

	CStyledListCtrl();
	~CStyledListCtrl();

	int InsertItem(int, LPWSTR);
	BOOL DeleteAllItems();
	void SetItemStyle(DWORD, DWORD);
	void SetItemData(DWORD, PITEM_DATA);
	
	afx_msg void OnCustomDraw(NMHDR*,LRESULT*);

	DECLARE_MESSAGE_MAP()
};
