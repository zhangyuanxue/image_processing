
// Image_ProcessingView.cpp : CImage_ProcessingView 类的实现
//

#include "stdafx.h"

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Image_Processing.h"
#endif

#include "Image_ProcessingDoc.h"
#include "Image_ProcessingView.h"

#include"GrayScale.h"
#include"Dgradation.h"
#include"ImgOverlay.h"
#include"HistogramImg.h"
#include <math.h>
#include "afxdialogex.h"
#include "TemplateSize.h"
#include "UniversalDlg.h"
#include<algorithm>
#include<vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#define MN 9//定义模板大小（MN必须为奇数）
#define Q -1.5
#define maxthreshold 200
#define PI 3.14
#define pi 3.14


struct Point
{
	int x;
	int y;
};
struct LinePolar
{
	float rho;
	float angle;
};

struct hough_cmp_gt
{
	hough_cmp_gt(const int* _aux) : aux(_aux) {}
	bool operator()(int l1, int l2) const
	{
		return aux[l1] > aux[l2] || (aux[l1] == aux[l2] && l1 < l2);
	}
	const int* aux;
};

// CImage_ProcessingView

IMPLEMENT_DYNCREATE(CImage_ProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImage_ProcessingView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImage_ProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CImage_ProcessingView::OnFileOpen)
	ON_COMMAND(IDM_DRAWLINE, &CImage_ProcessingView::OnDrawline)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImage_ProcessingView::OnFileSaveAs)
	ON_COMMAND(IDM_SHOWRED, &CImage_ProcessingView::OnShowred)
//	ON_COMMAND(ID_VIEW_TOOLBAR, &CImage_ProcessingView::OnViewToolbar)
	ON_COMMAND(ID_IMG_DEGRADATION, &CImage_ProcessingView::OnImgDegradation)
	ON_COMMAND(ID_IMG_GRA_BREAK, &CImage_ProcessingView::OnImgGraBreak)
	ON_COMMAND(ID_IMG_GRAY, &CImage_ProcessingView::OnImgGray)
	ON_COMMAND(ID_IMG_DIFF, &CImage_ProcessingView::OnImgDiff)
	ON_COMMAND(ID_IMG_DENOISING, &CImage_ProcessingView::OnImgDenoising)
	ON_COMMAND(ID_ORIGINAL_HISTOGRAM, &CImage_ProcessingView::OnOriginalHistogram)
	ON_COMMAND(ID_MeanFilter, &CImage_ProcessingView::OnMeanfilter)
	ON_COMMAND(ID_LaplasseFilter, &CImage_ProcessingView::OnLaplassefilter)
	ON_COMMAND(ID_IMG_Equilibrium, &CImage_ProcessingView::OnImgEquilibrium)
	ON_COMMAND(ID_MedianFilter, &CImage_ProcessingView::OnMedianfilter)
	ON_COMMAND(ID_Max_ValueFilter, &CImage_ProcessingView::OnMaxValuefilter)
	ON_COMMAND(ID_Mid_ValueFilter, &CImage_ProcessingView::OnMidValuefilter)
	ON_COMMAND(ID_Min_ValueFilter, &CImage_ProcessingView::OnMinValuefilter)
	ON_COMMAND(ID_Inverse_Harmonic_MeanFilter, &CImage_ProcessingView::OnInverseHarmonicMeanfilter)
	ON_COMMAND(ID_Harmonic_Meanfilter, &CImage_ProcessingView::OnHarmonicMeanfilter)
	ON_COMMAND(ID_Geometric_MeanFilter, &CImage_ProcessingView::OnGeometricMeanfilter)
	ON_COMMAND(ID_IMG_Normalization, &CImage_ProcessingView::OnImgNormalization)
	ON_COMMAND(ID_SHOWGREEN, &CImage_ProcessingView::OnShowgreen)
	ON_COMMAND(ID_SHOWBLU, &CImage_ProcessingView::OnShowblu)
	ON_COMMAND(ID_FFT_Transform, &CImage_ProcessingView::OnFftTransform)
	ON_COMMAND(ID_RGB_2_HSI, &CImage_ProcessingView::OnRgb2Hsi)
	ON_COMMAND(ID_SHOW_HSI, &CImage_ProcessingView::OnShowHsi)
	ON_COMMAND(ID_Adaptive_median_filter, &CImage_ProcessingView::OnAdaptivemedianfilter)
	ON_COMMAND(ID_Add_Gaussian_Noise, &CImage_ProcessingView::OnAddGaussianNoise)
	ON_COMMAND(ID_Add_Salt_And_Pepper_Noise, &CImage_ProcessingView::OnAddSaltAndPepperNoise)
	ON_COMMAND(ID_RGB_Equalization, &CImage_ProcessingView::OnRgbEqualization)
	ON_COMMAND(ID_I_Equalization, &CImage_ProcessingView::OnIEqualization)
	ON_COMMAND(ID_Global_Threshold_Seg, &CImage_ProcessingView::OnGlobalThresholdSeg)
	ON_COMMAND(ID_Max_Variance_SEG, &CImage_ProcessingView::OnMaxVarianceSeg)
	ON_COMMAND(ID_Hough_Detection_Line, &CImage_ProcessingView::OnHoughDetectionLine)
	ON_COMMAND(ID_Ideal_Low_Filter, &CImage_ProcessingView::OnIdealLowFilter)
	ON_COMMAND(ID_Ideal_High_Filter, &CImage_ProcessingView::OnIdealHighFilter)
	ON_COMMAND(ID_Butterworth_Low_Filter, &CImage_ProcessingView::OnButterworthLowFilter)
	ON_COMMAND(ID_ID_Butterworth_HIGH_Filter, &CImage_ProcessingView::OnIdButterworthHighFilter)
	ON_COMMAND(ID_Gaussian_Low_Filter, &CImage_ProcessingView::OnGaussianLowFilter)
	ON_COMMAND(ID_Gaussian_HIGH_Filter, &CImage_ProcessingView::OnGaussianHighFilter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_RGB_IMG_SEGMENT, &CImage_ProcessingView::OnRgbImgSegment)
END_MESSAGE_MAP()

// CImage_ProcessingView 构造/析构
//////////////////////////////////////////////
bool CImage_ProcessingView::textShowFlag=false;
bool CImage_ProcessingView::graBreakflag=false;


////////////////////////////////////////////////
CImage_ProcessingView::CImage_ProcessingView()
{
	// TODO: 在此处添加构造代码
	FFTtransformFinishFlag=false;
	drawRGB_RectFinish = false;
	m_ptStart=(0,0);
    m_ptEnd=(0,0);
}

CImage_ProcessingView::~CImage_ProcessingView()
{
	

}

BOOL CImage_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CImage_ProcessingView 绘制

void CImage_ProcessingView::OnDraw(CDC* pDC)
{
	CImage_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	int w;
	if (!m_Image.IsNull())
	{
		CString  str = _T("处理前图像");
		CString  str1 = _T("处理后图像");
		if(CImage_ProcessingView::textShowFlag)
		{
			pDC->DrawText(str1, CRect(200, 3, 300, 20), DT_CENTER);
		}
		else pDC->DrawText(str, CRect(200, 3, 300, 20), DT_CENTER);
		m_Image.Draw(pDC->m_hDC,0,40);
		w=m_Image.GetWidth();
	}
	if (!p_Image.IsNull())
	{
		CString str= _T("降质后图像");
		pDC->DrawText(str, CRect(w+150, 3, w+250, 20), DT_CENTER);
		p_Image.Draw(pDC->m_hDC,w+30,40);
	}
	if (!p_Image2.IsNull())
	{
		CString str= _T("灰度图降质后图像");
		pDC->DrawText(str, CRect(w+150, 3, w+300, 20), DT_CENTER);
		p_Image2.Draw(pDC->m_hDC,w+30,40);
	}
	if (!g_Image.IsNull())
	{
		CString str= _T("处理后图像");
		pDC->DrawText(str, CRect(w+150, 3, w+250, 20), DT_CENTER);
		g_Image.Draw(pDC->m_hDC,w+30,40);
	}
	if (!second_Image.IsNull())
	{
		CString str= _T("处理后图像");
		pDC->DrawText(str, CRect(w+150, 3, w+250, 20), DT_CENTER);
		second_Image.Draw(pDC->m_hDC,w+30,40);
	}
	return;
}

void CImage_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImage_ProcessingView 打印


void CImage_ProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImage_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImage_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImage_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImage_ProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImage_ProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImage_ProcessingView 诊断

