#pragma once
#include "afxdialogex.h"


// CCannyedge 对话框

class CCannyedge : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyedge)

public:
	CCannyedge(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCannyedge();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCannyedge };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit sigma_Edit;
	CString sigma;
	virtual INT_PTR DoModal();
};
