#pragma once
#include "afxwin.h"


// CGrayScale �Ի���

class CGrayScale : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayScale)

public:
	CGrayScale(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGrayScale();

// �Ի�������
	enum { IDD = IDD_GRAY_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_grayScale;
};
