// UniversalDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "UniversalDlg.h"
#include "afxdialogex.h"


// CUniversalDlg �Ի���

IMPLEMENT_DYNAMIC(CUniversalDlg, CDialogEx)

CUniversalDlg::CUniversalDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUniversalDlg::IDD, pParent)
	, m_intNumber(0)
{

}

CUniversalDlg::~CUniversalDlg()
{
}

void CUniversalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_INT_NUMBER, m_intNumber);
}


BEGIN_MESSAGE_MAP(CUniversalDlg, CDialogEx)
END_MESSAGE_MAP()


// CUniversalDlg ��Ϣ�������
