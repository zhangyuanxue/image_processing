// HistogramImg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "HistogramImg.h"
#include "afxdialogex.h"
//#include "Image_ProcessingView.h"

// CHistogramImg �Ի���

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


// CHistogramImg ��Ϣ�������


void CHistogramImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CPen *pPenRed = new CPen(); 
	pPenRed ->CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); 
	CPen *pPenGreen = new CPen();
	pPenGreen ->CreatePen(PS_SOLID,1,RGB(0,255,0));
	dc.SelectObject(pPenRed);

	//����������
	//ˮƽ�ᣬ�Ҷȣ���ֱ�ᣬ���ʣ�
	dc.MoveTo(30,40);
	dc.LineTo(30,360);//��y��
	dc.LineTo(650,360);//��x��

	dc.MoveTo(25,45);//��y���ͷ
	dc.LineTo(30,40);
	dc.MoveTo(35,45);
	dc.LineTo(30,40);

	dc.MoveTo(645,355);//��x���ͷ
	dc.LineTo(650,360);
	dc.MoveTo(645,365);
	dc.LineTo(650,360);

	for(int i=0;i<10;i++)//��y��̶���(360-40-20)/10=30
	{
		dc.MoveTo(30,60+30*i);
		dc.LineTo(35,60+30*i);
	}

	for(int i=1;i<17;i++)//��x��̶���
	{
		dc.MoveTo(30+36*i,360);
		dc.LineTo(30+36*i,355);
	}

	dc.TextOutW(38,30,L"���ʹ���ֵ(P*1000)");
	dc.TextOutW(7,362,L"0");
	dc.TextOutW(5,322,L"30");
	dc.TextOutW(5,292,L"60");
	dc.TextOutW(5,262,L"90");
	dc.TextOutW(0,232,L"120");
	dc.TextOutW(0,202,L"150");
	dc.TextOutW(0,172,L"180");
	dc.TextOutW(0,142,L"210");
	dc.SelectObject(pPenGreen);
	for(int i=0;i<256;i++)
	{
		dc.MoveTo(30+i*576/255,358);
		dc.LineTo(30+i*576/255,358-pGraScaleRatio[i]*10000);
	}
	delete pPenGreen;
	delete pPenRed;
}
