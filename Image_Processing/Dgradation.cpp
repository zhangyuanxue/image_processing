// Dgradation.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Dgradation.h"
#include "afxdialogex.h"


// CDgradation 对话框

IMPLEMENT_DYNAMIC(CDgradation, CDialogEx)

CDgradation::CDgradation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDgradation::IDD, pParent)
	, m_degradationValue(0)
{

}

CDgradation::~CDgradation()
{
}

void CDgradation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DEGRADATION_VALUE, m_degradationValue);
}


BEGIN_MESSAGE_MAP(CDgradation, CDialogEx)
END_MESSAGE_MAP()


// CDgradation 消息处理程序
