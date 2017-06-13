#pragma once


// CDlgRTab 对话框

class CDlgRTab : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRTab)

public:
	CDlgRTab(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgRTab();

// 对话框数据
	enum { IDD = IDD_SOTOOLS_DIALOG };

private:
	void resize();

private:
	POINT m_tab_size;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
