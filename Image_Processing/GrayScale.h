#pragma once
#include "afxwin.h"


// CGrayScale 对话框

class CGrayScale : public CDialogEx
{
	DECLARE_DYNAMIC(CGrayScale)

public:
	CGrayScale(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGrayScale();

// 对话框数据
	enum { IDD = IDD_GRAY_SCALE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_grayScale;
};
