// CInputXYRGB.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CInputXYRGB.h"


// CInputXYRGB 对话框

IMPLEMENT_DYNAMIC(CInputXYRGB, CDialogEx)

CInputXYRGB::CInputXYRGB(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CInputXYRGB, pParent)
	, X_Coord(_T(""))
	, Y_Coord(_T(""))
	, R(_T(""))
	, G(_T(""))
	, B(_T(""))
	, Reserved(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CInputXYRGB::~CInputXYRGB()
{
}

void CInputXYRGB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_X, X);
	DDX_Control(pDX, IDC_Y, Y);
	DDX_Control(pDX, IDC_R, R_Edit);
	DDX_Control(pDX, IDC_G, G_Edit);
	DDX_Control(pDX, IDC_B, B_Edit);
	DDX_Control(pDX, IDC_Reserved, Reserved_Edit);
	DDX_Text(pDX, IDC_X, X_Coord);
	DDX_Text(pDX, IDC_Y, Y_Coord);
	DDX_Text(pDX, IDC_R, R);
	DDX_Text(pDX, IDC_G, G);
	DDX_Text(pDX, IDC_B, B);
	DDX_Text(pDX, IDC_Reserved, Reserved);
}


BEGIN_MESSAGE_MAP(CInputXYRGB, CDialogEx)
END_MESSAGE_MAP()


// CInputXYRGB 消息处理程序


INT_PTR CInputXYRGB::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
