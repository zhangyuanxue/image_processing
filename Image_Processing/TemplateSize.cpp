// TemplateSize.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "TemplateSize.h"
#include "afxdialogex.h"


// CTemplateSize �Ի���

IMPLEMENT_DYNAMIC(CTemplateSize, CDialogEx)

CTemplateSize::CTemplateSize(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTemplateSize::IDD, pParent)
	, m_templateSize(0)
{

}

CTemplateSize::~CTemplateSize()
{
}

void CTemplateSize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TemplateSize, m_templateSize);
}


BEGIN_MESSAGE_MAP(CTemplateSize, CDialogEx)
END_MESSAGE_MAP()


// CTemplateSize ��Ϣ�������
