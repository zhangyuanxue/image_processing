#pragma once
#include "afxwin.h"


// CTemplateSize �Ի���

class CTemplateSize : public CDialogEx
{
	DECLARE_DYNAMIC(CTemplateSize)

public:
	CTemplateSize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTemplateSize();

// �Ի�������
	enum { IDD = IDD_TEMPLATE_SIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	int m_templateSize;
};
