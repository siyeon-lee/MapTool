#pragma once
#include "MapForm.h"

// MapTool

class MapToolPane : public CDockablePane
{
	DECLARE_DYNAMIC(MapToolPane)
public:
	MapForm*	m_wndForm;
public:
	MapToolPane();
	virtual ~MapToolPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};


