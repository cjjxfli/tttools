#pragma once


// CDlgRTab �Ի���

class CDlgRTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRTab)

public:
	CDlgRTab(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRTab();

// �Ի�������
	enum { IDD = IDD_SOTOOLS_DIALOG };

private:
	void resize();

private:
	POINT m_tab_size;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
