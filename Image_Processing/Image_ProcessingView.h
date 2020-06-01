
// Image_ProcessingView.h : CImage_ProcessingView 类的接口
//

#pragma once
#include "MyImage_.h"
#include<complex>
using namespace std;

struct myComplex
{
	double real;
	double imag;
};

class CImage_ProcessingView : public CScrollView
{
protected: // 仅从序列化创建
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

// 特性
public:
	CImage_ProcessingDoc* GetDocument() const;	

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImage_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
public:
	void FFT(complex<double>*TD, complex<double>*FD, int index);
	void ImgGray();
	double GausSample(void);
	int Midean(int p[], int k, int l);
	int Minimum(int p[], int k, int l);
	int Maximum(int p[], int k, int l);
	bool FFTtransformFinishFlag; 
	CPoint m_ptStart;
	CPoint m_ptEnd;
	bool drawRGB_RectFinish;
public:
	MyImage_ m_Image;
	MyImage_ p_Image;
	MyImage_ p_Image2;
	MyImage_ g_Image;
	MyImage_ second_Image;
	MyImage_ overlaySecond_Image;

	static bool textShowFlag;
	static bool graBreakflag;

	afx_msg void OnDrawline();
	afx_msg void OnFileSaveAs();
	afx_msg void OnShowred();
//	afx_msg void OnViewToolbar();
	afx_msg void OnImgDegradation();
	afx_msg void OnImgGraBreak();
	afx_msg void OnImgGray();
	afx_msg void OnImgDiff();
	afx_msg void OnImgDenoising();
	afx_msg void OnOriginalHistogram();
	afx_msg void OnMeanfilter();
	afx_msg void OnLaplassefilter();
	afx_msg void OnImgEquilibrium();
	afx_msg void OnMedianfilter();
	afx_msg void OnMaxValuefilter();
	afx_msg void OnMidValuefilter();
	afx_msg void OnMinValuefilter();
	afx_msg void OnInverseHarmonicMeanfilter();
	afx_msg void OnHarmonicMeanfilter();
	afx_msg void OnGeometricMeanfilter();
	afx_msg void OnImgNormalization();
	afx_msg void OnShowgreen();
	afx_msg void OnShowblu();
	afx_msg void OnFftTransform();
	afx_msg void OnRgb2Hsi();
	afx_msg void OnShowHsi();
	afx_msg void OnAdaptivemedianfilter();
	afx_msg void OnAddGaussianNoise();
	afx_msg void OnAddSaltAndPepperNoise();
	afx_msg void OnRgbEqualization();
	afx_msg void OnIEqualization();
	afx_msg void OnGlobalThresholdSeg();
	afx_msg void OnMaxVarianceSeg();
	afx_msg void OnHoughDetectionLine();
	afx_msg void OnIdealLowFilter();
	afx_msg void OnIdealHighFilter();
	afx_msg void OnButterworthLowFilter();
	afx_msg void OnIdButterworthHighFilter();
	afx_msg void OnGaussianLowFilter();
	afx_msg void OnGaussianHighFilter();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRgbImgSegment();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp 中的调试版本
inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
   { return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument); }
#endif

