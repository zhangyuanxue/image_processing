#pragma once
#include "afxwin.h"


// CUniversalDlg �Ի���

class CUniversalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUniversalDlg)

public:
	CUniversalDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUniversalDlg();

// �Ի�������
	enum { IDD = IDD_Universal_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	int m_intNumber;
};
