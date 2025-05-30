// 이 MFC 샘플 소스 코드는 MFC Microsoft Office Fluent 사용자 인터페이스("Fluent UI")를 
// 사용하는 방법을 보여 주며, MFC C++ 라이브러리 소프트웨어에 포함된 
// Microsoft Foundation Classes Reference 및 관련 전자 문서에 대해 
// 추가적으로 제공되는 내용입니다.  
// Fluent UI를 복사, 사용 또는 배포하는 데 대한 사용 약관은 별도로 제공됩니다.  
// Fluent UI 라이선싱 프로그램에 대한 자세한 내용은 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ChildView.h: CChildView 클래스의 인터페이스
//


#pragma once
#include "ThreeContainer.h"



class CChildView : public CWnd
{
	DECLARE_DYNCREATE(CChildView)
private:
	CDC* pDC;
	HDC hdc;
	HGLRC hrc;
	int m_nPixelFormat;
	BOOL m_bHandleCreated;
	int m_viewportX;
	int m_viewportY;
	int m_viewportWidth;
	int m_viewportHeight;
	MSG msg;

	void CleanOpenGL();
	int InitOpenGL();

// 구현입니다.
public:
	float m_fAspect;
	bool m_isAnimate = true;
	std::shared_ptr<CThreeContainer> m_example = nullptr;
	CChildView();
	virtual ~CChildView();
	void SetExample(std::shared_ptr<CThreeContainer> example);
	void Clear();
	void Render();
	int GetWidth() const;
	int GetHeight() const;

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