#ifdef _DEBUG
void CImage_ProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImage_ProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
	return (CImage_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImage_ProcessingView 消息处理程序


void CImage_ProcessingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if(IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{

		if(!m_Image.IsNull()) m_Image.Destroy();//判断是否已经有图片，有的话进行清除
		m_Image.Load(dlg.GetPathName());//获得图片的地址，并且加载图片//调用load函数后便可通过m_Image对象访问图片信息

		//获得图片的大小，并按其大小设置滚动条的初始窗口大小等参数
		/*CSize sizeTotal;
		int w=m_Image.GetWidth();
		int h=m_Image.GetHeight();
		sizeTotal.cx =w;
		sizeTotal.cy =h;
		SetScrollSizes(MM_TEXT, sizeTotal);*/

		//Window_Image_w=m_Image.GetWidth();//获得图片的初始大小，为放大缩小功能做准备
		//Window_Image_h=m_Image.GetHeight();//
		graBreakflag=false;
		p_Image.m_CImage.Destroy();//使可重复加载
		p_Image2.m_CImage.Destroy();//使可重复加载
		g_Image.m_CImage.Destroy();//使可重复加载
		second_Image.m_CImage.Destroy();//使可重复加载
		overlaySecond_Image.m_CImage.Destroy();//使可重复加载

		Invalidate(1); //强制调用ONDRAW函数
	}
}


void CImage_ProcessingView::OnDrawline()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;
	
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();

	for (int k=0;k<w;k++)
	{
		m_Image.m_pBits[0][10][k]=0;//B
		m_Image.m_pBits[1][10][k]=0;//G
		m_Image.m_pBits[2][10][k]=0;//R
	}

	Invalidate(1); //强制调用ONDRAW函数

}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) {
		MessageBox(_T("你还没有打开一个要保存的图像文件！"));
		return;
	}
	CString strFilter;
	strFilter ="位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
	//strFilter ="所有文件|*.*||";   //

	//CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	

	if ( IDOK != dlg.DoModal()) 
		return;
	// 如果用户没有指定文件扩展名，则为其添加一个
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;   //  获得文件名
	//if(dlg.m_ofn.nFileExtension == 0)     //获得文件扩展名
	//{
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			strExtension = "bmp"; break;
		case 2:
			strExtension = "jpg"; break;
		case 3:
			strExtension = "gif"; break;
		case 4:
			strExtension = "png"; break;
		default:
			break;
		}
		strFileName = strFileName + _T(".") + strExtension;
	//	//strFileName="C:\\Users\\Lenovo\\Desktop\\QSanguosha-Qingming\\abc.bmp";
	//}
	// 图像保存
	HRESULT hResult = g_Image.Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}
	HRESULT hResult1 = p_Image.Save(strFileName);
	if (FAILED(hResult1))
	{
		MessageBox(_T("保存图像文件失败！"));
	}
}
//显示图像红色分量
void CImage_ProcessingView::OnShowred()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_Image.m_pBits[0][j][k]=0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_Image.m_pBits[1][j][k]=0;//G

		}
	}

	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}
//显示图像绿色分量
void CImage_ProcessingView::OnShowgreen()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_Image.m_pBits[0][j][k]=0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_Image.m_pBits[2][j][k]=0;//

		}
	}

	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}

//显示图像蓝色分量
void CImage_ProcessingView::OnShowblu()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_Image.m_pBits[1][j][k]=0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_Image.m_pBits[2][j][k]=0;//G

		}
	}

	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


//void CImage_ProcessingView::OnViewToolbar()
//{
//	// TODO: 在此添加命令处理程序代码
//}

/*******************图像降质*******************/
void CImage_ProcessingView::OnImgDegradation()
{
	// TODO: 在此添加命令处理程序代码
	if(!p_Image.IsNull()){p_Image.m_CImage.Destroy();}//使可重复加载
	if(!p_Image2.IsNull()){p_Image2.m_CImage.Destroy();}//使可重复加载
	if(!g_Image.IsNull()){g_Image.m_CImage.Destroy();}//使可重复加载

	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	CDgradation Degradation;
	INT_PTR nResponse = Degradation.DoModal();
	if(nResponse!=IDOK)
	{
		return;
	}
	UpdateData(true);
	if(MyImage_::nBit==24)
	{
		p_Image.m_CImage.Create(w/Degradation.m_degradationValue,h/Degradation.m_degradationValue,24);
		p_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3//创建一个三维数组
		for(int i=0;i<3;i++)  
		{ 
			p_Image.m_pBits[i] = (BYTE**)new  BYTE*[h/Degradation.m_degradationValue]; 
			for (int j=0; j<h/Degradation.m_degradationValue; j++)         
			{   
				p_Image.m_pBits[i][j]=new BYTE[w/Degradation.m_degradationValue];   
			}
		}   

		for (int j=Degradation.m_degradationValue,j1=0;j<h;j1++,j+=Degradation.m_degradationValue)
		{
			for (int k=Degradation.m_degradationValue,k1=0;k<w;k1++,k+=Degradation.m_degradationValue)
			{
				p_Image.m_pBits[0][j1][k1]=m_Image.m_pBits[0][j][k];//B   
				p_Image.m_pBits[1][j1][k1]=m_Image.m_pBits[1][j][k];//G
				p_Image.m_pBits[2][j1][k1]=m_Image.m_pBits[2][j][k];//R
			}
		}
	}
	else if(MyImage_::nBit==8)//使该函数具有可以打开一幅灰度图的功能
	{
		p_Image2.m_CImage.Create(w/Degradation.m_degradationValue,h/Degradation.m_degradationValue,8);
		p_Image2.m_pBits2=(BYTE**)new BYTE*[h/Degradation.m_degradationValue];
		for (int j=0; j<h/Degradation.m_degradationValue; j++)  
		{
			p_Image2.m_pBits2[j]=new BYTE[w/Degradation.m_degradationValue]; 
		}
		for (int j=Degradation.m_degradationValue,j1=0;j<h;j1++,j+=Degradation.m_degradationValue)
		{
			for (int k=Degradation.m_degradationValue,k1=0;k<w;k1++,k+=Degradation.m_degradationValue)
			{
				p_Image2.m_pBits2[j1][k1]=m_Image.m_pBits2[j][k];//
			}
		}
	}
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
	/////////////////////////////////////////////////只申请了一块内存，也可不必清理,程序退出时会自动清理
}

//灰度图像
void CImage_ProcessingView::OnImgGray()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误
	m_Image.isGray=true;
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	}   
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			g_Image.m_pBits[0][j][k]=(m_Image.m_pBits[2][j][k]+m_Image.m_pBits[1][j][k]+m_Image.m_pBits[0][j][k])/3;
			g_Image.m_pBits[1][j][k]=g_Image.m_pBits[0][j][k];
			g_Image.m_pBits[2][j][k]=g_Image.m_pBits[0][j][k];
		}
	}
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}

//灰度分解
void CImage_ProcessingView::OnImgGraBreak()
{
	 //TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误
	if(graBreakflag==true)
	{
		g_Image.m_CImage.Destroy();//使可重复加载
		m_Image.isGray=false;
	}
	if(!m_Image.isGray)
	{
		OnImgGray();
	}
	CGrayScale GrayScale;
	if(GrayScale.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int  grayScaleValue=256/GrayScale.m_grayScale;//输入2时=128

	int w=g_Image.GetWidth();//获得图像的宽度
	int h=g_Image.GetHeight();//获得图像的高度
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			for(int i=0;i<GrayScale.m_grayScale;i++)
			{
				if((g_Image.m_pBits[0][j][k]>=i*grayScaleValue)&&(g_Image.m_pBits[0][j][k]<(i+1)*grayScaleValue))
				{
					g_Image.m_pBits[0][j][k]=i*255/(GrayScale.m_grayScale-1);//此算法得之不易
				}
				if((g_Image.m_pBits[1][j][k]>=i*grayScaleValue)&&(g_Image.m_pBits[1][j][k]<(i+1)*grayScaleValue))
				{
					g_Image.m_pBits[1][j][k]=i*255/(GrayScale.m_grayScale-1);
				}
				if((g_Image.m_pBits[2][j][k]>=i*grayScaleValue)&&(g_Image.m_pBits[2][j][k]<(i+1)*grayScaleValue))
				{
					g_Image.m_pBits[2][j][k]=i*255/(GrayScale.m_grayScale-1);
				}
			}
		}
	}
	graBreakflag=true;
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}



//图像差分
void CImage_ProcessingView::OnImgDiff()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得第一幅图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	CFileDialog dlg(TRUE); 
	if (IDOK == dlg.DoModal())   
	{
		if (!second_Image.IsNull()) 
			second_Image.Destroy();//若已有图片则清除
		second_Image.Load(dlg.GetPathName());
	}
	if (second_Image.IsNull()) return;
	if (w != second_Image.GetWidth() || h != second_Image.GetHeight())//判断两幅图像大小是否一致
	{
		MessageBox(_T("两幅图像大小不一致！"));
		return;
	}
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			int a=second_Image.m_pBits[0][j][k]-m_Image.m_pBits[0][j][k];
			int b=second_Image.m_pBits[1][j][k]-m_Image.m_pBits[1][j][k];
			int c=second_Image.m_pBits[2][j][k]-m_Image.m_pBits[2][j][k];
			if(a<0)a=-a;
			if(b<0)b=-b;
			if(c<0)c=-c;
			if(a>50)//去噪
			second_Image.m_pBits[0][j][k]=a;
			else second_Image.m_pBits[0][j][k]=0;
			if(b>50)
			second_Image.m_pBits[1][j][k]=b;
			else second_Image.m_pBits[1][j][k]=0;
			if(c>50)
			second_Image.m_pBits[2][j][k]=c;
			else second_Image.m_pBits[2][j][k]=0;
		}
	}
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}

//多幅图像相加去噪
void CImage_ProcessingView::OnImgDenoising()
{
	// TODO: 在此添加命令处理程序代码
	CImage_ProcessingView::textShowFlag=true;
	if(m_Image.IsNull()) 
	{
		MessageBox(_T("请先打开一幅图像！"));
		return;
	}
	int w = m_Image.GetWidth();//获得第一幅图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度 
	long*** temp_pBits;//三维数组，用于保存叠加图像的BGR像素值
	temp_pBits= (long***)new long**[3];   //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		temp_pBits[i]=(long**)new long*[h];   
	}   
	for(int i=0; i<3; i++)   
	{          
		for(int j=0; j<h; j++)         
		{   
			temp_pBits[i][j]=new long[w];   
		}
	}
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			temp_pBits[0][j][k]=m_Image.m_pBits[0][j][k];
			temp_pBits[1][j][k]=m_Image.m_pBits[1][j][k];
			temp_pBits[2][j][k]=m_Image.m_pBits[2][j][k];
		}
	}
	CImgOverlay ImgOverlay;
	if(ImgOverlay.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	MessageBox(_T("请连续选中需要叠加的几张图片！"));
	for(int i=0;i<ImgOverlay.m_imgOverlayNum;i++)
	{
		CFileDialog dlg(TRUE); 
		if (IDOK == dlg.DoModal())   
		{
			if (!overlaySecond_Image.IsNull()) 
				overlaySecond_Image.Destroy();//若已有图片则清除
			overlaySecond_Image.Load(dlg.GetPathName());
		}
		if (overlaySecond_Image.IsNull()) return;
		if (w != overlaySecond_Image.GetWidth() || h != overlaySecond_Image.GetHeight())//判断两幅图像大小是否一致
		{
			MessageBox(_T("与第一幅图像大小不一致！"));
			return;
		}
		for (int j=0;j<h;j++)
		{
			for (int k=0;k<w;k++)
			{
				temp_pBits[0][j][k]+=overlaySecond_Image.m_pBits[0][j][k];
				temp_pBits[1][j][k]+=overlaySecond_Image.m_pBits[1][j][k];
				temp_pBits[2][j][k]+=overlaySecond_Image.m_pBits[2][j][k];		
			}
		}
	}
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_Image.m_pBits[0][j][k]=temp_pBits[0][j][k]/(ImgOverlay.m_imgOverlayNum+1);
			m_Image.m_pBits[1][j][k]=temp_pBits[1][j][k]/(ImgOverlay.m_imgOverlayNum+1);
			m_Image.m_pBits[2][j][k]=temp_pBits[2][j][k]/(ImgOverlay.m_imgOverlayNum+1);	
		}
	}
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


