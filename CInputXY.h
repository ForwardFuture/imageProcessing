#pragma once
#include "afxdialogex.h"


// CInputXY 对话框

class CInputXY : public CDialog
{
	DECLARE_DYNAMIC(CInputXY)

public:
	CInputXY(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInputXY();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CInputXY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString X;
	CString Y;
	virtual INT_PTR DoModal();
};
