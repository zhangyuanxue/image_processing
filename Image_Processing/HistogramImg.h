#pragma once


// CHistogramImg �Ի���

class CHistogramImg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramImg)

public:
	CHistogramImg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistogramImg();

// �Ի�������
	enum { IDD = IDD_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	float *pGraScaleRatio;
};
