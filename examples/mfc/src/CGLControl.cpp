#include "pch.h"
#include "CGLControl.h"
//#include <imgui/imgui.h>
//#include <imgui/imgui_impl_win32.h>
//#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#ifdef GLES
#include "GLESHeader.h"
#endif
IMPLEMENT_DYNCREATE(CGLControl,CWnd)


BEGIN_MESSAGE_MAP(CGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void CGLControl::SetViewport(int x, int y, int width, int height)
{
	m_viewportX = x;
	m_viewportY = y;
	m_viewportWidth = width;
	m_viewportHeight = height;
	glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
}

CGLControl::CGLControl()
{
	ZeroMemory(&msg, sizeof(msg));
}

CGLControl::~CGLControl()
{
	//ImGui_ImplOpenGL3_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();
	if (hrc) 
	{
		wglDeleteContext(hrc);
		hrc = NULL;
	}
}

int CGLControl::GetWidth() const
{
	CRect rect;
	GetClientRect(&rect);
	return rect.Width();
}

int CGLControl::GetHeight() const
{
	CRect rect;
	GetClientRect(&rect);
	return rect.Height();
}



void CGLControl::SetThreeContainer(CThreeContainer* examplePtr)
{
	demo = examplePtr;
	if (demo) {
		demo->sizeChanged(GetWidth(), GetHeight());
	}
}

void CGLControl::Clear()
{
	GLfloat clear_buffer[] = { 0.0f,0.0f,0.0f,1.0f };
	glClearBufferfv(GL_COLOR, 0, clear_buffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CGLControl::Render()
{
	RECT rect;
	GetClientRect(&rect);

	if (demo) {
		if (!demo->initialized) {
			Color c;
			//demo->renderer.getClearColor(c);
			//glClear(GL_COLOR_BUFFER_BIT);
			Clear();
			glClearColor(c.r, c.g, c.b, 1.0f);
			demo->renderer.clear(true, true, true);
			demo->init();
			demo->initialized = true;			
			demo->renderer.setViewport(0, 0, rect.right, rect.bottom);
		}
		demo->render();
	}
	SwapBuffers(hdc);
	///*if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//	return;
	//}*/

	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();

	//if (demo)
	//	demo->showControls();

	//ImGui::Render();
	//glViewport(0, 0, rect.right, rect.bottom);
	//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	

}

BOOL CGLControl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = ::AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CWnd::PreCreateWindow(cs);
}

void CGLControl::OnPaint()
{
	//CPaintDC dc(this); 

	ValidateRect(NULL);
}


void CGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;
	m_fAspect = (float)cy / (float)cx;
	if (GetDC()->GetSafeHdc()) {
		if (demo) {
			demo->sizeChanged(cx, cy);
            demo->render();
		}
	}
}

int CGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

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
	pfd.cColorBits = 24;				// Specifies the number of color biCXD3Planes in each color buffer
	pfd.cRedBits = 0;				// Specifies the number of red biCXD3Planes in each RGBA color buffer
	pfd.cRedShift = 0;				// Specifies the shift count for red biCXD3Planes in each RGBA color buffer
	pfd.cGreenBits = 0;				// Specifies the number of green biCXD3Planes in each RGBA color buffer
	pfd.cGreenShift = 0;				// Specifies the shift count for green biCXD3Planes in each RGBA color buffer
	pfd.cBlueBits = 0;				// Specifies the number of blue biCXD3Planes in each RGBA color buffer
	pfd.cBlueShift = 0;				// Specifies the shift count for blue biCXD3Planes in each RGBA color buffer
	pfd.cAlphaBits = 0;				// Specifies the number of alpha biCXD3Planes in each RGBA color buffer. Alpha biCXD3Planes are not supported
	pfd.cAlphaShift = 0;				// Specifies the shift count for alpha biCXD3Planes in each RGBA color buffer. Alpha biCXD3Planes are not supported
	pfd.cDepthBits = 24;				// Specifies the depth of the depth (z-axis) buffer
	pfd.cStencilBits = 8;				// Specifies the depth of thef stencil buffer
	pfd.cAuxBuffers = 0;				// Specifies the number of auxiliary buffers. Auxiliary buffers are not supported
	pfd.iLayerType = PFD_MAIN_PLANE;	// Ignored. Earlier implementations of OpenG used this member, but it is no longer used
	pfd.bReserved = 0;				// Specifies the number of overlay and underlay planes
	pfd.dwLayerMask = 0;				// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used
	pfd.dwVisibleMask = 0;				// Specifies the transparent color or index of an underlay plane
	pfd.dwDamageMask = 0;				// Ignored. Earlier implementations of OpenGL used this member, but it is no longer used

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

	// if(m_hMasterGLContext) // HGLRC m_hMasterGLContext;
	// hglrc1 : Specifies the OpenGL rendering context with which to share display lists.
	// hglrc2 : Specifies the OpenGL rendering context to share display lists with hglrc1.
	//			The hglrc2 parameter should not contain any existing display lists when wglShareLists is called.
	// wglShareLists(m_hMasterGLContext, _hGLContext);}

	// Make the RC Current
	if (::wglMakeCurrent(pDC->GetSafeHdc(), hrc) == FALSE)	return FALSE;
#ifdef GLEW
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		AfxMessageBox(_T("Graphic Driver of your PC is not Supported, Please Upgrade your graphic card!"));
		return FALSE;
	}
