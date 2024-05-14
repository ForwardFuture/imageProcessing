//GlobalCommon.cpp
#define _CRT_SECURE_NO_WARNINGS
#include "_GlobalCommon.h"
#include <algorithm>
#include <cstdlib>

/**
	 ����: ��ͼ���ļ��н���DIB��
	 ����: strBmpFile --- ��Ҫ�򿪵�BMP�ļ���
	 ���أ��ļ�������ָ�� (NULL��ʾʧ��)
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
	    pBmpInfo->biSize   != 0x28   ||		// λͼ��Ϣ�ӽṹ����(����40,��0x28)
		pBmpInfo->biPlanes != 0x01 )		// ����������1
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
//   ���ز���

//	 ��������ɫֵ
//	 ����: >=0 ��ʾ������λͼ�����е�ƫ��ֵ
//		    <0 ʧ�ܻ������Ч
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

//  ��������(x,y)����ɫֵ
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
// ����ΪBMP�ļ�

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
// ͼ���ֵ

/**
	 ����: ͼ���ֵ
		   nMethod  ��ֵ�㷨
					0 = ���ٽ���ֵ��
					1 = (˫)���Բ�ֵ��
	 ����: ��ͼ���BMP�ļ��������׵�ַ
           NULL ��ʾʧ�ܣ��ڴ治�㣩
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
			if( nMethod == 0 )		//���ٽ���ֵ��
			{
				int xx = min( (int)(fx+0.5), orgWidth - 1 );
				int yy = min( (int)(fy+0.5), orgHeight - 1 );
				GetPixel(pBmpFileBuf, xx, yy, &rgb);
			}
			else
			{						//(˫)���Բ�ֵ��
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
	 ����: ��˹ƽ��
		   sigma  ��˹�����ľ�����
	 ����: ��ͼ���BMP�ļ��������׵�ַ
		   NULL ��ʾʧ�ܣ��ڴ治�㣩
**/
char* ImageGausssmooth(char* pBmpFileBuf, int sigma)
{
	BITMAPFILEHEADER* pFileHeader = (BITMAPFILEHEADER*)pBmpFileBuf;
	BITMAPINFOHEADER* pDIBInfo = (BITMAPINFOHEADER*)(pBmpFileBuf + sizeof(BITMAPFILEHEADER));

	char* pNewBmpFileBuf = new char[pFileHeader->bfSize];
	memcpy(pNewBmpFileBuf, pBmpFileBuf, pFileHeader->bfOffBits);

	// �����˹��
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
	 ����: ��ֵ�˲�
		   N1  �˲���X����뾶
		   N2  �˲���Y����뾶
	 ����: ��ͼ���BMP�ļ��������׵�ַ
		   NULL ��ʾʧ�ܣ��ڴ治�㣩
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

					// ������ɫ����
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