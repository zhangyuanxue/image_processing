#pragma once


// CHistogramImg 对话框

class CHistogramImg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramImg)

public:
	CHistogramImg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistogramImg();

// 对话框数据
	enum { IDD = IDD_HISTOGRAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	float *pGraScaleRatio;
};
