//GlobalCommon.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "_GlobalCommon.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <queue>

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

/**
	 功能: 高斯平滑
		   sigma  高斯函数的均方差
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageGausssmooth(char* pBmpFileBuf, int sigma)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	// 计算高斯核
	int R = (sigma << 1) + 1;
	double* Kernel = new double[R * R];
	for (int i = 0; i < R * R; i++)Kernel[i] = 0.0;
	double tot_value = 0.0;
	for (int y = -sigma; y <= sigma; y++) {
		for (int x = -sigma; x <= sigma; x++) {
			int id = (y + sigma) * R + x + sigma;
			Kernel[id] = exp(-1.0 * (x * x + y * y) / (2.0 * sigma * sigma));
			tot_value += Kernel[id];
		}
	}
	for (int i = 0; i < R * R; i++) {
		Kernel[i] /= tot_value;
	}

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {

			RGBQUAD rgb;
			double r = 0.0, g = 0.0, b = 0.0, res = 0.0;
			int id = 0;
			for (int i = -sigma; i <= sigma; i++) {
				for (int j = -sigma; j <= sigma; j++) {
					int ny = y + i, nx = x + j;
					if (ny < 0)ny = 0;
					if (ny >= Height)ny = Height - 1;
					if (nx < 0)nx = 0;
					if (nx >= Width)nx = Width - 1;
					GetPixel(pBmpFileBuf, nx, ny, &rgb);
					b += Kernel[id] * ((BYTE*)&rgb)[0];
					g += Kernel[id] * ((BYTE*)&rgb)[1];
					r += Kernel[id] * ((BYTE*)&rgb)[2];
					res += Kernel[id] * ((BYTE*)&rgb)[3];
					id++;
				}
			}

			((BYTE*)&rgb)[0] = (int)b;
			((BYTE*)&rgb)[1] = (int)g;
			((BYTE*)&rgb)[2] = (int)r;
			((BYTE*)&rgb)[3] = int(res);

			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: 中值滤波
		   N1  滤波器X方向半径
		   N2  滤波器Y方向半径
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageMedianfilter(char* pBmpFileBuf, int N1, int N2)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int siz = (N1 * 2 + 1) * (N2 * 2 + 1);
	std::pair<double, RGBQUAD>* elem = new std::pair<double, RGBQUAD>[siz];

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {

			RGBQUAD rgb;
			double brightness = 0.0;
			int id = 0;
			for (int i = -N2; i <= N2; i++) {
				for (int j = -N1; j <= N1; j++) {
					int ny = y + i, nx = x + j;
					if (ny < 0)ny = 0;
					if (ny >= Height)ny = Height - 1;
					if (nx < 0)nx = 0;
					if (nx >= Width)nx = Width - 1;
					GetPixel(pBmpFileBuf, nx, ny, &rgb);

					// 计算颜色亮度
					brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
					elem[id] = std::make_pair(brightness, rgb);
					id++;
				}
			}
			
			std::sort(elem, elem + siz, [](std::pair<double, RGBQUAD>& a, std::pair<double, RGBQUAD>& b) {return a.first < b.first; });
			
			SetPixel(pNewBmpFileBuf, x, y, elem[(siz + 1) >> 1].second);

		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: 双边滤波
		   N1       滤波器X方向半径
		   N2       滤波器Y方向半径
		   sigma_d  距离高斯函数均方差
		   sigma_R  颜色高斯函数均方差
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageBilateralfilter(char* pBmpFileBuf, int N1, int N2, int sigma_d, int sigma_R)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int lenX = 2 * N1 + 1;
	int lenY = 2 * N2 + 1;

	// 计算距离高斯函数核
	double* Kernel_d = new double[lenY * lenX];
	for (int i = 0; i < lenY * lenX; i++)Kernel_d[i] = 0.0;
	for (int y = -N2; y <= N2; y++) {
		for (int x = -N1; x <= N1; x++) {
			int id = (y + N2) * lenX + x + N1;
			Kernel_d[id] = exp(-1.0 * (x * x + y * y) / (2.0 * sigma_d * sigma_d));
		}
	}

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {

			RGBQUAD rgb, crgb;
			int id = 0;
			double totvalue = 0.0, weight = 0.0, r = 0.0, g = 0.0, b = 0.0, res = 0.0;
			GetPixel(pBmpFileBuf, x, y, &crgb);

			for (int i = -N2; i <= N2; i++) {
				for (int j = -N1; j <= N1; j++) {
					int ny = y + i, nx = x + j;
					if (ny < 0)ny = 0;
					if (ny >= Height)ny = Height - 1;
					if (nx < 0)nx = 0;
					if (nx >= Width)nx = Width - 1;
					GetPixel(pBmpFileBuf, nx, ny, &rgb);

					// 计算颜色高斯函数核
					int d_r = ((BYTE*)&rgb)[2] - ((BYTE*)&crgb)[2];
					int d_g = ((BYTE*)&rgb)[1] - ((BYTE*)&crgb)[1];
					int d_b = ((BYTE*)&rgb)[0] - ((BYTE*)&crgb)[0];
					weight = exp(-1.0 * (d_r * d_r + d_g * d_g + d_b * d_b) / (2.0 * sigma_R * sigma_R));

					totvalue += weight * Kernel_d[id];
					r += weight * Kernel_d[id] * ((BYTE*)&rgb)[2];
					g += weight * Kernel_d[id] * ((BYTE*)&rgb)[1];
					b += weight * Kernel_d[id] * ((BYTE*)&rgb)[0];
					res += weight * Kernel_d[id] * ((BYTE*)&rgb)[3];

					id++;
				}
			}

			((BYTE*)&rgb)[0] = (int)(b / totvalue);
			((BYTE*)&rgb)[1] = (int)(g / totvalue);
			((BYTE*)&rgb)[2] = (int)(r / totvalue);
			((BYTE*)&rgb)[3] = (int)(res / totvalue);

			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: 直方图均衡化
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageHistoequalization(char* pBmpFileBuf)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	// 统计图像中每个灰度级的像素数量
	int* num = new int[256];
	for (int i = 0; i < 256; i++)num[i] = 0;
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			RGBQUAD rgb;
			GetPixel(pBmpFileBuf, x, y, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			num[(int)brightness]++;
		}
	}

	// 计算每个灰度级像素的频率并做前缀和
	double* perc_num = new double[256];
	int maxn = Width * Height;
	perc_num[0] = 1.0 * num[0] / maxn;
	for (int i = 1; i < 256; i++) {
		perc_num[i] = 1.0 * num[i] / maxn;
		perc_num[i] += perc_num[i - 1];
	}

	// 根据统计出的结果重新计算每个像素的灰度级并更新图像
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			RGBQUAD rgb;
			GetPixel(pBmpFileBuf, x, y, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			int new_brightness = int(perc_num[(int)brightness] * 255.0 + 0.5);

			((BYTE*)&rgb)[3] = min(255, int(1.0 * ((BYTE*)&rgb)[3] / int(brightness) * new_brightness));
			((BYTE*)&rgb)[2] = min(255, int(1.0 * ((BYTE*)&rgb)[2] / int(brightness) * new_brightness));
			((BYTE*)&rgb)[1] = min(255, int(1.0 * ((BYTE*)&rgb)[1] / int(brightness) * new_brightness));
			((BYTE*)&rgb)[0] = min(255, int(1.0 * ((BYTE*)&rgb)[0] / int(brightness) * new_brightness));

			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: 基于梯度的锐化
			k1   基本信息保留因子
			k2   细节增强强度系数
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageSharpengrad(char* pBmpFileBuf, double k1, double k2)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {

			RGBQUAD rgb, crgb;
			// 获得当前像素的颜色值
			GetPixel(pBmpFileBuf, x, y, &crgb);
			double cbrightness = 1.0 * ((((BYTE*)&crgb)[2] * 299) + (((BYTE*)&crgb)[1] * 587) + (((BYTE*)&crgb)[0] * 114)) / 1000.0;

			// 计算X方向的梯度
			int ny = y, nx = x + 1;
			if (nx >= Width)nx = Width - 1;
			GetPixel(pBmpFileBuf, nx, ny, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			double gradX = brightness - cbrightness;

			// 计算Y方向的梯度
			ny = y + 1, nx = x;
			if (ny >= Height)ny = Height - 1;
			GetPixel(pBmpFileBuf, nx, ny, &rgb);
			brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			double gradY = brightness - cbrightness;

			double grad = sqrt(gradX * gradX + gradY * gradY);

			((BYTE*)&rgb)[3] = min(255, int(k1 * ((BYTE*)&crgb)[3] + k2 * grad));
			((BYTE*)&rgb)[2] = min(255, int(k1 * ((BYTE*)&crgb)[2] + k2 * grad));
			((BYTE*)&rgb)[1] = min(255, int(k1 * ((BYTE*)&crgb)[1] + k2 * grad));
			((BYTE*)&rgb)[0] = min(255, int(k1 * ((BYTE*)&crgb)[0] + k2 * grad));

			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: Canny算子边缘检测步骤1
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageCannyedgeStep1(char* pBmpFileBuf)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	int dx[8] = { 1,1,0,-1,-1,-1,0,1 };
	int dy[8] = { 0,-1,-1,-1,0,1,1,1 };
	int sobel_x[3][3] = { {-1,0,1},{-2,0,2},{-1,0,1} };
	int sobel_y[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };
	double pi = acos(-1);
	double* grad = new double[Height * Width];
	int* theta = new int[Height * Width];

	// 利用Sobel算子计算每个点处的梯度模和方向
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {

			RGBQUAD rgb;
			double gradX = 0.0, gradY = 0.0;
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {

					int ny = y + i, nx = x + j;
					if (ny < 0)ny = 0;
					if (ny >= Height)ny = Height - 1;
					if (nx < 0)nx = 0;
					if (nx >= Width)nx = Width - 1;

					GetPixel(pBmpFileBuf, nx, ny, &rgb);
					double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
					
					gradX += brightness * sobel_x[i + 1][j + 1];
					gradY += brightness * sobel_y[i + 1][j + 1];
				}
			}

			int index = y * Width + x;
			grad[index] = sqrt(gradX * gradX + gradY * gradY);
			if (fabs(gradX) < 1e-4) {
				if (gradY > 0)theta[index] = 2;
				else theta[index] = 6;
			}
			else {
				double angle = atan(gradY / gradX);
				if (gradX > 0) {
					if (gradY < 0)angle += 2.0 * pi;
				}
				else if (gradX < 0)angle += pi;

				double min_delta = 30.0;
				int dir = -1;
				double now_angle = 0.0;
				for (int i = 0; i < 7; i++) {
					double tmp = angle - now_angle;
					double now_delta = min(fabs(tmp), min(fabs(tmp + 2 * pi), fabs(tmp - 2 * pi)));
					if (now_delta < min_delta) {
						min_delta = now_delta;
						dir = i;
					}
				}
				theta[index] = dir;
			}
		}
	}

	// 非极大值抑制
	int id = 0;
	RGBQUAD rgb;
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {

			bool check = true;
			int pos = theta[id], neg = (theta[id] + 4) % 8;
			// 检测梯度正方向
			int nx = x + dx[pos], ny = y + dy[pos];
			if (nx < 0)nx = 0;
			if (nx >= Width)nx = Width - 1;
			if (ny < 0)ny = 0;
			if (ny >= Height)ny = Height - 1;
			check &= grad[id] >= grad[ny * Width + nx];
			// 检测梯度负方向
			nx = x + dx[neg], ny = y + dy[neg];
			if (nx < 0)nx = 0;
			if (nx >= Width)nx = Width - 1;
			if (ny < 0)ny = 0;
			if (ny >= Height)ny = Height - 1;
			check &= grad[id] >= grad[ny * Width + nx];

			if (check) {
				((BYTE*)&rgb)[3] = (int)grad[id];
				((BYTE*)&rgb)[2] = (int)grad[id];
				((BYTE*)&rgb)[1] = (int)grad[id];
				((BYTE*)&rgb)[0] = (int)grad[id];
			}
			else {
				((BYTE*)&rgb)[3] = 0;
				((BYTE*)&rgb)[2] = 0;
				((BYTE*)&rgb)[1] = 0;
				((BYTE*)&rgb)[0] = 0;
			}

			SetPixel(pNewBmpFileBuf, x, y, rgb);

			id++;
		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: Canny算子边缘检测步骤2
			maxth   强阈值
			minth   弱阈值
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageCannyedgeStep2(char* pBmpFileBuf, int maxth, int minth)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	// 从所有灰度值高于强阈值的像素出发进行遍历，标记相邻的灰度值不低于弱阈值的像素
	int dx[8] = { 1,1,0,-1,-1,-1,0,1 };
	int dy[8] = { 0,-1,-1,-1,0,1,1,1 };
	bool* vis = new bool[Height * Width];
	for (int i = 0; i < Height * Width; i++)vis[i] = false;
	std::queue<std::pair<int, int> >q;
	RGBQUAD rgb;
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			GetPixel(pBmpFileBuf, x, y, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			if ((int)brightness > maxth) {
				int id = y * Width + x;
				vis[id] = true;
				q.push(std::make_pair(x, y));
			}
		}
	}
	while (!q.empty()) {
		int x = q.front().first, y = q.front().second;
		q.pop();
		for (int i = 0; i < 8; i++) {
			int nx = x + dx[i], ny = y + dy[i];
			if (nx < 0 || nx >= Width || ny < 0 || ny >= Height)continue;
			int index = ny * Width + nx;
			if (vis[index])continue;

			GetPixel(pBmpFileBuf, nx, ny, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			if ((int)brightness > minth) {
				vis[index] = true;
				q.push(std::make_pair(nx, ny));
			}
		}
	}

	int id = 0;
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			if (vis[id]) {
				GetPixel(pBmpFileBuf, x, y, &rgb);
			}
			else {
				((BYTE*)&rgb)[3] = 0;
				((BYTE*)&rgb)[2] = 0;
				((BYTE*)&rgb)[1] = 0;
				((BYTE*)&rgb)[0] = 0;
			}
			SetPixel(pNewBmpFileBuf, x, y, rgb);
			id++;
		}
	}

	return pNewBmpFileBuf;
}

/**
	 功能: Otsu图像分割
			threshold   计算得到的最佳分割阈值
	 返回: 新图像的BMP文件缓冲区首地址
		   NULL 表示失败（内存不足）
**/
char* ImageOtsusegment(char* pBmpFileBuf, int& threshold)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	int Width = pDIBInfo->biWidth;
	int Height = pDIBInfo->biHeight;

	// 计算频率直方图 & 频率累积直方图
	double* histo = new double[256];
	double* acc_histo = new double[256];
	for (int i = 0; i < 256; i++)histo[i] = 0.0;
	for (int i = 0; i < 256; i++)acc_histo[i] = 0.0;
	RGBQUAD rgb;
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			GetPixel(pBmpFileBuf, x, y, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			histo[(int)brightness] += 1.0;
		}
	}
	double fm = 1.0 * Width * Height;
	acc_histo[0] = histo[0] /= fm;
	for (int i = 1; i < 256; i++) {
		histo[i] /= fm;
		acc_histo[i] = histo[i] + acc_histo[i - 1];
	}

	// 计算均值前缀和
	double* mu = new double[256];
	mu[0] = 0.0;
	for (int i = 1; i < 256; i++) {
		mu[i] = 1.0 * i * histo[i];
		mu[i] += mu[i - 1];
	}

	double max_var = 0.0;

	// 枚举可能的阈值
	for (int now_threshold = 1; now_threshold < 256; now_threshold++) {

		// 计算类间方差
		double w1 = acc_histo[now_threshold - 1];
		double w2 = 1 - w1;
		double mu1 = mu[now_threshold - 1] / w1;
		double mu2 = (mu[255] - mu[now_threshold - 1]) / w2;
		double var = w1 * w2 * (mu2 - mu1) * (mu2 - mu1);

		if (var > max_var) {
			max_var = var;
			threshold = now_threshold;
		}
	}

	// 图像分割
	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			GetPixel(pBmpFileBuf, x, y, &rgb);
			double brightness = 1.0 * ((((BYTE*)&rgb)[2] * 299) + (((BYTE*)&rgb)[1] * 587) + (((BYTE*)&rgb)[0] * 114)) / 1000.0;
			if ((int)brightness < threshold) {
				((BYTE*)&rgb)[3] = 0;
				((BYTE*)&rgb)[2] = 0;
				((BYTE*)&rgb)[1] = 0;
				((BYTE*)&rgb)[0] = 0;
			}
			else {
				((BYTE*)&rgb)[3] = 255;
				((BYTE*)&rgb)[2] = 255;
				((BYTE*)&rgb)[1] = 255;
				((BYTE*)&rgb)[0] = 255;
			}
			SetPixel(pNewBmpFileBuf, x, y, rgb);
		}
	}

	return pNewBmpFileBuf;
}