void CImage_ProcessingView::OnOriginalHistogram()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误
	if(!m_Image.isGray)
	{
		OnImgGray();
	}
	int w=g_Image.GetWidth();//获得图像的宽度
	int h=g_Image.GetHeight();//获得图像的高度
	int graScaleNum[256];
	memset(graScaleNum, 0, sizeof(int) * 256);//数组初始化为0
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			graScaleNum[g_Image.m_pBits[0][j][k]]++;
		}
	}
	float graScaleRatio[256];
	memset(graScaleRatio, 0, sizeof(float) * 256);//数组初始化为0
	for(int i=0;i<256;i++)
	{
		graScaleRatio[i]=((float)graScaleNum[i])/(float)(w*h);
	}
	CHistogramImg HistogramImg;
	HistogramImg.pGraScaleRatio=graScaleRatio;
	HistogramImg.DoModal();
}
//图像的均衡化
void CImage_ProcessingView::OnImgEquilibrium()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	int graScaleNum[256];
	memset(graScaleNum, 0, 256 * sizeof(int));
	//统计原始直方图    
	for (int i = 0; i <h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int ss = (m_Image.m_pBits[0][i][j] + m_Image.m_pBits[1][i][j] + m_Image.m_pBits[2][i][j])/3;
			graScaleNum[ss]++;
		}
	}
	//统计原始直方图各灰度比例 
	float graScaleRatio[256];
	memset(graScaleRatio, 0, sizeof(float) * 256);//数组初始化为0
	for(int i=0;i<256;i++)
	{
		graScaleRatio[i]=((float)graScaleNum[i])/(float)(w*h);
	}
	CHistogramImg HistogramImg;
	HistogramImg.pGraScaleRatio=graScaleRatio;
	HistogramImg.DoModal();
	//统计各个分量的个数  
	int countB[256];
	int countG[256];
	int countR[256];
	//各个分量的比例（归一化后）  
	float graScaleRatioB[256];
	float graScaleRatioG[256];
	float graScaleRatioR[256];
	//累计各个分量  对之前的分量的累计  
	float tempArrayB[256];
	float tempArrayG[256];
	float tempArrayR[256];
	//均衡化后各个分量的比例 比例*255
	int eqHistB[256];
	int eqHistG[256];
	int eqHistR[256];
	//初始化数组为0
	memset(countB, 0, sizeof(int)* 256);
	memset(countG, 0, sizeof(int)* 256);
	memset(countR, 0, sizeof(int)* 256);
	memset(graScaleRatioB, 0, sizeof(float) * 256);//数组初始化为0
	memset(graScaleRatioG, 0, sizeof(float) * 256);//
	memset(graScaleRatioR, 0, sizeof(float) * 256);//
	memset(tempArrayB, 0, sizeof(float) * 256);//数组初始化为0
	memset(tempArrayG, 0, sizeof(float) * 256);//
	memset(tempArrayR, 0, sizeof(float) * 256);//
	//统计三个分量的灰度值	
	for (int j = 0; j<h; j++)
	{
		for (int k = 0; k<w; k++)
		{
			(countB[m_Image.m_pBits[0][j][k]])++;
			(countG[m_Image.m_pBits[1][j][k]])++;
			(countR[m_Image.m_pBits[2][j][k]])++;
		}
	}
	//归一化
	int size=w*h;
	for (int i = 0; i<256; i++)
	{
		graScaleRatioB[i] = (float)countB[i] / (float)size;
		graScaleRatioG[i] = (float)countG[i] / (float)size;
		graScaleRatioR[i] = (float)countR[i] / (float)size;
	}
	//累计灰度比例 均衡预处理
	for (int i = 0; i<256; i++)
	{
		if (i == 0)
		{
		    tempArrayB[i]=graScaleRatioB[i];
			tempArrayG[i]=graScaleRatioG[i];
			tempArrayR[i]=graScaleRatioR[i];

		}
		else
		{
			tempArrayB[i]=graScaleRatioB[i] + tempArrayB[i - 1];
			tempArrayG[i]=graScaleRatioG[i] + tempArrayG[i - 1];
			tempArrayR[i]=graScaleRatioR[i] + tempArrayR[i - 1];
		}
	}
	//缩放至0~255范围内  并做四舍五入处理
	for (int i = 0; i<256; i++)
	{
		eqHistB[i] = (int)(tempArrayB[i] * 255.0 + 0.5);
		eqHistG[i] = (int)(tempArrayG[i] * 255.0 + 0.5);
		eqHistR[i] = (int)(tempArrayR[i] * 255.0 + 0.5);
	}
	//进行灰度映射 均衡化
	p_Image.m_CImage.Create(w,h,24);
	p_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		p_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			p_Image.m_pBits[i][j]=new BYTE[w];   
		}
	}
	for (int j = 0; j<h; j++)
	{
		for (int k = 0; k<w; k++)
		{
			p_Image.m_pBits[0][j][k] = eqHistB[m_Image.m_pBits[0][j][k]];
			p_Image.m_pBits[1][j][k] = eqHistG[m_Image.m_pBits[1][j][k]];
			p_Image.m_pBits[2][j][k] = eqHistR[m_Image.m_pBits[2][j][k]];
		}
	}
	int dstNewHist[256];
	memset(dstNewHist, 0, 256 * sizeof(int));
	//统计均衡化后的直方图灰度及比例并显示  
	for (int i = 0; i <h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int ss = (p_Image.m_pBits[0][i][j] + p_Image.m_pBits[1][i][j] + p_Image.m_pBits[2][i][j])/3;
			dstNewHist[ss]++;
		}
	}
	Invalidate(1);
	float newGraScaleRatio[256];
	memset(newGraScaleRatio, 0, sizeof(float) * 256);//数组初始化为0
	for(int i=0;i<256;i++)
	{
		newGraScaleRatio[i]=((float)dstNewHist[i])/(float)size;
	}
	CHistogramImg NewHistogramImg;
	NewHistogramImg.pGraScaleRatio=newGraScaleRatio;
	NewHistogramImg.DoModal();
}

