// CInputXY.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CInputXY.h"
#include "afxwin.h"


// CInputXY 对话框

IMPLEMENT_DYNAMIC(CInputXY, CDialog)

CInputXY::CInputXY(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CInputXY, pParent)
{
	
}

CInputXY::~CInputXY()
{
}

void CInputXY::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Y, Y);
	DDX_Text(pDX, IDC_X, X);
}


BEGIN_MESSAGE_MAP(CInputXY, CDialog)
END_MESSAGE_MAP()


// CInputXY 消息处理程序


INT_PTR CInputXY::DoModal()
{
	return CDialog::DoModal();
}
