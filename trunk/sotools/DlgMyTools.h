#pragma once
#include "afxwin.h"


// DlgMyTools 对话框
#define BYTE_SHORT_TYPE 0
#define BYTE_INT_TYPE 1

class DlgMyTools : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMyTools)

public:
	DlgMyTools(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgMyTools();

// 对话框数据
	enum { IDD = IDD_DIALOG_MYTOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	BOOL CheckUShortNumber(CString cstrNum,unsigned short & out_number);
	BOOL CheckUIntNumber(CString cstrNum, unsigned int & out_number);
	BOOL CheckULongNumber(CString cstrNum, unsigned long & out_number);
private:
	int m_byte_type;
	CFont * m_pfont;
public:
	afx_msg void OnBnClickedByteType();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
private:
	// 网络字节序值
	CString m_netorder_number;
	// 本地字节序值
	CString m_localorder_number;
	// 数值型ip
	CString m_number_ip;
	// 字符串型ip
	CString m_str_ip;
public:
	afx_msg void OnBnClickedButtonNet2host();
	afx_msg void OnBnClickedButtonHost2net();
	afx_msg void OnBnClickedButtonIpNum2dot();
	afx_msg void OnBnClickedButtonIpDot2num();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
