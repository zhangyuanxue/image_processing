// ImgOverlay.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "ImgOverlay.h"
#include "afxdialogex.h"


// CImgOverlay �Ի���

IMPLEMENT_DYNAMIC(CImgOverlay, CDialogEx)

CImgOverlay::CImgOverlay(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgOverlay::IDD, pParent)
	, m_imgOverlayNum(0)
{

}

CImgOverlay::~CImgOverlay()
{
}

void CImgOverlay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IMG_OVERLAY_NUM, m_imgOverlayNum);
}


BEGIN_MESSAGE_MAP(CImgOverlay, CDialogEx)
END_MESSAGE_MAP()


// CImgOverlay ��Ϣ�������
