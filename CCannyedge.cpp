// CCannyedge.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CCannyedge.h"


// CCannyedge 对话框

IMPLEMENT_DYNAMIC(CCannyedge, CDialogEx)

CCannyedge::CCannyedge(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCannyedge, pParent)
	, sigma(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CCannyedge::~CCannyedge()
{
}

void CCannyedge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_sigma, sigma_Edit);
	DDX_Text(pDX, IDC_sigma, sigma);
}


BEGIN_MESSAGE_MAP(CCannyedge, CDialogEx)
END_MESSAGE_MAP()


// CCannyedge 消息处理程序


INT_PTR CCannyedge::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
