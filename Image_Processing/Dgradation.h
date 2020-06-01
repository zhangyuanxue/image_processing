#pragma once


// CDgradation 对话框

class CDgradation : public CDialogEx
{
	DECLARE_DYNAMIC(CDgradation)

public:
	CDgradation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDgradation();

// 对话框数据
	enum { IDD = IDD_DEGRADATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_degradationValue;
};
