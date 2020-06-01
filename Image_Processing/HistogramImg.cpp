// HistogramImg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "HistogramImg.h"
#include "afxdialogex.h"
//#include "Image_ProcessingView.h"

// CHistogramImg 对话框

IMPLEMENT_DYNAMIC(CHistogramImg, CDialogEx)

CHistogramImg::CHistogramImg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistogramImg::IDD, pParent)
{

}

CHistogramImg::~CHistogramImg()
{
}

void CHistogramImg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistogramImg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistogramImg 消息处理程序


void CHistogramImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CPen *pPenRed = new CPen(); 
	pPenRed ->CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); 
	CPen *pPenGreen = new CPen();
	pPenGreen ->CreatePen(PS_SOLID,1,RGB(0,255,0));
	dc.SelectObject(pPenRed);

	dc.MoveTo(30, 40);
	dc.LineTo(30, 360);//画y轴
	dc.LineTo(650, 360);//画x轴

	dc.MoveTo(25, 45);//画y轴箭头
	dc.LineTo(30, 40);
	dc.MoveTo(35, 45);
	dc.LineTo(30, 40);

	dc.MoveTo(645, 355);//画x轴箭头
	dc.LineTo(650, 360);
	dc.MoveTo(645, 365);
	dc.LineTo(650, 360);

	for (int i = 0; i<10; i++)//画y轴刻度线(360-40-20)/10=30
	{
		dc.MoveTo(30, 60 + 30 * i);
		dc.LineTo(35, 60 + 30 * i);
	}

	for (int i = 1; i<17; i++)//画x轴刻度线
	{
		dc.MoveTo(30 + 36 * i, 360);
		dc.LineTo(30 + 36 * i, 355);
	}
	dc.TextOutW(38, 30, L"概率估计值(P*1000)");//画y轴标度数字
	dc.TextOutW(10, 361, L"0");
	dc.TextOutW(5, 322, L"30");
	dc.TextOutW(5, 292, L"60");
	dc.TextOutW(5, 262, L"90");
	dc.TextOutW(0, 232, L"120");
	dc.TextOutW(0, 202, L"150");
	dc.TextOutW(0, 172, L"180");
	dc.TextOutW(0, 142, L"210");
	dc.TextOutW(0, 112, L"240");
	dc.TextOutW(0, 82, L"270");
	dc.TextOutW(0, 52, L"300");

	dc.TextOutW(66-7, 364, L"15");//输出x轴标度数字
	dc.TextOutW(102-7, 364, L"31");
	dc.TextOutW(138 - 7, 364, L"47");
	dc.TextOutW(174 - 7, 364, L"63");
	dc.TextOutW(210 - 7, 364, L"79");
	dc.TextOutW(246 - 7, 364, L"95");
	dc.TextOutW(282 - 10, 364, L"111");
	dc.TextOutW(318 - 10, 364, L"127");
	dc.TextOutW(354 - 10, 364, L"143");
	dc.TextOutW(390 - 10, 364, L"159");
	dc.TextOutW(426 - 10, 364, L"175");
	dc.TextOutW(462 - 10, 364, L"191");
	dc.TextOutW(498 - 10, 364, L"207");
	dc.TextOutW(534 - 10, 364, L"223");
	dc.TextOutW(570 - 10, 364, L"239");
	dc.TextOutW(606 - 10, 364, L"255");
	
	dc.SelectObject(pPenGreen);
	for (int i = 0; i<256; i++)
	{
		dc.MoveTo(30 + i * 576 / 255, 358);
		dc.LineTo(30 + i * 576 / 255, 358 - pGraScaleRatio[i] * 10000);
	}
	delete pPenGreen;
	delete pPenRed;
}
