// GrayScale.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "GrayScale.h"
#include "afxdialogex.h"


// CGrayScale 对话框

IMPLEMENT_DYNAMIC(CGrayScale, CDialogEx)

CGrayScale::CGrayScale(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGrayScale::IDD, pParent)
	, m_grayScale(0)
{

}

CGrayScale::~CGrayScale()
{
}

void CGrayScale::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GRAY_SCALE, m_grayScale);
}


BEGIN_MESSAGE_MAP(CGrayScale, CDialogEx)
END_MESSAGE_MAP()


// CGrayScale 消息处理程序
