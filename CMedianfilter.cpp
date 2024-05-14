// CMedianfilter.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CMedianfilter.h"


// CMedianfilter 对话框

IMPLEMENT_DYNAMIC(CMedianfilter, CDialogEx)

CMedianfilter::CMedianfilter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMedianfilter, pParent)
	, N1(_T(""))
	, N2(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CMedianfilter::~CMedianfilter()
{
}

void CMedianfilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_N1, N1_Edit);
	DDX_Control(pDX, IDC_N2, N2_Edit);
	DDX_Text(pDX, IDC_N1, N1);
	DDX_Text(pDX, IDC_N2, N2);
}


BEGIN_MESSAGE_MAP(CMedianfilter, CDialogEx)
END_MESSAGE_MAP()


// CMedianfilter 消息处理程序


INT_PTR CMedianfilter::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
