#pragma once
#include "afxwin.h"


// CUniversalDlg 对话框

class CUniversalDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUniversalDlg)

public:
	CUniversalDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUniversalDlg();

// 对话框数据
	enum { IDD = IDD_Universal_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	int m_intNumber;
};
