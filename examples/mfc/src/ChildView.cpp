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

// ChildView.cpp: CChildView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "ThreeppMDI.h"
#include "ChildView.h"
#include <glad/glad.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CChildView, CWnd)

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CChildView::CleanOpenGL()
{
	if (hrc) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		hrc = NULL;
	}
}

int CChildView::InitOpenGL()
{
	CDC* pDC = GetDC();
	hdc = pDC->GetSafeHdc();
	PIXELFORMATDESCRIPTOR pfd;

	// Set Window Pixel Format
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Size of this pfd
	pfd.nVersion = 1;							// Version number : must be 1
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |		// Support window
		PFD_SUPPORT_OPENGL |		// Support OpenGL
		PFD_DOUBLEBUFFER |		// Double buffered
		PFD_GENERIC_FORMAT;

	pfd.iPixelType = PFD_TYPE_RGBA;	// RGBA type
	pfd.cColorBits = 32;				// Specifies the number of color biCXD3Planes in each color buffer
	pfd.cDepthBits = 24;				// Specifies the depth of the depth (z-axis) buffer
	pfd.cStencilBits = 8;				// Specifies the depth of thef stencil buffer
	pfd.iLayerType = PFD_MAIN_PLANE;	// Ignored. Earlier implementations of OpenG used this member, but it is no longer used
	

	// Attempts to match an appropriate pixel format supported by a device context to a given pixel format specification
	int m_GLPixelIndex = ChoosePixelFormat(hdc, &pfd);
	// Choose default
	if (m_GLPixelIndex == 0)
	{
		// Obtains information about the pixel format identified by iPixelFormat of the device associated with hdc
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hdc, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pfd) == 0)	return FALSE;
	}
	//Sets the pixel format of the specified device context to the format specified by the iPixelFormat index
	if (!SetPixelFormat(hdc, m_GLPixelIndex, &pfd))	return FALSE;
	//Create Rendering Context
	hrc = ::wglCreateContext(pDC->GetSafeHdc());
	if (hrc == 0)	return FALSE;	//Failure to Create Rendering Context

	// Make the RC Current
	if (wglMakeCurrent(pDC->GetSafeHdc(), hrc) == FALSE)	return FALSE;

	int success = gladLoadGL();

	
	return 0;
}

CChildView::CChildView()
{
	ZeroMemory(&msg, sizeof(msg));
}

CChildView::~CChildView()
{
}

void CChildView::SetExample(std::shared_ptr<CThreeContainer> example)
{
	CRect rect;
	GetClientRect(&rect);
	m_example = example;
	m_example->setHwnd((int)rect.right, (int)rect.bottom, GetSafeHwnd());
	m_example->sizeChanged((int)rect.right, (int)rect.bottom);
}

void CChildView::Clear()
{
	GLfloat clear_buffer[] = { 0.0f,0.0f,0.0f,1.0f };
	glClearBufferfv(GL_COLOR, 0, clear_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CChildView::Render()
{
	RECT rect;
	GetClientRect(&rect);

	if (!wglMakeCurrent(hdc, hrc)) {
		AfxMessageBox(_T("wglMakeCurrent failed in Render()"));
		return;
	}

	if (m_example) {
		if (!m_example->Initialized) {
			Color c;
			m_example->renderer->getClearColor(c);
			//glClear(GL_COLOR_BUFFER_BIT);
			Clear();
			glClearColor(c.r, c.g, c.b, 1.0f);
			m_example->renderer->clear(true, true, true);
			m_example->init();
			m_example->Initialized = true;
			m_example->renderer->setViewport(0, 0, rect.right, rect.bottom);
		}
		m_example->render();
	}
	SwapBuffers(hdc);
}

int CChildView::GetWidth() const
{
	CRect rect;
	GetClientRect(&rect);
	return rect.Width();
}

int CChildView::GetHeight() const
{
	CRect rect;
	GetClientRect(&rect);
	return rect.Height();
}

// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return CWnd::PreCreateWindow(cs);
}

void CChildView::OnPaint() 
{
	//CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
	ValidateRect(NULL);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetTimer(0, 1, NULL);
	return InitOpenGL();
}

void CChildView::OnDestroy()
{
	CleanOpenGL();
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}
void CChildView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	wglMakeCurrent(hdc, hrc);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnEraseBkgnd(pDC);
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_example && m_example->controls) {
		m_example->onMousePressedEvent(0, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::PRESS);
		if (!m_isAnimate) Render();

	}
	CWnd::OnLButtonDown(nFlags, point);

}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_example && m_example->controls) {
		m_example->onMousePressedEvent(0, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::RELEASE);
		if (!m_isAnimate) Render();

	}
	CWnd::OnLButtonUp(nFlags, point);
}
void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_example && m_example->controls) {
		m_example->onMouseMoveEvent(Vector2((float)point.x, (float)point.y));
		if (!m_isAnimate) Render();
	}
	CWnd::OnMouseMove(nFlags, point);
}
BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (m_example && m_example->controls) {
		m_example->onMouseWheelEvent(Vector2(zDelta, zDelta));
		if (!m_isAnimate) Render();

	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_example && m_example->controls) {
		m_example->onMousePressedEvent(1, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::PRESS);
		if (!m_isAnimate) Render();

	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_example && m_example->controls) {
		m_example->onMousePressedEvent(1, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::RELEASE);
		if (!m_isAnimate) Render();

	}
	CWnd::OnRButtonUp(nFlags, point);
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;
	m_fAspect = (float)cy / (float)cx;
	if (GetDC()->GetSafeHdc()) {
		if (m_example) {
			m_example->sizeChanged(cx, cy);
			Render();
		}
	}
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_isAnimate) Render();
	CWnd::OnTimer(nIDEvent);
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CChildView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (ImGui_ImplWin32_WndProcHandler(GetSafeHwnd(), message, wParam, lParam))
		return true;

	return CWnd::WindowProc(message, wParam, lParam);
}



