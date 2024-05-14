#pragma once
#include "afxdialogex.h"


// CGausssmooth 对话框

class CGausssmooth : public CDialogEx
{
	DECLARE_DYNAMIC(CGausssmooth)

public:
	CGausssmooth(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGausssmooth();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGausssmooth };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit Gauss_Edit;
	CString Gauss;
};
