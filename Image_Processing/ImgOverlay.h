#pragma once


// CImgOverlay �Ի���

class CImgOverlay : public CDialogEx
{
	DECLARE_DYNAMIC(CImgOverlay)

public:
	CImgOverlay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImgOverlay();

// �Ի�������
	enum { IDD = IDD_IMG_OVERLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_imgOverlayNum;
};