//图像的规定化
void CImage_ProcessingView::OnImgNormalization()
{
	
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	int graScaleNum[256];
	memset(graScaleNum, 0, 256 * sizeof(int));
	//统计原始直方图    
	for (int i = 0; i <h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int ss = (m_Image.m_pBits[0][i][j] + m_Image.m_pBits[1][i][j] + m_Image.m_pBits[2][i][j])/3;
			graScaleNum[ss]++;
		}
	}
	//统计原始直方图各灰度比例 
	float graScaleRatio[256];
	memset(graScaleRatio, 0, sizeof(float) * 256);//数组初始化为0
	for(int i=0;i<256;i++)
	{
		graScaleRatio[i]=((float)graScaleNum[i])/(float)(w*h);
	}
	CHistogramImg HistogramImg;
	HistogramImg.pGraScaleRatio=graScaleRatio;
	HistogramImg.DoModal();
	//统计各个分量的个数  
	int countB[256];
	int countG[256];
	int countR[256];
	//各个分量的比例（归一化后）  
	float graScaleRatioB[256];
	float graScaleRatioG[256];
	float graScaleRatioR[256];
	//累计各个分量  对之前的分量的累计  
	float tempArrayB[256];
	float tempArrayG[256];
	float tempArrayR[256];
	//定义规定化相关数组
	float GuidingBP[256]; float GuidingGP[256]; float GuidingRP[256];
	int YinB[256]; int YinG[256]; int YinR[256];
	//初始化数组为0
	memset(countB, 0, sizeof(int)* 256);
	memset(countG, 0, sizeof(int)* 256);
	memset(countR, 0, sizeof(int)* 256);
	memset(graScaleRatioB, 0, sizeof(float) * 256);//数组初始化为0
	memset(graScaleRatioG, 0, sizeof(float) * 256);//
	memset(graScaleRatioR, 0, sizeof(float) * 256);//
	memset(tempArrayB, 0, sizeof(float) * 256);//数组初始化为0
	memset(tempArrayG, 0, sizeof(float) * 256);//
	memset(tempArrayR, 0, sizeof(float) * 256);//
	//统计三个分量的灰度值	
	for (int j = 0; j<h; j++)
	{
		for (int k = 0; k<w; k++)
		{
			(countB[m_Image.m_pBits[0][j][k]])++;
			(countG[m_Image.m_pBits[1][j][k]])++;
			(countR[m_Image.m_pBits[2][j][k]])++;
		}
	}
	//归一化
	int size=w*h;
	for (int i = 0; i<256; i++)
	{
		graScaleRatioB[i] = (float)countB[i] / (float)size;
		graScaleRatioG[i] = (float)countG[i] / (float)size;
		graScaleRatioR[i] = (float)countR[i] / (float)size;
	}
	//累计灰度比例 均衡预处理
	for (int i = 0; i<256; i++)
	{
		if (i == 0)
		{
		    tempArrayB[i]=graScaleRatioB[i];
			tempArrayG[i]=graScaleRatioG[i];
			tempArrayR[i]=graScaleRatioR[i];

		}
		else
		{
			tempArrayB[i]=graScaleRatioB[i] + tempArrayB[i - 1];
			tempArrayG[i]=graScaleRatioG[i] + tempArrayG[i - 1];
			tempArrayR[i]=graScaleRatioR[i] + tempArrayR[i - 1];
		}
	}
	//规定灰度级概率
	for (int i = 0; i<256; i++)
	{
		GuidingBP[i] = (128 + i) / (256 * 256 * 1.0f);
		GuidingGP[i] = (128 + i) / (256 * 256 * 1.0f);
		GuidingRP[i] = (128 + i) / (256 * 256 * 1.0f);

	}

	for (int i = 1; i<256; i++)
	{
		GuidingBP[i] = GuidingBP[i] + GuidingBP[i - 1];
		GuidingGP[i] = GuidingGP[i] + GuidingGP[i - 1];
		GuidingRP[i] = GuidingRP[i] + GuidingRP[i - 1];

	}
	//规定灰度级找对应的均衡灰度级
	for (int i = 0; i<256; i++)
	{
		float minB = 3.0f; float minG = 3.0f; float minR = 3.0f;
		int jjB = 0, jjG = 0, jjR = 0;
		for (int j = 0; j<256; j++)
		{
			if (minB>abs(tempArrayB[i] - GuidingBP[j]))
			{
				minB = abs(tempArrayB[i] - GuidingBP[j]);
				jjB = j;
			}
			if (minG>abs(tempArrayG[i] - GuidingGP[j]))
			{
				minG = abs(tempArrayG[i] - GuidingGP[j]);
				jjG = j;
			}
			if (minR>abs(tempArrayR[i] - GuidingRP[j]))
			{
				minR = abs(tempArrayR[i] - GuidingRP[j]);
				jjR = j;
			}

		}
		YinB[i] = jjB; YinG[i] = jjG; YinR[i] = jjR;
	}
	p_Image.m_CImage.Create(w,h,24);
	p_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		p_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			p_Image.m_pBits[i][j]=new BYTE[w];   
		}
	}
	for (int j = 0; j<h; j++)
	{
		for (int k = 0; k<w; k++)
		{
			p_Image.m_pBits[0][j][k] =YinB[m_Image.m_pBits[0][j][k]];
			p_Image.m_pBits[1][j][k] = YinG[m_Image.m_pBits[1][j][k]];
			p_Image.m_pBits[2][j][k] = YinR[m_Image.m_pBits[2][j][k]];
		}
	}
	int dstNewHist[256];
	memset(dstNewHist, 0, 256 * sizeof(int));
	//统计均衡化后的直方图灰度及比例并显示  
	for (int i = 0; i <h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int ss = (p_Image.m_pBits[0][i][j] + p_Image.m_pBits[1][i][j] + p_Image.m_pBits[2][i][j])/3;
			dstNewHist[ss]++;
		}
	}
	Invalidate(1);
	float newGraScaleRatio[256];
	memset(newGraScaleRatio, 0, sizeof(float) * 256);//数组初始化为0
	for(int i=0;i<256;i++)
	{
		newGraScaleRatio[i]=((float)dstNewHist[i])/(float)size;
	}
	CHistogramImg NewHistogramImg;
	NewHistogramImg.pGraScaleRatio=newGraScaleRatio;
	NewHistogramImg.DoModal();
}



/*************************************************
Others: 模板
	**********
	1   1   1
	1   1	1
	1	1	1
	**********
g = (f(x-1,y-1) + f(x,y-1)+ f(x+1,y-1) + f(x-1,y) + f(x,y) + f(x+1,y)  + f(x-1,y+1) + f(x,y+1) + f(x+1,y+1))/9
*******	******************************************/
//图像的均值滤波
void CImage_ProcessingView::OnMeanfilter()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	}   

	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);

	int MN=TemplateSize.m_templateSize;

	int sum=0;
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						sum+= m_Image.m_pBits[i][j+m][k+n];
					}
				}	
				g_Image.m_pBits[i][j][k]=sum/(MN*MN);
				sum=0;
			}
		}
	}
	Invalidate(1);
}

/*************************************************
拉普拉斯滤波
*************************************************/
void CImage_ProcessingView::OnLaplassefilter()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	}   

	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;

	int sum=0;
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						if(m==0&&n==0)
						{
							sum-=(MN*MN-1)*m_Image.m_pBits[i][j+m][k+n];
						}
						else
							sum+= m_Image.m_pBits[i][j+m][k+n];
					}
				}	
				g_Image.m_pBits[i][j][k]=sum/(MN*MN);
				sum=0;
			}
		}
	}
	Invalidate(1);
}
/*************************************************
Description: 数据的冒泡排序
Input:
	pData:待排序数据 
    count ： 数据的个数 
Output:排序后的数据 
*************************************************/
void BubbleSort(int* pData, int count)  
{  
    int temp;  
    for (int i = 1; i < count; i++)  
    {  
        for (int j = count - 1; j >= i; j--)  
        {  
            if (pData[j] < pData[j - 1])  
            {  
                temp = pData[j - 1];  
                pData[j - 1] = pData[j];  
                pData[j] = temp;  
            }  
        }  
    }  
} 
//中值滤波
void CImage_ProcessingView::OnMedianfilter()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 

	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;

	int *temp=new int[MN*MN]; //保存图像的值

	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				int p=0;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						temp[p]=m_Image.m_pBits[i][j+m][k+n];
						p++;
					}
				}	
				BubbleSort(temp,MN*MN);
				g_Image.m_pBits[i][j][k]=temp[(MN*MN-1)/2];
			}
		}
	}
	delete []temp;
	Invalidate(1);
}
//最大值滤波
void CImage_ProcessingView::OnMaxValuefilter()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 
	
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;

	int *temp=new int[MN*MN]; //保存图像的值
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				int p=0;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						temp[p]=m_Image.m_pBits[i][j+m][k+n];
						p++;
					}
				}	
				BubbleSort(temp,MN*MN);
				g_Image.m_pBits[i][j][k]=temp[MN*MN-1];
			}
		}
	}
	delete []temp;
	Invalidate(1);
}
//最小值滤波
void CImage_ProcessingView::OnMinValuefilter()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 
	
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;

	int *temp=new int[MN*MN]; //保存图像的值
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				int p=0;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						temp[p]=m_Image.m_pBits[i][j+m][k+n];
						p++;
					}
				}	
				BubbleSort(temp,MN*MN);
				g_Image.m_pBits[i][j][k]=temp[0];
			}
		}
	}
	delete []temp;
	Invalidate(1);
}
//中点滤波
void CImage_ProcessingView::OnMidValuefilter()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;
	int *temp=new int[MN*MN]; //保存图像的值
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				int p=0;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						temp[p]=m_Image.m_pBits[i][j+m][k+n];
						p++;
					}
				}	
				BubbleSort(temp,MN*MN);
				g_Image.m_pBits[i][j][k]=(temp[0]+temp[MN*MN-1])/2;
			}
		}
	}
	delete []temp;
	Invalidate(1);
}
//逆谐波均值滤波
void CImage_ProcessingView::OnInverseHarmonicMeanfilter()
{
	// TODO: 在此添加命令处理程序代码
		if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 
	
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;

	double mux=0;
	double mux1=0;
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				mux=mux1=0;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						mux+=pow(m_Image.m_pBits[i][j+m][k+n],Q+1);
						mux1+=pow(m_Image.m_pBits[i][j+m][k+n],Q);
					}
				}	
				g_Image.m_pBits[i][j][k]=mux/mux1;
			}
		}
	}
	Invalidate(1);
}
//谐波均值滤波
void CImage_ProcessingView::OnHarmonicMeanfilter()
{
	// TODO: 在此添加命令处理程序代码
			if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;
	double sum=0;
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				sum=0;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{
						sum+=(double)1/m_Image.m_pBits[i][j+m][k+n];
					}
				}	
				g_Image.m_pBits[i][j][k]=MN*MN/sum;
			}
		}
	}
	Invalidate(1);
}
//几何均值滤波
void CImage_ProcessingView::OnGeometricMeanfilter()
{
	// TODO: 在此添加命令处理程序代码
			if(m_Image.IsNull()) 
	{
			return;
	}
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();
	g_Image.m_CImage.Create(w,h,24);
	g_Image.m_pBits= (BYTE***) new BYTE**[3]; //一维长度为3
	for(int i=0;i<3;i++)  
	{ 
		g_Image.m_pBits[i] = (BYTE**)new  BYTE*[h]; 
		for (int j=0; j<h; j++)         
		{   
			g_Image.m_pBits[i][j]=new BYTE[w];   
		}
	} 
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;
	double sum=1;
	for(int i=0;i<3;i++)
	{
		for (int j=(MN-1)/2;j<h-(MN-1)/2;j++)
		{
			for (int k=(MN-1)/2;k<w-(MN-1)/2;k++)
			{
				sum=1;
				for (int m=(1-MN)/2;m<(MN+1)/2;m++)
				{
					for (int n=(1-MN)/2;n<(MN+1)/2;n++)
					{						
						sum*=pow((double)m_Image.m_pBits[i][j+m][k+n],(double)1.0/(MN*MN));
					}
				}	
				g_Image.m_pBits[i][j][k]=(int)sum;
			}
		}
	}
	Invalidate(1);
}


