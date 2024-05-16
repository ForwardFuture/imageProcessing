// CHazeremoval.cpp: 实现文件
//

#include "framework.h"
#include "imageProcessing.h"
#include "afxdialogex.h"
#include "CHazeremoval.h"


// CHazeremoval 对话框

IMPLEMENT_DYNAMIC(CHazeremoval, CDialogEx)

CHazeremoval::CHazeremoval(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHazeremoval, pParent)
	, w(_T(""))
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CHazeremoval::~CHazeremoval()
{
}

void CHazeremoval::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_w, w_Edit);
	DDX_Text(pDX, IDC_w, w);
}


BEGIN_MESSAGE_MAP(CHazeremoval, CDialogEx)
END_MESSAGE_MAP()


// CHazeremoval 消息处理程序


INT_PTR CHazeremoval::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::DoModal();
}
