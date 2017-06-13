#pragma once
#include "afxwin.h"


// DlgMyTools �Ի���
#define BYTE_SHORT_TYPE 0
#define BYTE_INT_TYPE 1

class DlgMyTools : public CDialogEx
{
	DECLARE_DYNAMIC(DlgMyTools)

public:
	DlgMyTools(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgMyTools();

// �Ի�������
	enum { IDD = IDD_DIALOG_MYTOOLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
	// �����ֽ���ֵ
	CString m_netorder_number;
	// �����ֽ���ֵ
	CString m_localorder_number;
	// ��ֵ��ip
	CString m_number_ip;
	// �ַ�����ip
	CString m_str_ip;
public:
	afx_msg void OnBnClickedButtonNet2host();
	afx_msg void OnBnClickedButtonHost2net();
	afx_msg void OnBnClickedButtonIpNum2dot();
	afx_msg void OnBnClickedButtonIpDot2num();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
