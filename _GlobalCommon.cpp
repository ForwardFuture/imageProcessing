//GlobalCommon.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "_GlobalCommon.h"

/**
	 功能: 从图像文件中建造DIB类
	 参数: strBmpFile --- 需要打开的BMP文件名
	 返回：文件缓冲区指针 (NULL表示失败)
**/
char *OpenBMPfile(CString strBmpFile)
{
	CFile hFile;
	if( !hFile.Open(strBmpFile,CFile::modeRead|CFile::typeBinary) )
	{
		AfxMessageBox("Failed to open the BMP file");
		return( NULL );
	}
	/**/
//	if(	hFile.Seek(0L,CFile::begin) != 0L )
//	{
//		return( NULL );
//	}
	/**/
	long lFileSize = (long)hFile.Seek(0L, CFile::end);
	char *pFileBuf = new char [lFileSize+1];
	hFile.Seek(0L, CFile::begin);
	hFile.Read(pFileBuf, lFileSize);
	hFile.Close();
	/**/
	BITMAPFILEHEADER *pBmpHead = (BITMAPFILEHEADER *)pFileBuf;
	BITMAPINFOHEADER *pBmpInfo = (BITMAPINFOHEADER *)(pFileBuf + sizeof(BITMAPFILEHEADER));
	/**/
	if(	pBmpHead->bfType   != 0x4D42 ||		//"BM"=0x424D
	    pBmpInfo->biSize   != 0x28   ||		// 位图信息子结构长度(等于40,即0x28)
		pBmpInfo->biPlanes != 0x01 )		// 此域必须等于1
	{
		AfxMessageBox("It isn't a valid BMP file");
		return( NULL );
	}
	/**/
	if( pBmpInfo->biCompression != BI_RGB )
	{
		AfxMessageBox("It is a compressed BMP file");
		return( NULL );
	}
	/**/
	if( pBmpInfo->biBitCount != 8  &&
	    pBmpInfo->biBitCount != 24 )
	{
		AfxMessageBox("Only 8-bit and 24-bit BMP files are supported");
		return( NULL );
	}
	/**/
	return( pFileBuf );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	

BITMAPFILEHEADER *GetDIBHEADER(char *pFileBuffer)
{
	char *p = pFileBuffer + 0;
	return( (BITMAPFILEHEADER *)p );
}

BITMAPINFOHEADER *GetDIBINFO(char *pFileBuffer)
{
	char *p = pFileBuffer + sizeof(BITMAPFILEHEADER);
	return( (BITMAPINFOHEADER *)p );
}

char *GetDIBImageData(char *pFileBuffer)
{
	const BITMAPFILEHEADER *pBmpHead = GetDIBHEADER(pFileBuffer);
	char *p = pFileBuffer + pBmpHead->bfOffBits;
	return( p );
}

//return NULL denoting no palette
RGBQUAD *GetDIBPaletteData(char *pFileBuffer,int nEntryNumber[1])
{
	char *pPaletteData = NULL;
	if( GetColorBits(pFileBuffer) <= 8 )
	{
		nEntryNumber[0] = 0;
		char *pDIBImageData = GetDIBImageData(pFileBuffer);
		pPaletteData = pFileBuffer + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		int  nNum = (DWORD)(pDIBImageData - pPaletteData) / sizeof(RGBQUAD);
		int  iUsedColors = (int)GetDIBINFO(pFileBuffer)->biClrUsed;
		if( nNum > 0 && (int)iUsedColors > 0 )
			nEntryNumber[0] = min(nNum,(int)iUsedColors);
		else
			pPaletteData = NULL;
	}
	return( (RGBQUAD *)pPaletteData );
}

int GetImageWidth(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biWidth );
}

int GetImageHeight(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biHeight );
}

int GetColorBits(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( pInfo->biBitCount );
}

long GetUsedColors(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	return( (long)pInfo->biClrUsed );
}

long GetWidthBytes(char *pFileBuffer)
{
	BITMAPINFOHEADER *pInfo = GetDIBINFO(pFileBuffer);
	long nBytesPerRow = 4 * ((pInfo->biWidth * pInfo->biBitCount + 31) / 32);
	return( nBytesPerRow );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	

void DisplayHeaderMessage(char *pBmpFileBuf)
{
	BITMAPFILEHEADER *pBmpHead = GetDIBHEADER(pBmpFileBuf);
	BITMAPINFOHEADER *pBmpInfo = GetDIBINFO(pBmpFileBuf);
	/**/
	char msg[4096];
	sprintf(msg,
	"bfType (file type) = %4.4X \n"
	"bfSize (file length) = %ld \n"
	"bfOffBits (offset of bitmap data in bytes) = %ld \n"
	"biSize (header structure length should be 40 or 0x28) = %ld \n"
	"biWidth (image width)  = %ld \n"
	"biHeight (image height) = %ld \n"
	"biPlanes (must be eaual to 1) = %u \n"
	"biBitCount (color/pixel bits) = %u \n"
	"biCompression (compressed?) = %ld \n"
	"biSizeImage (length of bitmap data in bytes must be the times of 4) = %ld \n"
	"biXPelsPerMeter (horizontal resolution of target device in pixels/metre) = %ld \n"
	"biYPelsPerMeter (vertical resolution of target device in pixels/metre) = %ld \n"
	"biColorUsed (number of colors used in bitmap,0=2**biBitCount) = %ld \n"
	"biColorImportant (number of important colors,0=all colors are important) = %ld \n\n"
	"The following is additional information: \n"
	"Bytes per row in bitmap (nBytesPerRow) = %ld \n"
	"Total bytes of bitmap (nImageSizeInByte) = %ld \n"
	"Actual pixels per row in bitmap (nPixelsPerRow) = %ld \n"
	"Total rows of bitmap (nTotalRows) = %ld \n"
	"Total colors (2**biBitCount)(nTotalColors) = %ld \n"
	"Used colors (biColorUsed)(nUsedColors) = %ld ",
	pBmpHead->bfType,
	pBmpHead->bfSize,
	pBmpHead->bfOffBits,
	pBmpInfo->biSize,
	pBmpInfo->biWidth,
	pBmpInfo->biHeight,
	pBmpInfo->biPlanes,
	pBmpInfo->biBitCount,
	pBmpInfo->biCompression,
	pBmpInfo->biSizeImage,
	pBmpInfo->biXPelsPerMeter,
	pBmpInfo->biYPelsPerMeter,
	pBmpInfo->biClrUsed,
	pBmpInfo->biClrImportant,
	GetWidthBytes(pBmpFileBuf),
	GetWidthBytes(pBmpFileBuf) * GetImageHeight(pBmpFileBuf),
	GetImageWidth(pBmpFileBuf),
	GetImageHeight(pBmpFileBuf),
	1 << GetColorBits(pBmpFileBuf),
	GetUsedColors(pBmpFileBuf) );
	AfxMessageBox(msg);
}

//Mode = 0, normal display
//   1,2,3, display grayscale image in red, green, blue colors
void DisplayImage(CDC *pDC,char *pBmpFileBuf,int disp_xL,int disp_yL,int disp_Width,int disp_Height,int mode)
{
	ASSERT( pDC != NULL );
	HDC hDC = pDC->GetSafeHdc();
	ASSERT( hDC != 0 );
	/**/
	int imageWidth  = GetImageWidth(pBmpFileBuf);
	int imageHeight = GetImageHeight(pBmpFileBuf);
	if( disp_Width <= 0 || disp_Height <= 0 )
	{
		disp_Width  = imageWidth;
		disp_Height = imageHeight;
	}
	CRect rect;
	CWnd *pWnd = pDC->GetWindow();
	pWnd->GetClientRect(&rect);
	disp_Width = min(disp_Width, rect.right - disp_xL);
	disp_Height = min(disp_Height, rect.bottom - disp_yL);
	/**/
	BITMAPINFOHEADER *pBitmapInfo = GetDIBINFO(pBmpFileBuf);
	char *pDIBImageData = GetDIBImageData(pBmpFileBuf);
	/**/
	char buf[40+256*4];
	BITMAPINFO *pBitsInfo = (BITMAPINFO *)buf;
	memcpy(&pBitsInfo->bmiHeader,pBitmapInfo,sizeof(BITMAPINFOHEADER));
	/**/
	int palleteNum = 0;
	RGBQUAD *pallete = GetDIBPaletteData(pBmpFileBuf,&palleteNum);
	for(int c = 0; c < 256; c++)
	{
		if( mode == 0 )
		{
			(pBitsInfo->bmiColors[c]).rgbRed   = (pallete!=NULL && c<palleteNum? pallete[c].rgbRed   : c);
			(pBitsInfo->bmiColors[c]).rgbGreen = (pallete!=NULL && c<palleteNum? pallete[c].rgbGreen : c);
			(pBitsInfo->bmiColors[c]).rgbBlue  = (pallete!=NULL && c<palleteNum? pallete[c].rgbBlue  : c);
		}
		else
		{
			(pBitsInfo->bmiColors[c]).rgbRed   = (mode==1? c : 0); 
			(pBitsInfo->bmiColors[c]).rgbGreen = (mode==2? c : 0); 
			(pBitsInfo->bmiColors[c]).rgbBlue  = (mode==3? c : 0); 
		}
	}
	/**/
	SetStretchBltMode(hDC,COLORONCOLOR);
	StretchDIBits(hDC,disp_xL,disp_yL,disp_Width,disp_Height,
	0,0,imageWidth,imageHeight,pDIBImageData,pBitsInfo,DIB_RGB_COLORS,SRCCOPY );
	/**/
	return;
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
//   象素操作

//	 读象素颜色值
//	 返回: >=0 表示象素在位图数据中的偏移值
//		    <0 失败或参数无效
long GetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb[1],bool bGray[1])
{
	int  nColorBits   = GetColorBits(pFileBuffer);
	int  nImageHeight = GetImageHeight(pFileBuffer);
	int  nBytesPerRow = GetWidthBytes(pFileBuffer);
	/**/
	long nOffInImage  = (nImageHeight-1-y) * nBytesPerRow;
	char *p = GetDIBImageData(pFileBuffer) + nOffInImage;
	/**/
	if( bGray != NULL ) *bGray = true;
	if( nColorBits == 8 )
	{
		nOffInImage += x;
		rgb[0].rgbReserved = p[x];
		rgb[0].rgbRed      = p[x];
		rgb[0].rgbGreen    = p[x];
		rgb[0].rgbBlue     = p[x];
	}
	else if( nColorBits == 24 )
	{
		if( bGray != NULL ) *bGray = false;
		nOffInImage += 3 * x;
		p += (3 * x);
		rgb[0].rgbReserved = 0;
		rgb[0].rgbRed      = p[2];
		rgb[0].rgbGreen    = p[1];
		rgb[0].rgbBlue     = p[0];
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
		return( -1L );
	}
	/**/
	return( nOffInImage );
}

//  设置像素(x,y)的颜色值
void SetPixel(char *pFileBuffer,int x,int y,RGBQUAD rgb)
{
	int  nColorBits   = GetColorBits(pFileBuffer);
	int  nImageHeight = GetImageHeight(pFileBuffer);
	int  nBytesPerRow = GetWidthBytes(pFileBuffer);
	/**/
	long nOffInImage  = (nImageHeight-1-y) * nBytesPerRow;
	char *p = GetDIBImageData(pFileBuffer) + nOffInImage;
	/**/
	if( nColorBits == 8 )
	{
		 p[ x ] = rgb.rgbReserved;
	}
	else if( nColorBits == 24 )
	{
		p += (3 * x);
		p[0] = rgb.rgbBlue;
		p[1] = rgb.rgbGreen;
		p[2] = rgb.rgbRed;
	}
	else
	{
		AfxMessageBox("It is not an 8-bit or 24-bit image");
	}
	/**/
	return;
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
// 保存为BMP文件

BOOL SaveDIB(char *pFileBuffer,CString strBmpFile)
{
	CFile hFile;
	if( !hFile.Open(strBmpFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		AfxMessageBox("Failed to create the BMP file");
		return( FALSE );
	}
	/**/
	BITMAPFILEHEADER *pBmpHead = (BITMAPFILEHEADER *)pFileBuffer;
	long lFileSize = pBmpHead->bfSize;
	hFile.Write(pFileBuffer,lFileSize);
	hFile.Close();
	return( TRUE );
}

///////////////////////////////////////////////////////////////////////	
///////////////////////////////////////////////////////////////////////	
// 图像插值

/**
	 功能: 图像插值
		   nMethod  插值算法
					0 = 最临近插值法
					1 = (双)线性插值法
	 返回: 新图像的BMP文件缓冲区首地址
           NULL 表示失败（内存不足）
**/
char *ImageInterpolation(char *pBmpFileBuf,int newWidth,int newHeight,int nMethod)
{
	BITMAPFILEHEADER *pFileHeader = (BITMAPFILEHEADER *)pBmpFileBuf;
	BITMAPINFOHEADER *pDIBInfo = (BITMAPINFOHEADER *)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));
//	char *pDIBData = pBmpFileBuf + pFileHeader->bfOffBits;
	int  orgWidth  = pDIBInfo->biWidth;
	int  orgHeight = pDIBInfo->biHeight;
	int  colorBits = pDIBInfo->biBitCount;
	/**/
	long bytesPerRow = 4 * ((newWidth * colorBits + 31) / 32);
	long newBmpFileSize = pFileHeader->bfOffBits + bytesPerRow * newHeight;
	char *pNewBmpFileBuf = new char [newBmpFileSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);
	BITMAPFILEHEADER *pNewFileHeader = (BITMAPFILEHEADER *)pNewBmpFileBuf;
	BITMAPINFOHEADER *pNewDIBInfo = (BITMAPINFOHEADER *)(pNewBmpFileBuf + sizeof(BITMAPFILEHEADER));
	pNewFileHeader->bfSize = newBmpFileSize;
	pNewDIBInfo->biWidth = newWidth;
	pNewDIBInfo->biHeight = newHeight;
	pNewDIBInfo->biSizeImage = bytesPerRow * newHeight;
//	char *pNewDIBData = pNewBmpFileBuf + pFileHeader->bfOffBits;
	/**/
	/**/
	float xScale  = (float)orgWidth  / (float)newWidth;
	float yScale  = (float)orgHeight / (float)newHeight;
	for(int y = 0; y < newHeight; y++)
	{
		float fy = y * yScale;
		for(int x = 0; x < newWidth; x++)
		{
			RGBQUAD rgb;
			float fx = x * xScale;
			if( nMethod == 0 )		//最临近插值法
			{
				int xx = min( (int)(fx+0.5), orgWidth - 1 );
				int yy = min( (int)(fy+0.5), orgHeight - 1 );
				GetPixel(pBmpFileBuf, xx, yy, &rgb);
			}
			else
			{						//(双)线性插值法
				RGBQUAD rgbLT,rgbRT,rgbLB,rgbRB;
				int   x1 = (int)fx;
				int   x2 = min(x1+1, orgWidth-1);
				float dx = fx - (float)x1;
				int   y1 = (int)fy;
				int   y2 = min(y1+1, orgHeight-1);
				float dy = fy - (float)y1;
				GetPixel(pBmpFileBuf, x1, y1, &rgbLT);
				GetPixel(pBmpFileBuf, x2, y1, &rgbRT);
				GetPixel(pBmpFileBuf, x1, y2, &rgbLB);
				GetPixel(pBmpFileBuf, x2, y2, &rgbRB);
				for(int N = 0; N < 4; N++)
				{
					float v1 = ((BYTE *)&rgbLT)[N] + dy * (((BYTE *)&rgbLB)[N] - ((BYTE *)&rgbLT)[N]);
					float v2 = ((BYTE *)&rgbRT)[N] + dy * (((BYTE *)&rgbRB)[N] - ((BYTE *)&rgbRT)[N]);
					((BYTE *)&rgb)[N] = (int)(v1 + dx * (v2 - v1) + 0.5);
				}
			}
			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}
	/**/
	return( pNewBmpFileBuf );
}
