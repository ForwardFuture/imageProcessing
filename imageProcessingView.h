// imageProcessingView.h: CimageProcessingView 类的接口
//
#pragma once

class CimageProcessingView : public CView
{
public:
	char *pFileBuf;
	/**/
protected: // 仅从序列化创建
	CimageProcessingView() noexcept;
	DECLARE_DYNCREATE(CimageProcessingView)

// 特性
public:
	CimageProcessingDoc* GetDocument() const;

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CimageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImageprocessOpenbmpfile();
	afx_msg void OnImageprocessSavetofile();
	afx_msg void OnImageprocessDisplayfileheader();
	afx_msg void OnImageprocessDisplaypalette();
	afx_msg void OnImageprocessGetpixelvalue();
	afx_msg void OnImageprocessSetpixelvalue();
	afx_msg void OnImageprocessInerpolation();
	afx_msg void OnImageprocessGausssmooth();
	afx_msg void OnImageprocessMedianfilter();
	afx_msg void OnImageprocessBilateralfilter();
	afx_msg void OnImageprocessHistoequalization();
	afx_msg void OnImageprocessSharpengrad();
	afx_msg void OnImageprocessCannyedge();
	afx_msg void OnImageprocessOtsusegment();
};

#ifndef _DEBUG  // imageProcessingView.cpp 中的调试版本
inline CimageProcessingDoc* CimageProcessingView::GetDocument() const
   { return reinterpret_cast<CimageProcessingDoc*>(m_pDocument); }
#endif

