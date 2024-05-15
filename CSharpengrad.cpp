// CSharpengrad.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CSharpengrad.h"


// CSharpengrad 对话框

IMPLEMENT_DYNAMIC(CSharpengrad, CDialogEx)

CSharpengrad::CSharpengrad(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CSharpengrad, pParent)
	, k1(_T(""))
	, k2(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CSharpengrad::~CSharpengrad()
{
}

void CSharpengrad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_k1, k1_Edit);
	DDX_Control(pDX, IDC_k2, k2_Edit);
	DDX_Text(pDX, IDC_k1, k1);
	DDX_Text(pDX, IDC_k2, k2);
}


BEGIN_MESSAGE_MAP(CSharpengrad, CDialogEx)
END_MESSAGE_MAP()


// CSharpengrad 消息处理程序


INT_PTR CSharpengrad::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
