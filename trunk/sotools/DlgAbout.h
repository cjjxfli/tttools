#pragma once


// CDlgAbout �Ի���

class CDlgAbout : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAbout)

public:
	CDlgAbout(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAbout();

// �Ի�������
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	CBrush m_brush;
	CFont m_font;
	CRect m_pRectLink;

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStaticAuthorWeb();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