void CImage_ProcessingView::FFT(complex<double>*TD, complex<double>*FD, int index)
{
	int dotcount = 0;
	int bfsize = 0, p = 0;
	double angle;//角度
	complex<double>*W,*X1,*X2,*X;
	dotcount = 1 << index;//傅立叶变换总点数。
	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];
	for (int i = 0; i<dotcount / 2; i++)//计算加权系数 
	{   
		angle=-i*2*pi/dotcount; 
		W[i]=complex<double>(cos(angle),sin(angle)); 
	}
	memcpy(X1, TD, sizeof(complex<double>)*dotcount);//清零
	for (int k = 0; k<index; k++)
	{
		for (int j = 0;j<1<<k;j++)
		{
			bfsize = 1 << (index - k);//每个蝶形节的点数
			for (int i = 0; i<bfsize / 2; i++)
			{
				p = j*bfsize;//第j个蝶形节的总点数。
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//按频域抽选的基-2 FFT算法，加权系数仅挑选一部分
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}
	for (int j = 0; j<dotcount; j++)
	{
		p = 0;
		for (int i = 0; i<index; i++)
		{
			if (j&(1 << i))
			{
				p += 1 << (index - i - 1);
			}
		}
		FD[j] = X1[p];
	}
	delete W;
	delete X1;
	delete X2;
}

void CImage_ProcessingView::ImgGray()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;

	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	for (int j = 0; j<h; j++)
	{
		for (int k = 0; k<w; k++)
		{
			BYTE b = m_Image.m_pBits[0][j][k];//B   
			BYTE g = m_Image.m_pBits[1][j][k];//G
			BYTE r = m_Image.m_pBits[2][j][k];//R
			BYTE ave = (b + g + r) / 3;
			m_Image.m_pBits[0][j][k] = ave;
			m_Image.m_pBits[1][j][k] = ave;
			m_Image.m_pBits[2][j][k] = ave;
		}
	}
	Invalidate(1); 
}
//FFT变换
void CImage_ProcessingView::OnFftTransform()
{

	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;//图像进行扩展,寻找2的幂次方  
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while(h_extend<=h/2)
	{
		h_extend*=2;
		h_index++;
	}
	while (w_extend <= w/2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD=new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	complex<double>*pFD=new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for(int i = 0; i<h_extend*w_extend; i++)//图像补零处理
	{
		pTD[i]=complex<double>(0,0);
	}
	for(int i = 0; i<h; i++)//把图像的值传给pTD
	{
		for(int j=0;j<w;j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//频谱搬移到频谱图的中心
		}
	}
	for(int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i],&pFD[w_extend*i],w_index);//对w方向进行快速傅立叶变换 
	}
	for(int i = 0; i<h_extend; i++)//把pFD进行转置
	{
		for(int j=0;j<w_extend;j++)
		{
			pTD[h_extend*j+i]=pFD[w_extend*i+j];
		}
	}
	for(int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对w方向进行快速傅立叶变换
	}
	for(int i = 0; i<h_extend; i++)//对变换结果进行转置
	{
		for (int j=0;j<w_extend;j++)
		{
			pTD[w_extend*i+j]=pFD[h_extend*j+i];
		}
	}
	for(int i = 0; i<h_extend; i++)
	{
		for(int j = 0; j<w_extend; j++)
		{
			BYTE temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
		    temp = (temp>255) ? 255 : temp;
		    m_Image.m_pBits[0][i][j] = temp;
		    m_Image.m_pBits[1][i][j] = temp;
		    m_Image.m_pBits[2][i][j] = temp;
		}
	}
	Invalidate(1);
}

//RGB到HSI转换
void CImage_ProcessingView::OnRgb2Hsi()
{
	// TODO:  在此添加命令处理程序代码
	if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	BYTE*** m_nBits = nullptr;
	m_nBits = (BYTE***)new BYTE**[3];
	for (int i = 0; i<3; i++)
	{
		m_nBits[i] = (BYTE**)new BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_nBits[i][j] = new BYTE[w];
		}
	}
	double H, S, I,up,down,angle,min;
	BYTE b,g,r;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			b = m_Image.m_pBits[0][i][j];
			g = m_Image.m_pBits[1][i][j];
			r = m_Image.m_pBits[2][i][j];

			up = (2 * r - g - b) / 2.0;
			down = sqrt(double((r - b)*(g - b) + (r - g)*(r - g)));
			if (down == 0)
				down = 0.01;
			angle = acos(up / down);
			if (b <= g)
				H = angle / (2 * 3.14);
			else
				H = (2 * 3.14 - angle) / (2 * 3.14);
			min = r < g ? r : g;
			min = b < min ? b : min;
			S = 1 - 3.0*min / (b + g + r);
			I = (b + g + r) / 3.0;

			m_nBits[0][i][j] = int(H * 255);
			m_nBits[1][i][j] = int(S * 255);
			m_nBits[2][i][j] = int(I);
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			m_Image.m_pBits[0][i][j] = m_nBits[0][i][j];
			m_Image.m_pBits[1][i][j] = m_nBits[1][i][j];
			m_Image.m_pBits[2][i][j] = m_nBits[2][i][j];
		}
	}
	Invalidate(1);
}

//显示HSI三个分量
void CImage_ProcessingView::OnShowHsi()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	OnRgb2Hsi();
	CUniversalDlg UniversalDlg;
	int index;
	if (UniversalDlg.DoModal() == IDOK)
	{
		index = UniversalDlg.m_intNumber;
	}
	else if (UniversalDlg.DoModal() == IDCANCEL)
	{
		return;
	}
	switch (index)
	{
		case 0://得到H分量
			for (int i = 0; i < h; i++)
			{
				for (int j = 0; j < w; j++)
				{
					m_Image.m_pBits[1][i][j] = m_Image.m_pBits[0][i][j];
					m_Image.m_pBits[2][i][j] = m_Image.m_pBits[0][i][j];
				}
			}
		case 1://得到S分量
			for (int i = 0; i < h; i++)
			{
				for (int j = 0; j < w; j++)
				{
					m_Image.m_pBits[0][i][j] = m_Image.m_pBits[1][i][j];
					m_Image.m_pBits[2][i][j] = m_Image.m_pBits[1][i][j];
				}
			}
		case 2://得到I分量
			for (int i = 0; i < h; i++)
			{
				for (int j = 0; j < w; j++)
				{
					m_Image.m_pBits[0][i][j] = m_Image.m_pBits[2][i][j];
					m_Image.m_pBits[1][i][j] = m_Image.m_pBits[2][i][j];
				}
			}
	}
	Invalidate(1);
}
//产生高斯样本
double CImage_ProcessingView::GausSample(void)
{
	//以U为均值，D为均方差
	double sum=0;
	for(int i=0;i<12;i++) 
		sum+=rand()/32767.00;
	return (20+30*(sum-6));
}
//添加高斯噪声
void CImage_ProcessingView::OnAddGaussianNoise()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
	return;//判断图像是否为空，如果对空图像进行操作会出现位置的错误
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	for(int j=0;j<w;j++)
	{
		for(int i=0;i<h;i++)
			{
				m_Image.m_pBits[0][i][j]+=int(GausSample());
				m_Image.m_pBits[1][i][j]+=int(GausSample());
			    m_Image.m_pBits[2][i][j]+=int(GausSample());
			}		
	}
	Invalidate(1);
}

//添加椒盐噪声
void CImage_ProcessingView::OnAddSaltAndPepperNoise()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) 
		return;//判断图像是否为空，如果对空图像进行操作会出现位置的错误
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度

	float p_a=0.1;
	float p_b=0.1;

	float P_Sum=p_a+p_b;//p_a是盐噪声的概率， p_b是胡椒噪声的概率。
	float x_s=1/P_Sum;
	float p_a_1=p_a*x_s;
	for(int j=0;j<w;j++)
	{
		for(int i=0;i<h;i++)
		{		
			if(rand()/32767.0<P_Sum)
			{
				if(rand()/32767.0<p_a_1)
				{ 	
					m_Image.m_pBits[0][i][j]=255;
					m_Image.m_pBits[1][i][j]=255;
					m_Image.m_pBits[2][i][j]=255;
				}
				else
				{	
					m_Image.m_pBits[0][i][j]=0;
					m_Image.m_pBits[1][i][j]=0;
					m_Image.m_pBits[2][i][j]=0;
				}
			}
		}
	}
	Invalidate(1);	
}


