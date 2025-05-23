// CFormLeft.cpp: 구현 파일
//

#include "pch.h"
#include "mfcapp.h"
#include "CFormLeft.h"
#include "CFormRight.h"
#include "LoadExamples.h"
#include "FirstSceneExample.h"
//#include <learningjs/ch1/FirstScene.h>
//#include <learningjs/ch1/MaterialLight.h>
//#include <learningjs/ch1/MaterialLightAnimation.h>
//#include <learningjs/ch2/BasicScene.h>
//#include <learningjs/ch2/FoggyScene.h>
//#include <learningjs/ch2/ForcedMaterials.h>
//#include <learningjs/ch2/CamerasLookAt.h>
//#include <learningjs/ch2/BothCamera.h>
//#include <learningjs/ch2/Geometries.h>
//#include <learningjs/ch2/CustomGeometry.h>
//#include <learningjs/ch2/MeshProperties.h>
//#include <learningjs/ch3/AmbientLightExample.h>
//#include <learningjs/ch3/AreaLightExample.h>
//#include <learningjs/ch3/DirectionalLightExample.h>
//#include <learningjs/ch3/HemisphereLightExample.h>
//#include <learningjs/ch3/LensflaresExample.h>
//#include <learningjs/ch3/PointLightExample.h>
//#include <learningjs/ch3/SpotLightExample.h>
//#include <learningjs/ch4/BasicMeshMaterialExample.h>
//#include <learningjs/ch5/Basic2DGeometriesPlane.h>


// CFormLeft

IMPLEMENT_DYNCREATE(CFormLeft, CFormView)

CFormLeft::CFormLeft()	: CFormView(IDD_FORM_LEFT)
{
	m_initFlag = false;
}

CFormLeft::~CFormLeft()
{
	//if (exampleList.GetCount() > 0) {
	//	for (int i = 0; i < exampleList.GetCount(); i++)
	//		delete exampleList.GetAt(i);
	//}
}

void CFormLeft::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXAMPLE, m_listBox);
}

BEGIN_MESSAGE_MAP(CFormLeft, CFormView)
	ON_LBN_SELCHANGE(IDC_EXAMPLE, &CFormLeft::OnLbnSelchangeExample)
	ON_WM_SIZE()
END_MESSAGE_MAP()



#ifdef _DEBUG
void CFormLeft::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormLeft::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG




void CFormLeft::InitExample()
{
	if (m_target) {
		RECT rect;
		((CFormRight*)m_target)->GetClientRect(&rect);
        CGLControl* glControl = ((CFormRight*) m_target)->m_glControl;
		CLoadExamples examples((int)rect.right, (int)rect.bottom,glControl->GetSafeHwnd());
		for (int i = 0; i < examples.examples.size(); i++) {
			CLoadExamples::Example item = examples.examples[i];
			m_listBox.AddString(_T(item.exampleName.c_str()));
			exampleList.Add(item.example);
		}
	}
}

void CFormLeft::SetTarget(CWnd* m_cwnd)
{
	m_target = m_cwnd;
}

void CFormLeft::OnLbnSelchangeExample()
{
	int index = m_listBox.GetCurSel();
	auto example = exampleList.GetAt(index);
	CGLControl* glControl = ((CFormRight*)m_target)->m_glControl;
	glControl->SetThreeContainer(example.get());
	glControl->Clear();
	//example->renderer->clear();
	glControl->Render();
}


void CFormLeft::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (m_listBox)
		m_listBox.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOOWNERZORDER | SWP_SHOWWINDOW);

}


void CFormLeft::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

}
