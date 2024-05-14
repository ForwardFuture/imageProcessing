// CInterpolation.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CInterpolation.h"


// CInterpolation 对话框

IMPLEMENT_DYNAMIC(CInterpolation, CDialogEx)

CInterpolation::CInterpolation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CInterpolation, pParent)
	, Method(_T(""))
	, factorX(_T(""))
	, factorY(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CInterpolation::~CInterpolation()
{
}

void CInterpolation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Method, Method_Edit);
	DDX_Control(pDX, IDC_factorX, factorX_Edit);
	DDX_Control(pDX, IDC_factorY, factorY_Edit);
	DDX_Text(pDX, IDC_Method, Method);
	DDX_Text(pDX, IDC_factorX, factorX);
	DDX_Text(pDX, IDC_factorY, factorY);
}


BEGIN_MESSAGE_MAP(CInterpolation, CDialogEx)
END_MESSAGE_MAP()


// CInterpolation 消息处理程序


INT_PTR CInterpolation::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
