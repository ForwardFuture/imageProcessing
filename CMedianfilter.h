#pragma once
#include "afxdialogex.h"


// CMedianfilter 对话框

class CMedianfilter : public CDialogEx
{
	DECLARE_DYNAMIC(CMedianfilter)

public:
	CMedianfilter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMedianfilter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMedianfilter };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit N1_Edit;
	CEdit N2_Edit;
	CString N1;
	CString N2;
	virtual INT_PTR DoModal();
};