int CImage_ProcessingView::Midean(int p[], int k, int l)
{
	sort(p, p + k*l);
	return p[(k*l - 1) / 2];
}
int CImage_ProcessingView::Minimum(int p[], int k, int l)
{
	sort(p, p + k*l);
	return p[0];
}
int CImage_ProcessingView::Maximum(int p[], int k, int l)
{
	sort(p, p + k*l);
	return p[k*l-1];
}
//自适应中值滤波
void CImage_ProcessingView::OnAdaptivemedianfilter()
{
	// TODO: 在此添加命令处理程序代码
		// TODO:  在此添加命令处理程序代码
	if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	//添加高斯噪声和椒盐噪声
	OnAddGaussianNoise();
	OnAddSaltAndPepperNoise();
	//输入模板最大值
	CTemplateSize TemplateSize;
	if(TemplateSize.DoModal()!=IDOK)
	{
		return;
	}
	UpdateData(true);
	int MN=TemplateSize.m_templateSize;

	BYTE*** m_nBits = nullptr;
	m_nBits = (BYTE***)new BYTE**[3];
	for (int i = 0; i<3; i++)
	{
		m_nBits[i] = (BYTE**)new BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_nBits[i][j] = new BYTE[w];
		}
	}
	int minsize = 3;
	int midvalue = 0;
	//灰度处理
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			BYTE b = m_Image.m_pBits[0][i][j];//B
			BYTE g = m_Image.m_pBits[1][i][j];//G
			BYTE r = m_Image.m_pBits[2][i][j];//R
			BYTE average = int((b + g + r) / 3);
			m_Image.m_pBits[0][i][j] = average;
			m_Image.m_pBits[1][i][j] = average;
			m_Image.m_pBits[2][i][j] = average;
		}
	}
	//开辟数组空间
	int *blue = new int[MN*MN];
	int *green = new int[MN*MN];
	int *red = new int[MN*MN];
	for (int i = (MN - 1) / 2; i < h - (MN - 1) / 2; i++)
	{
		for (int j = (MN - 1) / 2; j < w - (MN - 1) / 2; j++)
		{
		loop: int b_count = 0, g_count = 0, r_count = 0;
			for (int m = i - (minsize - 1) / 2; m <= i + (minsize - 1) / 2; m++)
			{
				for (int n = j - (minsize - 1) / 2; n <= j + (minsize - 1) / 2; n++)
				{
					blue[b_count++] = m_Image.m_pBits[0][m][n];
				}
			}
			int A1 = Midean(blue, minsize, minsize) - Minimum(blue, minsize, minsize);
			int A2 = Midean(blue, minsize, minsize) - Maximum(blue, minsize, minsize);
			if (A1 > 0 && A2 < 0)//A1 > 0 & A2 < 0，处理 B
			{
				int B1 = blue[b_count / 2] - Minimum(blue, minsize, minsize);
				int B2 = blue[b_count / 2] - Maximum(blue, minsize, minsize);
				if (B1>0 && B2 < 0)//B1 > 0 & B2 < 0，则输出中值
					midvalue = blue[b_count / 2];
				else
					midvalue = Midean(blue, minsize, minsize);
			}
			else if (minsize < MN)//增大模板尺寸，从新滤波处理
			{
				minsize += 2;
				goto loop;
			}
			else//输出中值
				midvalue = Midean(blue, minsize, minsize);

			m_nBits[0][i][j] = midvalue;
			m_nBits[1][i][j] = midvalue;
			m_nBits[2][i][j] = midvalue;
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			m_Image.m_pBits[0][i][j] = m_nBits[0][i][j];
			m_Image.m_pBits[1][i][j] = m_nBits[1][i][j];
			m_Image.m_pBits[2][i][j] = m_nBits[2][i][j];
		}
	}
	Invalidate(1);
}
//对RGB作图像均衡
void CImage_ProcessingView::OnRgbEqualization()
{
	if (m_Image.IsNull())
		return;

	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度
	float B[256] = { 0 }, G[256] = { 0 }, R[256] = { 0 };
	float ProbilityB[256] = { 0 }, ProbilityG[256] = { 0 }, ProbilityR[256] = { 0 };
	float AccProbility[256] = { 0 };
	float ProbilitySum[3] = { 0, 0, 0 };
	int ValueB[256], ValueG[256], ValueR[256];

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			(B[m_Image.m_pBits[0][i][j]])++;
			(G[m_Image.m_pBits[1][i][j]])++;
			(R[m_Image.m_pBits[2][i][j]])++;
		}
	}
	for (int k = 0; k < 256; k++)//计算各分量像素值概率
	{
		ProbilityB[k] = (double)B[k] / (w*h);
		ProbilityG[k] = (double)G[k] / (w*h);
		ProbilityR[k] = (double)R[k] / (w*h);
	}
	for (int k = 0; k < 256; k++)//缩放至0~255范围内  并做四舍五入处理
	{
		ProbilitySum[0] += ProbilityB[k];
		ProbilitySum[1] += ProbilityG[k];
		ProbilitySum[2] += ProbilityR[k];
		ValueB[k] = 255 * ProbilitySum[0] + 0.5;
		ValueG[k] = 255 * ProbilitySum[1] + 0.5;
		ValueR[k] = 255 * ProbilitySum[2] + 0.5;
	}
	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			m_Image.m_pBits[0][j][k] = ValueB[m_Image.m_pBits[0][j][k]];
			m_Image.m_pBits[1][j][k] = ValueG[m_Image.m_pBits[1][j][k]];
			m_Image.m_pBits[2][j][k] = ValueR[m_Image.m_pBits[2][j][k]];
		}
	}
	Invalidate(1);
}
//只对I分量作均衡
void CImage_ProcessingView::OnIEqualization()
{
	// TODO: 在此添加命令处理程序代码
		if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	OnRgb2Hsi();
	float Count[256] = { 0 };
	float Probility[256] = { 0 };
	float AccProbility[256] = { 0 };
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			(Count[m_Image.m_pBits[2][i][j]])++;//统计灰度值
		}
	}
	for (int k = 0; k < 256; k++)
	{
		Probility[k] = Count[k] / (w*h);//统计灰度值
	}
	AccProbility[0] = Probility[0];
	for (int m = 1; m < 256; m++)
	{
		AccProbility[m] = AccProbility[m - 1] + Probility[m];
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			m_Image.m_pBits[2][i][j] = AccProbility[m_Image.m_pBits[2][i][j]] * 255;
		}
	}
	Invalidate();
}

//全局阈值分割图像
void CImage_ProcessingView::OnGlobalThresholdSeg()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	BYTE*** m_nBits = nullptr;
	m_nBits = (BYTE***)new BYTE**[3];
	for (int i = 0; i<3; i++)
	{
		m_nBits[i] = (BYTE**)new BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_nBits[i][j] = new BYTE[w];
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			BYTE b = m_Image.m_pBits[0][i][j];//B
			BYTE g = m_Image.m_pBits[1][i][j];//G
			BYTE r = m_Image.m_pBits[2][i][j];//R
			BYTE average = int((b + g + r) / 3);

			m_Image.m_pBits[0][i][j] = average;
			m_Image.m_pBits[1][i][j] = average;
			m_Image.m_pBits[2][i][j] = average;
		}
	}
	int Count[256] = {};
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Count[m_Image.m_pBits[0][i][j]]++;
		}
	}
	int  t0=0, t1, t2, threshold, gray_ave;
	double u0=0.0, u1, u2;
	for (int i = 0; i<256; i++)
	{
		t0 += Count[i];
		u0 += i*Count[i];
	}
	//  计算此范围灰度的平均值
	gray_ave = (int)(u0 / t0);                           
	do
	{
		threshold = gray_ave;
		t1 = 0;
		u1 = 0;
		for (int i = 0; i <= threshold; i++)
		{            
			//  计算低灰度组的累加和  
			t1 += Count[i];
			u1 += i*Count[i];
		}

		t2 = t0 - t1;
		u2 = u0 - u1;
		if (t1)
			u1 = u1 / t1;      //  计算低灰度组的平均值  
		else
			u1 = 0;
		if (t2)
			u2 = u2 / t2;       //  计算高灰度组的平均值  
		else
			u2 = 0;
		gray_ave = (int)((u1 + u2) / 2);    //  得到新的阈值估计值  
	} 
	while (threshold != gray_ave);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m_Image.m_pBits[0][i][j]>threshold)
			{
				m_nBits[0][i][j] = 255;
				m_nBits[1][i][j] = 255;
				m_nBits[2][i][j] = 255;
			}
			else
			{
				m_nBits[0][i][j] = 0;
				m_nBits[1][i][j] = 0;
				m_nBits[2][i][j] = 0;
			}
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			m_Image.m_pBits[0][i][j] = m_nBits[0][i][j];
			m_Image.m_pBits[1][i][j] = m_nBits[1][i][j];
			m_Image.m_pBits[2][i][j] = m_nBits[2][i][j];
		}
	}
	Invalidate(1);
}

//最大方差法分割图像
void CImage_ProcessingView::OnMaxVarianceSeg()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	//灰度化图像
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			BYTE b = m_Image.m_pBits[0][i][j];//B
			BYTE g = m_Image.m_pBits[1][i][j];//G
			BYTE r = m_Image.m_pBits[2][i][j];//R
			BYTE average = int((b + g + r) / 3);

			m_Image.m_pBits[0][i][j] = average;
			m_Image.m_pBits[1][i][j] = average;
			m_Image.m_pBits[2][i][j] = average;
		}
	}

	BYTE*** m_nBits = nullptr;
	m_nBits = (BYTE***)new BYTE**[3];
	for (int i = 0; i<3; i++)
	{
		m_nBits[i] = (BYTE**)new BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_nBits[i][j] = new BYTE[w];
		}
	}

	int Count[256] = {};
	int best = 0;
	int g_max = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			Count[m_Image.m_pBits[0][i][j]]++;
		}
	}
	for (int n = 0; n < 256; n++)
	{
		float u0 = 0.0,u1=0.0;
		float w0 = 0.0,w1=0.0;
		//背景的像素总和 
		for (int j = 0; j <= n; j++) 
		{
			w1 += Count[n];  
			u1 += j*Count[n]; 
		}
		u1 = u1 / w1; 
		w1 = w1 / (w*h); // 背景像素点比例  
		//前景各分量值计算
		for (int k =n + 1; k<255; k++)
		{
			w0 += Count[n];  
			u0 += k*Count[n]; 
		}
		u0 = u0 / w0;
		w0 = w0 / (w*h); // 前景像素点比例 
		//类间方差 
		double g = w0*w1*(u1 - u0)*(u1 - u0);
		if (g>g_max)
		{
			g_max = g;
			best = n;
		}
	}
	for(int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m_Image.m_pBits[0][i][j]>best)
			{
				m_nBits[0][i][j] = 255;
				m_nBits[1][i][j] = 255;
				m_nBits[2][i][j] = 255;
			}
			else
			{
				m_nBits[0][i][j] = 0;
				m_nBits[1][i][j] = 0;
				m_nBits[2][i][j] = 0;
			}
		}
	}
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			m_Image.m_pBits[0][i][j] = m_nBits[0][i][j];
			m_Image.m_pBits[1][i][j] = m_nBits[1][i][j];
			m_Image.m_pBits[2][i][j] = m_nBits[2][i][j];
		}
	}
	Invalidate(1);
}

