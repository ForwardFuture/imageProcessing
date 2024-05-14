// CInputXY.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CInputXY.h"


// CInputXY 对话框

IMPLEMENT_DYNAMIC(CInputXY, CDialogEx)

CInputXY::CInputXY(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CInputXY, pParent)
	, X_Coord(_T(""))
	, Y_Coord(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CInputXY::~CInputXY()
{
}

void CInputXY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_X, X);
	DDX_Control(pDX, IDC_Y, Y);
	DDX_Text(pDX, IDC_X, X_Coord);
	DDX_Text(pDX, IDC_Y, Y_Coord);
}


BEGIN_MESSAGE_MAP(CInputXY, CDialogEx)
END_MESSAGE_MAP()


// CInputXY 消息处理程序


INT_PTR CInputXY::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
