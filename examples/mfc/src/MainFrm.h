// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h: CMainFrame 
//

#pragma once
#include "CExampleDockDialog.h"
#include "ChildFrm.h"
#include "ThreeContainer.h"
class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
private :
	std::vector<class CChildFrame*> m_exampleChildWindows;
	void CleanupNullWindows();

public:

	CMainFrame() noexcept;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CExampleDockDialog m_wndExampleDialog;

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnExampleButton();
	void CreateChildExampleWindow(const char* title,std::shared_ptr<CThreeContainer> example);
};


