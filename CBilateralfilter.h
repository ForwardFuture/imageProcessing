#pragma once
#include "afxdialogex.h"


// CBilateralfilter 对话框

class CBilateralfilter : public CDialogEx
{
	DECLARE_DYNAMIC(CBilateralfilter)

public:
	CBilateralfilter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBilateralfilter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CBilateralfilter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit N1_Edit;
	CEdit N2_Edit;
	CEdit sigma_d_Edit;
	CEdit sigma_R_Edit;
	CString N1;
	CString N2;
	CString sigma_d;
	CString sigma_R;
	virtual INT_PTR DoModal();
};
