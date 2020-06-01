#pragma once


// CImgOverlay 对话框

class CImgOverlay : public CDialogEx
{
	DECLARE_DYNAMIC(CImgOverlay)

public:
	CImgOverlay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImgOverlay();

// 对话框数据
	enum { IDD = IDD_IMG_OVERLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_imgOverlayNum;
};
