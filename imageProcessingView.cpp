// imageProcessingView.cpp: CimageProcessingView 类的实现
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "imageProcessing.h"
#include "imageProcessingDoc.h"
#include "imageProcessingView.h"
#include "_GlobalCommon.h"

#include "CInputXY.h"
#include "CInputXYRGB.h"
#include "CInterpolation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CimageProcessingView, CView)

BEGIN_MESSAGE_MAP(CimageProcessingView, CView)
	ON_COMMAND(ID_IMAGEPROCESS_OPENBMPFILE, &CimageProcessingView::OnImageprocessOpenbmpfile)
	ON_COMMAND(ID_IMAGEPROCESS_SAVETOFILE, &CimageProcessingView::OnImageprocessSavetofile)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYFILEHEADER, &CimageProcessingView::OnImageprocessDisplayfileheader)
	ON_COMMAND(ID_IMAGEPROCESS_DISPLAYPALETTE, &CimageProcessingView::OnImageprocessDisplaypalette)
	ON_COMMAND(ID_IMAGEPROCESS_GETPIXELVALUE, &CimageProcessingView::OnImageprocessGetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_SETPIXELVALUE, &CimageProcessingView::OnImageprocessSetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESS_INERPOLATION, &CimageProcessingView::OnImageprocessInterpolation)
	ON_COMMAND(ID_IMAGEPROCESS_GAUSSSMOOTH, &CimageProcessingView::OnImageprocessGausssmooth)
	ON_COMMAND(ID_IMAGEPROCESS_MEDIANFILTER, &CimageProcessingView::OnImageprocessMedianfilter)
	ON_COMMAND(ID_IMAGEPROCESS_BILATERALFILTER, &CimageProcessingView::OnImageprocessBilateralfilter)
	ON_COMMAND(ID_IMAGEPROCESS_HISTOEQUALIZATION, &CimageProcessingView::OnImageprocessHistoequalization)
	ON_COMMAND(ID_IMAGEPROCESS_SHARPENGRAD, &CimageProcessingView::OnImageprocessSharpengrad)
	ON_COMMAND(ID_IMAGEPROCESS_CANNYEDGE, &CimageProcessingView::OnImageprocessCannyedge)
	ON_COMMAND(ID_IMAGEPROCESS_OTSUSEGMENT, &CimageProcessingView::OnImageprocessOtsusegment)
END_MESSAGE_MAP()

CimageProcessingView::CimageProcessingView() noexcept
{
	pFileBuf = NULL;
}

CimageProcessingView::~CimageProcessingView()
{
	if( pFileBuf ) 
	{
		delete [] pFileBuf;
		pFileBuf = 0;
	}
}

BOOL CimageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

#ifdef _DEBUG
void CimageProcessingView::AssertValid() const
{
	CView::AssertValid();
}

void CimageProcessingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CimageProcessingDoc *CimageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimageProcessingDoc)));
	return (CimageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

// CimageProcessingView 绘图
void CimageProcessingView::OnDraw(CDC *pDC)
{
	CimageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if( pFileBuf != NULL )
	{
		DisplayImage(pDC,pFileBuf,10,10,0,0,0);
	}
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//Here are the functions to be programmed by you!

//Open a BMP file
void CimageProcessingView::OnImageprocessOpenbmpfile()
{
	LPCTSTR lpszFilter = "BMP Files (*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE,NULL,NULL,OFN_NOCHANGEDIR,lpszFilter,NULL);
	if( dlg.DoModal() != IDOK ) return;
	if( pFileBuf != NULL )
	{
		delete [] pFileBuf;
	}
	pFileBuf = OpenBMPfile( dlg.GetPathName() );
	Invalidate();
	UpdateWindow();
}

//Save to a new BMP file
void CimageProcessingView::OnImageprocessSavetofile()
{
	if(pFileBuf == NULL) return;
	LPCTSTR lpszFilter = "BMP Files (*.bmp)|*.bmp||";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR, lpszFilter, NULL);
	if (dlg.DoModal() != IDOK) return;
	CString strBmpFile = dlg.GetPathName();
	SaveDIB(pFileBuf, strBmpFile);
}

//Display BMP file header
void CimageProcessingView::OnImageprocessDisplayfileheader()
{
	if(pFileBuf == NULL) return;
	DisplayHeaderMessage(pFileBuf);
}

//Display Pallete
void CimageProcessingView::OnImageprocessDisplaypalette()
{
	if(pFileBuf == NULL) return;
	int num = 0;
	RGBQUAD *palette = GetDIBPaletteData(pFileBuf,&num);
	if( palette == NULL )
	{
		AfxMessageBox("No palette");
	}
	else
	{
		LPCTSTR lpszFilter = "TXT Files (*.txt)|*.txt||";
		CFileDialog dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR, lpszFilter, NULL);
		if (dlg.DoModal() != IDOK) return;
		CString FilePath = dlg.GetPathName();
		CFile hFile;
		if (!hFile.Open(FilePath, CFile::modeCreate | CFile::modeWrite))
		{
			AfxMessageBox("Failed to create the TXT file");
			return;
		}
		char msg_buff[100];
		for (int i = 0; i < num; i++) {
			memset(msg_buff, 0, sizeof(msg_buff));
			sprintf(msg_buff, "Palette[%d]:rgbBlue=%hhx,rgbGreen=%hhx,rgbRed=%hhx,rgbReserved=%hhx\n"
				, i, (palette + i)->rgbBlue, (palette + i)->rgbGreen, (palette + i)->rgbRed, (palette + i)->rgbReserved);
			hFile.Write(msg_buff, (UINT)strlen(msg_buff));
		}
		hFile.Close();
	}
}

