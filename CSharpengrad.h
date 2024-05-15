#pragma once
#include "afxdialogex.h"


// CSharpengrad 对话框

class CSharpengrad : public CDialogEx
{
	DECLARE_DYNAMIC(CSharpengrad)

public:
	CSharpengrad(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSharpengrad();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSharpengrad };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit k1_Edit;
	CEdit k2_Edit;
	CString k1;
	CString k2;
	virtual INT_PTR DoModal();
};
