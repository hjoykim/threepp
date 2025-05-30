#pragma once
#include "afxdialogex.h"
#include <ThreeContainer.h>
#include <memory>


// CExampleDockDialog 대화 상자

class CExampleDockDialog : public CPaneDialog
{
	DECLARE_DYNAMIC(CExampleDockDialog)

public:
	CExampleDockDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CExampleDockDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMPLE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_list;
	CArray<std::shared_ptr<CThreeContainer>, std::shared_ptr<CThreeContainer>> exampleList;
	void InitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLbnSelchangeExampleList();
};
