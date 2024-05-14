﻿#pragma once
#include "afxdialogex.h"


// CInputXY 对话框

class CInputXY : public CDialogEx
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
	CEdit X;
	CEdit Y;
	CString X_Coord;
	CString Y_Coord;
	virtual INT_PTR DoModal();
};