//Get pixel value
void CimageProcessingView::OnImageprocessGetpixelvalue()
{
	if (pFileBuf == NULL) return;
	CInputXY inputDlg(NULL);
	if (inputDlg.DoModal() != IDOK)return;	
	int x = atoi(inputDlg.X_Coord);
	int y = atoi(inputDlg.Y_Coord);
	RGBQUAD rgb;
	bool bGray;
	GetPixel(pFileBuf,x,y,&rgb,&bGray);
	char buf[100];
	if( bGray )
		sprintf(buf, "(%d,%d) = %d",x,y,rgb.rgbReserved);
	else
		sprintf(buf, "(%d,%d) = (%d,%d,%d)",x,y,rgb.rgbRed,rgb.rgbGreen,rgb.rgbBlue);
	AfxMessageBox( buf );
}

//Set pixel value
void CimageProcessingView::OnImageprocessSetpixelvalue()
{
	if(pFileBuf == NULL) return;
	CInputXYRGB inputDlg(NULL);
	if (inputDlg.DoModal() != IDOK)return;
	int x = atoi(inputDlg.X_Coord);
	int y = atoi(inputDlg.Y_Coord);
	RGBQUAD rgb;
	rgb.rgbReserved = atoi(inputDlg.Reserved);
	rgb.rgbRed      = atoi(inputDlg.R);
	rgb.rgbGreen    = atoi(inputDlg.G);
	rgb.rgbBlue     = atoi(inputDlg.B);
	SetPixel(pFileBuf,x,y,rgb);
	Invalidate();
	UpdateWindow();
}

//Image interpolaion
void CimageProcessingView::OnImageprocessInterpolation()
{
	if(pFileBuf == NULL) return;
	CInterpolation inputDlg(NULL);
	if (inputDlg.DoModal() != IDOK)return;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pFileBuf + sizeof(BITMAPFILEHEADER));
	int orgWidth = pDIBInfo->biWidth;
	int orgHeight = pDIBInfo->biHeight;
	int newWidth  = atof(inputDlg.factorX) * orgWidth;
	int newHeight = atof(inputDlg.factorY) * orgHeight;
	char* pNewImage = ImageInterpolation(pFileBuf, newWidth, newHeight, atoi(inputDlg.Method));
	delete[] pFileBuf;
	pFileBuf = pNewImage;
	Invalidate();
	UpdateWindow();
}

//Gaussian smoothing
void CimageProcessingView::OnImageprocessGausssmooth()
{
}

//Median filtering
void CimageProcessingView::OnImageprocessMedianfilter()
{
}

//Bilateral filtering
void CimageProcessingView::OnImageprocessBilateralfilter()
{
}

//Histogram equalization
void CimageProcessingView::OnImageprocessHistoequalization()
{
}

//Sharpening by gradient
void CimageProcessingView::OnImageprocessSharpengrad()
{
}

//Canny edge detection
void CimageProcessingView::OnImageprocessCannyedge()
{
}

//Otsu segmentation
void CimageProcessingView::OnImageprocessOtsusegment()
{
}
