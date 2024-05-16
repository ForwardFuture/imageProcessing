#pragma once
#include "afxdialogex.h"


// CHazeremoval 对话框

class CHazeremoval : public CDialogEx
{
	DECLARE_DYNAMIC(CHazeremoval)

public:
	CHazeremoval(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHazeremoval();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHazeremoval };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit w_Edit;
	CString w;
	virtual INT_PTR DoModal();
};
