#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <list>
#include <iterator>


// CDlgProtocol �Ի���

typedef struct tagCharNode
{
	//1��ʾchar��2��ʾshort��4��ʾint��8��ʾlong��16��ʾ�ַ���
	unsigned short t_type;
	unsigned char t_value;
}CharNode;
typedef struct tagShortNode
{
	//1��ʾchar��2��ʾshort��4��ʾint��8��ʾlong��16��ʾ�ַ���
	unsigned short t_type;
	unsigned short t_value;
}ShortNode;

typedef struct tagIntNode
{
	//1��ʾchar��2��ʾshort��4��ʾint��8��ʾlong��16��ʾ�ַ���
	unsigned short t_type;
	unsigned int t_value;
}IntNode;

typedef struct tagLongNode
{
	//1��ʾchar��2��ʾshort��4��ʾint��8��ʾlong��16��ʾ�ַ���
	unsigned short t_type;
	unsigned long long t_value;
}LongNode;

typedef struct tagStrNode
{
	//1��ʾchar��2��ʾshort��4��ʾint��8��ʾlong��16��ʾ�ַ���
	unsigned short t_type;
	CString t_value;
}StrNode;

typedef struct tagListNode
{
	//1��ʾchar��2��ʾshort��4��ʾint��8��ʾlong��16��ʾ�ַ���
	unsigned short t_type;
	union 
	{
		CharNode * t_char_node;
		ShortNode * t_short_node;
		IntNode * t_int_node;
		LongNode * t_long_node;
		StrNode * t_str_node;
	};
	tagListNode * t_next;
}ListNode;

class CDlgProtocol : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgProtocol)

public:
	CDlgProtocol(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgProtocol();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROTOCOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	
private:
	CComboBox m_key_type;
	// �ֶ�ֵ
	CString m_field_value;
	CString m_cstr_head;
	CString m_cstr_tail;
	CString m_cstr_body;
	// �ֶ�ֵ����
	CString m_field_filled;
	CRichEditCtrl m_head_protocol;
	CRichEditCtrl m_tail_protocol;
	CRichEditCtrl m_full_body_protocol;
	// 0ΪЭ��ͷ��1ΪЭ��β��2ΪЭ��body
	int m_protocol_type;
	std::list<ListNode*> m_head_list_node;
	std::list<ListNode*> m_tail_list_node;
	std::list<ListNode*> m_body_list_node;

private:
	void InsertCharNode(unsigned char t_value, std::list<ListNode*> & list_node);
	void InsertShortNode(unsigned short t_value, std::list<ListNode*> & list_node);
	void InsertIntNode(unsigned int t_value, std::list<ListNode*> & list_node);
	void InsertLongNode(unsigned long t_value, std::list<ListNode*> & list_node);
	void InsertStringNode(CString & t_value, std::list<ListNode*> & list_node);
	void ResetList(std::list<ListNode*> & list_node);
	void Import(CString filePath,CString & text, CRichEditCtrl & rich_edit, std::list<ListNode*> & list_node);

public:
	afx_msg void OnBnClickedSetProtocolType();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonImportTplHead();
	afx_msg void OnBnClickedButtonExportTplHead();
	afx_msg void OnBnClickedButtonImportTplTail();
	afx_msg void OnBnClickedButtonExportTplTail();
	afx_msg void OnBnClickedButtonAppendKvExport();
	afx_msg void OnBnClickedInsertKV();
	afx_msg void OnBnClickedButtonResetProtocol();
};