//霍夫变换检测直线
void CImage_ProcessingView::OnHoughDetectionLine()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull())
		return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	//二值化一幅图像
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			BYTE b = m_Image.m_pBits[0][i][j];
			BYTE g = m_Image.m_pBits[1][i][j];
			BYTE r = m_Image.m_pBits[2][i][j];
			BYTE average = int((b + g + r) / 3);
			if (average>128)
				average = 255;
			else
				average = 0;
			m_Image.m_pBits[0][i][j] = average;
			m_Image.m_pBits[1][i][j] = average;
			m_Image.m_pBits[2][i][j] = average;
		}
	}
	BYTE*** m_nBits = nullptr;//创建三维数组空间
	m_nBits = (BYTE***)new BYTE**[3];
	for (int i = 0; i<3; i++)
	{
		m_nBits[i] = (BYTE**)new BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_nBits[i][j] = new BYTE[w];
		}
	}
	//做拉普拉斯算子边缘检测
	int temp[9] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
	for (int m = 1; m < h - 1; m++)
	{     
		for (int n = 1; n < w - 1; n++)
		{
			int sum[3] = { 0, 0, 0 };
			int index[3] = { 0, 0, 0 };
			for (int i = m - 1; i <= m + 1; i++)
			{
				for (int j = n - 1; j <= n + 1; j++)
				{
					sum[0] += m_Image.m_pBits[0][i][j] * temp[index[0]++];
					sum[1] += m_Image.m_pBits[1][i][j] * temp[index[1]++];
					sum[2] += m_Image.m_pBits[2][i][j] * temp[index[2]++];
				}
			}
			//将结果赋值给中心点
			m_nBits[0][m][n] = sum[0];
			m_nBits[1][m][n] = sum[1];
			m_nBits[2][m][n] = sum[2];
		}
	}
	for (int m = 1; m < h - 1; m++)
	{
		for (int n = 0; n < w - 1; n++)
		{
			m_Image.m_pBits[0][m][n] = m_nBits[0][m][n];
			m_Image.m_pBits[1][m][n] = m_nBits[1][m][n];
			m_Image.m_pBits[2][m][n] = m_nBits[2][m][n];
		}
	}

	double rho = 1.0, theta = 3.14 / 180.0;
	int threshold = maxthreshold;//设置阈值为200
	float irho = 1 / rho;
	int AngleNum = (3.14 / theta+0.5);//返回四舍五入整数值。
	int RhoNum = (((w + h) * 2 + 1) / rho+0.5);
	int *accum = new int[(AngleNum + 2) * (RhoNum + 3)];
	float *tabSin = new float[AngleNum];
	float *tabCos = new float[AngleNum];
	memset(accum, 0, sizeof(accum[0]) * (AngleNum + 2) * (RhoNum + 2));
	vector<int> _sort_buf;
	float ang = 0;
	for (int n = 0; n < AngleNum; ang += theta, n++)
	{
		tabSin[n] = (float)(sin((double)ang) * irho);
		tabCos[n] = (float)(cos((double)ang) * irho);
	}

	std::vector<int> acc;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (m_Image.m_pBits[0][i][j] != 0)
			for (int n = 0; n < AngleNum; n++)
			{
				int r = (j * tabCos[n] + i * tabSin[n]+0.5);//+0.5表示返回四舍五入整数值。
				r += (RhoNum - 1) / 2;
				accum[(n + 1) * (RhoNum + 2) + r + 1]++;
			}
		}
	}
	for (int r = 0; r < RhoNum; r++)
	{
		for (int n = 0; n < AngleNum; n++)
		{
			//大于阈值的话则保留下来
			int base = (n + 1) * (RhoNum + 2) + r + 1;
			if (accum[base] > threshold &&
				accum[base] > accum[base - 1] && accum[base] >= accum[base + 1] &&
				accum[base] > accum[base - RhoNum - 2] && accum[base] >= accum[base + RhoNum + 2])
				_sort_buf.push_back(base);
		}
		std::sort(_sort_buf.begin(), _sort_buf.end(), hough_cmp_gt(accum));
	}

	double scale = 1. / (RhoNum + 2);
	vector<Point> points;
	for (int i = 0; i < (int)_sort_buf.size(); i++)
	{
		LinePolar line;
		int idx = _sort_buf[i];
		int n = floor(idx*scale) - 1;
		int r = idx - (n + 1)*(RhoNum + 2) - 1;
		line.rho = (r - (RhoNum - 1)*0.5f) * rho;
		line.angle = n * theta;
		double a = cos(line.angle), b = sin(line.angle);
		int x0 = (int)(a*line.rho), y0 = (int)(b*line.rho);//直线必过x0、y0点
		Point pt1, pt2;
		pt1.x = (x0 + maxthreshold * (-b)+0.5);
		pt1.y = (y0 + maxthreshold * (a)+0.5);
		pt2.x = (x0 - maxthreshold * (-b)+0.5);
		pt2.y = (y0 - maxthreshold * (a)+0.5);
		double K, B;  
		K = -a / b; B = line.rho / b;
		int yy = 0;
		if (pt1.x>w || pt1.y>h || pt2.x>w || pt2.y>h)
			continue;
		for (int m = min(pt1.x,pt2.x); m < max(pt1.x,pt2.x); m++)
		{
			yy = K*m + B;//直线表达式
			if (yy < 0||K==0||m<0)
				continue;
			m_Image.m_pBits[0][yy][m] = 0;
			m_Image.m_pBits[1][yy][m] = 0;
			m_Image.m_pBits[2][yy][m] = 255;
		}	
	}
	Invalidate(1);
}

//理想低通滤波
void CImage_ProcessingView::OnIdealLowFilter()
{
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while (h_extend <= h / 2)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend <= w / 2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];//分配内存空间 
	complex<double>*pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for (int i = 0; i<h_extend*w_extend; i++)//图像补零
	{
		pTD[i] = complex<double>(0, 0);
	}
	for (int i = 0; i<h; i++)
	{
		for (int j = 0; j<w; j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//图像频谱搬移到中心 
		}
	}
	for (int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对w方向进行快速傅立叶变换 
	}
	for (int i = 0; i<h_extend; i++)//把pFD进行转置
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}
	for (int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//w方向进行快速傅立叶变换
	}
	for (int i = 0; i<h_extend; i++)//对变换结果进行转置  
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}

	CUniversalDlg UniversalDlg;
	int value;
	if (UniversalDlg.DoModal() == IDOK)
	{
		value = UniversalDlg.m_intNumber;//获取阈值
	}
	else return;
	for (int i = 0; i<h_extend; i++)
	{
		for (int j = 0; j<w_extend; j++)
		{
			BYTE temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
			if (sqrt(float(abs(i - h_extend / 2)*abs(i - h_extend / 2) + abs(j - w_extend / 2)*abs(j - w_extend / 2)))>value)
			{
				m_Image.m_pBits[0][i][j] = 0;
				m_Image.m_pBits[1][i][j] = 0;
				m_Image.m_pBits[2][i][j] = 0;
			}
			else
			{
				temp = (temp > 255) ? 255 : temp;
				m_Image.m_pBits[0][i][j] = temp;
				m_Image.m_pBits[1][i][j] = temp;
				m_Image.m_pBits[2][i][j] = temp;
			}
		}
	}
	Invalidate(1);
}

////理想高通滤波
void CImage_ProcessingView::OnIdealHighFilter()
{
if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;//图像进行扩展
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while (h_extend <= h / 2)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend <= w / 2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	complex<double>*pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for (int i = 0; i<h_extend*w_extend; i++)//图像补零
	{
		pTD[i] = complex<double>(0, 0);
	}
	for (int i = 0; i<h; i++)//
	{
		for (int j = 0; j<w; j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//频谱搬移到中心 
		}
	}
	for (int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对w方向进行快速傅立叶变换 
	}
	for (int i = 0; i<h_extend; i++)//pFD转置
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}
	for (int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对w方向进行快速傅立叶变换
	}
	for (int i = 0; i<h_extend; i++)//对结果进行转置  
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}
	CUniversalDlg UniversalDlg;
	int value;
	if (UniversalDlg.DoModal() == IDOK)
	{
		value = UniversalDlg.m_intNumber;//获取阈值
	}
	else return;
	for (int i = 0; i<h_extend; i++)
	{
		for (int j = 0; j<w_extend; j++)
		{
			BYTE temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
			if (sqrt(float(abs(i - h_extend / 2)*abs(i - h_extend / 2) + abs(j - w_extend / 2)*abs(j - w_extend / 2)))<value)
			{
				m_Image.m_pBits[0][i][j] = 0;
				m_Image.m_pBits[1][i][j] = 0;
				m_Image.m_pBits[2][i][j] = 0;
			}
			else
			{
				temp = (temp > 255) ? 255 : temp;
				m_Image.m_pBits[0][i][j] = temp;
				m_Image.m_pBits[1][i][j] = temp;
				m_Image.m_pBits[2][i][j] = temp;
			}
		}
	}
	Invalidate(1);
}

