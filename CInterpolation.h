#pragma once
#include "afxdialogex.h"


// CInterpolation 对话框

class CInterpolation : public CDialogEx
{
	DECLARE_DYNAMIC(CInterpolation)

public:
	CInterpolation(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CInterpolation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CInterpolation };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit Method_Edit;
	CEdit factorX_Edit;
	CEdit factorY_Edit;
	CString Method;
	CString factorX;
	CString factorY;
	virtual INT_PTR DoModal();
};
