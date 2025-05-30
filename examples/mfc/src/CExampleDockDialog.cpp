// CExampleDockDialog.cpp: 구현 파일
//

#include "pch.h"
#include "ThreeppMDI.h"
#include "afxdialogex.h"
#include "CExampleDockDialog.h"
#include <LoadExamples.h>
#include <MainFrm.h>


// CExampleDockDialog 대화 상자

IMPLEMENT_DYNAMIC(CExampleDockDialog, CPaneDialog)

CExampleDockDialog::CExampleDockDialog(CWnd* pParent /*=nullptr*/)	
{

}

CExampleDockDialog::~CExampleDockDialog()
{
}

void CExampleDockDialog::DoDataExchange(CDataExchange* pDX)
{
	CPaneDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXAMPLE_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CExampleDockDialog, CPaneDialog)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_EXAMPLE_LIST, &CExampleDockDialog::OnLbnSelchangeExampleList)
END_MESSAGE_MAP()



void CExampleDockDialog::InitDialog()
{
	CLoadExamples m_Examples(800, 600, nullptr);
	for (int i = 0; i < m_Examples.examples.size(); i++) {
		CLoadExamples::Example item = m_Examples.examples[i];
		m_list.AddString(_T(item.exampleName.c_str()));
		exampleList.Add(item.example);
	}
}

void CExampleDockDialog::OnSize(UINT nType, int cx, int cy)
{
	CPaneDialog::OnSize(nType, cx, cy);

	if (m_list.GetSafeHwnd())
	{
		m_list.MoveWindow(0, 0, cx, cy);
	}
}

void CExampleDockDialog::OnLbnSelchangeExampleList()
{
	int sel = m_list.GetCurSel();
	if (sel < 0 || sel >= exampleList.GetSize()) return;
	auto example = exampleList.GetAt(sel);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if (pMainFrame) {
		CString strText;
		m_list.GetText(sel, strText);
		pMainFrame->CreateChildExampleWindow(strText,example);
	}
}