////巴特沃斯低通滤波
void CImage_ProcessingView::OnButterworthLowFilter()
{
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;//图像扩展
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while (h_extend <= h / 2)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend <= w / 2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	complex<double>*pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for (int i = 0; i<h_extend*w_extend; i++)//图像补零
	{
		pTD[i] = complex<double>(0, 0);
	}
	for (int i = 0; i<h; i++)//图像的值传给pTD
	{
		for (int j = 0; j<w; j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//频谱搬移到中心 
		}
	}
	for (int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对w方向进行快速傅立叶变换 
	}
	for (int i = 0; i<h_extend; i++)//pFD进行转置
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}
	for (int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对w方向进行快速傅立叶变换
	}
	for (int i = 0; i<h_extend; i++)//对结果进行转置  
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}
	CUniversalDlg UniversalDlg;
	int value;
	if (UniversalDlg.DoModal() == IDOK)
	{
		value = UniversalDlg.m_intNumber;//获取阈值D0
	}
	else return;
	CUniversalDlg UniversalDlg1;
	int value1;
	if (UniversalDlg1.DoModal() == IDOK)
	{
		value1 = UniversalDlg.m_intNumber;//获取巴特沃斯阶数
	}
	else return;
	
	for (int i = 0; i<h_extend; i++)
	{
		for (int j = 0; j<w_extend; j++)
		{
			double temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
			double temp1 = sqrt(float(abs(i - h_extend / 2)*abs(i - h_extend / 2) + abs(j - w_extend / 2)*abs(j - w_extend / 2))) / value;
			temp1 = 1 / (1 + pow(temp1, 2 * value1));
			temp1 = temp1*temp;
			temp1 = (temp1 > 255) ? 255 : temp1;
			m_Image.m_pBits[0][i][j] = temp1;
			m_Image.m_pBits[1][i][j] = temp1;
			m_Image.m_pBits[2][i][j] = temp1;
		}
	}
	Invalidate(1);
}

////巴特沃斯高通滤波
void CImage_ProcessingView::OnIdButterworthHighFilter()
{
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;//图像进行扩展
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while (h_extend <= h / 2)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend <= w / 2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	complex<double>*pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for (int i = 0; i<h_extend*w_extend; i++)//图像补零
	{
		pTD[i] = complex<double>(0, 0);
	}
	for (int i = 0; i<h; i++)//图像的值传给pTD
	{
		for (int j = 0; j<w; j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//频谱搬移到中心 
		}
	}
	for (int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对w方向进行快速傅立叶变换 
	}
	for (int i = 0; i<h_extend; i++)//把pFD进行转置
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}
	for (int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对w方向进行快速傅立叶变换
	}
	for (int i = 0; i<h_extend; i++)//对结果进行转置  
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}
	CUniversalDlg UniversalDlg;
	int value;
	if (UniversalDlg.DoModal() == IDOK)
	{
		value = UniversalDlg.m_intNumber;//获取阈值D0
	}
	else return;
	CUniversalDlg UniversalDlg1;
	int value1;
	if (UniversalDlg1.DoModal() == IDOK)
	{
		value1 = UniversalDlg.m_intNumber;//获取巴特沃斯阶数
	}
	else return;
	for (int i = 0; i<h_extend; i++)
	{
		for (int j = 0; j<w_extend; j++)
		{
			double temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
			double temp1 = value / sqrt((float)(abs(i - h_extend / 2)*abs(i - h_extend / 2) + abs(j - w_extend / 2)*abs(j - w_extend / 2)));
			temp1 = 1 / (1 + pow(temp1, 2 * value1));
			temp1 = temp1*temp;
			temp1 = (temp1 > 255) ? 255 : temp1;
			m_Image.m_pBits[0][i][j] = temp1;
			m_Image.m_pBits[1][i][j] = temp1;
			m_Image.m_pBits[2][i][j] = temp1;
		}
	}
	Invalidate(1);
}

////高斯低通滤波
void CImage_ProcessingView::OnGaussianLowFilter()
{
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;//图像进行扩展
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while (h_extend <= h / 2)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend <= w / 2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	complex<double>*pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for (int i = 0; i<h_extend*w_extend; i++)//图像补零
	{
		pTD[i] = complex<double>(0, 0);
	}
	for (int i = 0; i<h; i++)//把图像的值传给pTD
	{
		for (int j = 0; j<w; j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//频谱搬移到中心 
		}
	}
	for (int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对w方向进行快速傅立叶变换 
	}
	for (int i = 0; i<h_extend; i++)//pFD转置
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}
	for (int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对w方向进行快速傅立叶变换
	}
	for (int i = 0; i<h_extend; i++)//变换结果进行转置 
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}
	CUniversalDlg UniversalDlg;
	int value;
	if (UniversalDlg.DoModal() == IDOK)
	{
		value = UniversalDlg.m_intNumber;//获取阈值
	}
	else return;
	for (int i = 0; i<h_extend; i++)
	{
		for (int j = 0; j<w_extend; j++)
		{
			double temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
			double temp1 = 1 / exp(float(abs(i - h_extend / 2)*abs(i - h_extend / 2) + abs(j - w_extend / 2)*abs(j - w_extend / 2))/(2*value*value));
			temp1 = temp1*temp;
			temp1 = (temp1 > 255) ? 255 : temp1;
			m_Image.m_pBits[0][i][j] = temp1;
			m_Image.m_pBits[1][i][j] = temp1;
			m_Image.m_pBits[2][i][j] = temp1;
		}
	}
	Invalidate(1);
}

////高斯高通滤波
void CImage_ProcessingView::OnGaussianHighFilter()
{
	if (m_Image.IsNull())
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	ImgGray();
	int h_extend = 1;//图像进行扩展
	int w_extend = 1;
	int h_index = 0;
	int w_index = 0;
	while (h_extend <= h / 2)
	{
		h_extend *= 2;
		h_index++;
	}
	while (w_extend <= w / 2)
	{
		w_extend *= 2;
		w_index++;
	}
	complex<double>*pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	complex<double>*pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend];
	for (int i = 0; i<h_extend*w_extend; i++)//图像补零
	{
		pTD[i] = complex<double>(0, 0);
	}
	for (int i = 0; i<h; i++)//图像的值传给pTD
	{
		for (int j = 0; j<w; j++)
		{
			pTD[i*w_extend + j] = m_Image.m_pBits[0][i][j] * (pow(-1.0, (i + j)));//频谱搬移到中心 
		}
	}
	for (int i = 0; i<h_extend; i++)
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对w方向进行快速傅立叶变换 
	}
	for (int i = 0; i<h_extend; i++)//
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}
	for (int i = 0; i<w_extend; i++)
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对w方向进行快速傅立叶变换
	}
	for (int i = 0; i<h_extend; i++)//对变换结果进行转置 
	{
		for (int j = 0; j<w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}
	CUniversalDlg UniversalDlg;
	int value;
	if (UniversalDlg.DoModal() == IDOK)
	{
		value = UniversalDlg.m_intNumber;//获取阈值
	}
	else return;
	for (int i = 0; i<h_extend; i++)
	{
		for (int j = 0; j<w_extend; j++)
		{
			double temp = sqrt(pTD[i*w_extend + j].real()*pTD[i*w_extend + j].real() + pTD[i*w_extend + j].imag()*pTD[i*w_extend + j].imag()) / 100;
			double temp1 = 1 / exp(float(abs(i - h_extend / 2)*abs(i - h_extend / 2) + abs(j - w_extend / 2)*abs(j - w_extend / 2)) / (2 * value*value));
			temp1 = 1 - temp1;
			temp1 = temp1*temp;
			temp1 = (temp1 > 255) ? 255 : temp1;
			m_Image.m_pBits[0][i][j] = temp1;
			m_Image.m_pBits[1][i][j] = temp1;
			m_Image.m_pBits[2][i][j] = temp1;
		}
	}
	Invalidate(1);
}


void CImage_ProcessingView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetCursorPos(&point);
	ScreenToClient(&point);
	m_ptStart = point;
	CScrollView::OnLButtonDown(nFlags, point);
	Invalidate(1);//刷新界面，否则会留下多个框框
}


void CImage_ProcessingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetCursorPos(&point);
	ScreenToClient(&point);//完成Screen（屏幕坐标)到 Client（客户区坐标）的转换。
	m_ptEnd = point;
	//绘制黑色边框
	CClientDC dc(this);
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = dc.SelectObject(pBrush);
	dc.Rectangle(CRect(m_ptStart, point));
	dc.SelectObject(pOldBrush);
	drawRGB_RectFinish = true;
	CScrollView::OnLButtonUp(nFlags, point);
}

//彩色图像分割
void CImage_ProcessingView::OnRgbImgSegment()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) 
	{
		return;
	}
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	int countB = 0, countG = 0, countR = 0;
	int avgB = 0, avgG = 0, avgR = 0;
	if (drawRGB_RectFinish)
	{
		int a = m_ptEnd.y - m_ptStart.y, b = m_ptEnd.x - m_ptStart.x;
		int size = a*b;

		for (int j = m_ptStart.y; j < m_ptEnd.y; j++)
		{
			for (int i = m_ptStart.x; i < m_ptEnd.x; i++)
			{
				countB = countB + m_Image.m_pBits[0][j][i];
				countG = countG + m_Image.m_pBits[1][j][i];
				countR = countR + m_Image.m_pBits[2][j][i];
			}
		}
		avgB = countB / size;
		avgG = countG / size;
		avgR = countR / size;
		int d1 = 50;//设置阈值
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				int a = (m_Image.m_pBits[0][j][i] - avgB)*(m_Image.m_pBits[0][j][i] - avgB);
				int b = (m_Image.m_pBits[1][j][i] - avgG)*(m_Image.m_pBits[1][j][i] - avgG);
				int c = (m_Image.m_pBits[2][j][i] - avgR)*(m_Image.m_pBits[2][j][i] - avgR);
				int d2 = sqrt(double(a + b + c));
				if (d2 <= d1){
					m_Image.m_pBits[0][j][i] = avgB;
					m_Image.m_pBits[1][j][i] = avgG;
					m_Image.m_pBits[2][j][i] = avgR;
				}
				else{
					m_Image.m_pBits[0][j][i] = 0;
					m_Image.m_pBits[1][j][i] = 0;
					m_Image.m_pBits[2][j][i] = 0;
				}
			}
		}
	}
	else
	{
		MessageBox(_T("请先选择一个区域！"));
		return ;
	}
	drawRGB_RectFinish = false;
	Invalidate(1);
}