#endif
	int success = gladLoadGL();

	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//imGuiIO = &io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	//ImGui::StyleColorsDark();

	//ImGui_ImplWin32_Init(GetSafeHwnd());
	//ImGui_ImplOpenGL3_Init(0);

	SetTimer(0, 1, NULL);
	return 0;

}

void CGLControl::OnDestroy()
{
	CWnd::OnDestroy();

}


BOOL CGLControl::OnEraseBkgnd(CDC* pDC)
{

	return CWnd::OnEraseBkgnd(pDC);
}


void CGLControl::OnMouseMove(UINT nFlags, CPoint point)
{

	CWnd::OnMouseMove(nFlags, point);
	if (demo && demo->controls) {
		demo->onMouseMoveEvent(Vector2((float)point.x, (float)point.y));
		if (!m_isAnimate) Render();
	}
}


void CGLControl::OnLButtonUp(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonUp(nFlags, point);
	if (demo && demo->controls) {
		demo->onMousePressedEvent(0, Vector2((float)point.x, (float)point.y),PeripheralsEventSource::MouseAction::RELEASE);
		if (!m_isAnimate) Render();
	
	}
}


void CGLControl::OnRButtonUp(UINT nFlags, CPoint point)
{

	CWnd::OnRButtonUp(nFlags, point);
	if (demo && demo->controls) {
		demo->onMousePressedEvent(1, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::RELEASE);
		if (!m_isAnimate) Render();

	}
}


BOOL CGLControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	
	if (demo && demo->controls) {
		demo->onMouseWheelEvent(Vector2(zDelta,zDelta));
		if (!m_isAnimate) Render();

	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLControl::OnTimer(UINT_PTR nIDEvent)
{

	if (m_isAnimate) Render();
	CWnd::OnTimer(nIDEvent);
}

void CGLControl::OnLButtonDown(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonDown(nFlags, point);
	if (demo && demo->controls) {
		demo->onMousePressedEvent(0, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::PRESS);
		if (!m_isAnimate) Render();

	}
}


void CGLControl::OnRButtonDown(UINT nFlags, CPoint point)
{

	CWnd::OnRButtonDown(nFlags, point);
	if (demo && demo->controls) {
		demo->onMousePressedEvent(1, Vector2((float)point.x, (float)point.y), PeripheralsEventSource::MouseAction::PRESS);
		if (!m_isAnimate) Render();

	}
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CGLControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(GetSafeHwnd(), message, wParam, lParam))
		return true;

	switch (message) {
	case WM_DESTROY :
		::PostQuitMessage(0);
		break;
	}
	return CWnd::WindowProc(message, wParam, lParam);
}


