#pragma once


// CDgradation �Ի���

class CDgradation : public CDialogEx
{
	DECLARE_DYNAMIC(CDgradation)

public:
	CDgradation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDgradation();

// �Ի�������
	enum { IDD = IDD_DEGRADATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_degradationValue;
};
