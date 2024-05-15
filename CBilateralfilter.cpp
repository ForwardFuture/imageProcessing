// CBilateralfilter.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CBilateralfilter.h"


// CBilateralfilter 对话框

IMPLEMENT_DYNAMIC(CBilateralfilter, CDialogEx)

CBilateralfilter::CBilateralfilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CBilateralfilter, pParent)
	, N1(_T(""))
	, N2(_T(""))
	, sigma_d(_T(""))
	, sigma_R(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CBilateralfilter::~CBilateralfilter()
{
}

void CBilateralfilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_N1, N1_Edit);
	DDX_Control(pDX, IDC_N2, N2_Edit);
	DDX_Control(pDX, IDC_sigma_d, sigma_d_Edit);
	DDX_Control(pDX, IDC_sigma_R, sigma_R_Edit);
	DDX_Text(pDX, IDC_N1, N1);
	DDX_Text(pDX, IDC_N2, N2);
	DDX_Text(pDX, IDC_sigma_d, sigma_d);
	DDX_Text(pDX, IDC_sigma_R, sigma_R);
}


BEGIN_MESSAGE_MAP(CBilateralfilter, CDialogEx)
END_MESSAGE_MAP()


// CBilateralfilter 消息处理程序


INT_PTR CBilateralfilter::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
