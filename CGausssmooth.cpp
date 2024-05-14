// CGausssmooth.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CGausssmooth.h"


// CGausssmooth 对话框

IMPLEMENT_DYNAMIC(CGausssmooth, CDialogEx)

CGausssmooth::CGausssmooth(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CGausssmooth, pParent)
	, Gauss(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CGausssmooth::~CGausssmooth()
{
}

void CGausssmooth::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Gauss, Gauss_Edit);
	DDX_Text(pDX, IDC_Gauss, Gauss);
}


BEGIN_MESSAGE_MAP(CGausssmooth, CDialogEx)
END_MESSAGE_MAP()


// CGausssmooth 消息处理程序
