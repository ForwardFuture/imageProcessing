// imageProcessingDoc.cpp: CimageProcessingDoc 类的实现
//
#include "framework.h"
#include "imageProcessing.h"
#include "imageProcessingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CimageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CimageProcessingDoc, CDocument)
END_MESSAGE_MAP()

CimageProcessingDoc::CimageProcessingDoc() noexcept
{
}

CimageProcessingDoc::~CimageProcessingDoc()
{
}

BOOL CimageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

void CimageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef _DEBUG
void CimageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CimageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
