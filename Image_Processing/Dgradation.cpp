// Dgradation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "Dgradation.h"
#include "afxdialogex.h"


// CDgradation �Ի���

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


// CDgradation ��Ϣ�������
