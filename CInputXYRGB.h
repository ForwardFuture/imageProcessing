#pragma once
#include "afxdialogex.h"


// CInputXYRGB 对话框

class CInputXYRGB : public CDialogEx
{
	DECLARE_DYNAMIC(CInputXYRGB)

public:
	CInputXYRGB(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInputXYRGB();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CInputXYRGB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit X;
	CEdit Y;
	CEdit R_Edit;
	CEdit G_Edit;
	CEdit B_Edit;
	CEdit Reserved_Edit;
	CString X_Coord;
	CString Y_Coord;
	CString R;
	CString G;
	CString B;
	CString Reserved;
	virtual INT_PTR DoModal();
};
