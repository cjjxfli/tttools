// DlgProtocol.cpp : 实现文件
//

#include "stdafx.h"
#include "sotools.h"
#include "DlgProtocol.h"
#include "afxdialogex.h"
#include "yg_spp_utils.h"
#include "../sodeps/utils.h"


// CDlgProtocol 对话框

IMPLEMENT_DYNAMIC(CDlgProtocol, CDialogEx)

CDlgProtocol::CDlgProtocol(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgProtocol::IDD, pParent)
	, m_protocol_type(0)
	, m_field_value(_T(""))
	, m_field_filled(_T(""))
{

}

CDlgProtocol::~CDlgProtocol()
{
}

void CDlgProtocol::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_KEY_TYPE, m_key_type);
	DDX_Control(pDX, IDC_RICHEDIT_PROTOCOL_HEAD, m_head_protocol);
	DDX_Control(pDX, IDC_RICHEDIT_PROTOCOL_TAIL, m_tail_protocol);
	DDX_Control(pDX, IDC_RICHEDIT_FULL_BODY, m_full_body_protocol);
	DDX_Radio(pDX, IDC_RADIO_HEAD_TYPE, m_protocol_type);
	DDX_Text(pDX, IDC_EDIT1, m_field_value);
	DDX_Text(pDX, IDC_EDIT2, m_field_filled);
}


BEGIN_MESSAGE_MAP(CDlgProtocol, CDialogEx)
	//ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_HEAD_TYPE, &CDlgProtocol::OnBnClickedSetProtocolType)
	ON_BN_CLICKED(IDC_RADIO_TAIL_TYPE, &CDlgProtocol::OnBnClickedSetProtocolType)
	ON_BN_CLICKED(IDC_RADIO_BODY_TYPE, &CDlgProtocol::OnBnClickedSetProtocolType)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_TPL_HEAD, &CDlgProtocol::OnBnClickedButtonImportTplHead)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_TPL_HEAD, &CDlgProtocol::OnBnClickedButtonExportTplHead)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_TPL_TAIL, &CDlgProtocol::OnBnClickedButtonImportTplTail)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_TPL_TAIL, &CDlgProtocol::OnBnClickedButtonExportTplTail)
	ON_BN_CLICKED(IDC_BUTTON_APPEND_KV_EXPORT, &CDlgProtocol::OnBnClickedButtonAppendKvExport)
	//ON_BN_CLICKED(IDC_BUTTON_APPEND_KV, &CDlgProtocol::OnBnClickedButtonAppendKV)
	ON_BN_CLICKED(IDC_INSERT_KV, &CDlgProtocol::OnBnClickedInsertKV)
	ON_BN_CLICKED(IDC_BUTTON_RESET_PROTOCOL, &CDlgProtocol::OnBnClickedButtonResetProtocol)
END_MESSAGE_MAP()


// CDlgProtocol 消息处理程序
HBRUSH CDlgProtocol::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}

BOOL CDlgProtocol::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	m_key_type.AddString("1字节char");
	m_key_type.AddString("2字节short");
	m_key_type.AddString("4字节int");
	m_key_type.AddString("8字节long");
	m_key_type.AddString("变长字节string");
	m_key_type.SetCurSel(0);

	m_protocol_type = 0;

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}


void CDlgProtocol::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
}

//设置协议
void CDlgProtocol::OnBnClickedSetProtocolType()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

//导入协议头head模板
void CDlgProtocol::OnBnClickedButtonImportTplHead()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = ".";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "协议头模板 (*.zh.h.tpl)|*.zh.h.tpl||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.zh.tpl";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
	else
	{
		return;
	}
	ResetList(m_head_list_node);
	Import(filePath, m_cstr_head, m_head_protocol, m_head_list_node);
	
	UpdateData(FALSE);
}

//导出协议头模板
void CDlgProtocol::OnBnClickedButtonExportTplHead()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = FALSE;     //是否打开(否则为保存)  
	CString defaultDir = ".";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "协议头模板 (*.zh.h.tpl)|*.zh.h.tpl||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.zh.h.tpl";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
	else
	{
		return;
	}
	FILE * in_file = fopen(filePath.GetBuffer(), "wb");
	if (NULL == in_file)
	{
		CString msg = "打开文件失败！";
		msg += filePath;
		MessageBox(msg);
		return;
	}
	
	unsigned int len = 1024 * 1024 * 10;
	char * bin_buffer = new char[len];
	char type_mark[512];
	//memset(bin_buffer, 0, len);
	char * p_mark = type_mark;
	char * pTail= bin_buffer;
	unsigned int bin_len = 0;
	unsigned int mark_len = 0;
	std::list<ListNode*>::iterator it = m_head_list_node.begin();
	for (; it != m_head_list_node.end(); it++)
	{
		unsigned short type = (*it)->t_type;
		if (1 == type)
		{
			*pTail = (*it)->t_char_node->t_value;
			pTail++;
			*p_mark = '1';
			p_mark++;
			bin_len++;
			mark_len++;
		}
		else if (2 == type)
		{
			unsigned short *p = (unsigned short *)pTail;
			*p = (*it)->t_short_node->t_value;
			pTail += 2;
			*p_mark = '2';
			p_mark++;
			bin_len += 2;
			mark_len++;
		}
		else if (4 == type)
		{
			unsigned int * p = (unsigned int *)pTail;
			*p = (*it)->t_int_node->t_value;
			pTail += 4;
			*p_mark = '4';
			p_mark++;
			bin_len += 4;
			mark_len++;
		}
		else if (8 == type)
		{
			memcpy(pTail, &((*it)->t_long_node->t_value), sizeof(long long));
			pTail += sizeof(long long);
			*p_mark = '8';
			p_mark++;
			bin_len += sizeof(long long);
			mark_len++;
		}
		else if (16 == type)
		{
			memcpy(pTail, (*it)->t_str_node->t_value.GetBuffer(), (*it)->t_str_node->t_value.GetLength());
			pTail += (*it)->t_str_node->t_value.GetLength();
			unsigned int dex = 0x10;
			*p_mark = dex;
			p_mark += 2;
			bin_len += (*it)->t_str_node->t_value.GetLength();
			mark_len += 2;
		}
	}
	pTail = '\0';
	p_mark = '\0';
 	char dump_buffer[1024 * 10] = { 0 };
 	int dump_len = 1024 * 10;
	dump_len = souitls::dump_hex2(bin_buffer, bin_len, dump_buffer, dump_len);
	fwrite(type_mark, 1, mark_len,in_file);
	fwrite("\n",1,1,in_file);
	fwrite(bin_buffer, 1, bin_len, in_file);
	fclose(in_file);
}

//导入协议尾模板
void CDlgProtocol::OnBnClickedButtonImportTplTail()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = ".";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "协议尾模板 (*.zh.t.tpl)|*.zh.t.tpl||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.zh.t.tpl";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
	else
	{
		return;
	}
	ResetList(m_tail_list_node);
	Import(filePath, m_cstr_tail, m_tail_protocol, m_tail_list_node);
	UpdateData(FALSE);
}

//导出协议尾模板
void CDlgProtocol::OnBnClickedButtonExportTplTail()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = FALSE;     //是否打开(否则为保存)  
	CString defaultDir = ".";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "协议尾模板 (*.zh.t.tpl)|*.zh.t.tpl||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.zh.t.tpl";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
	else
	{
		return;
	}
	
	FILE * in_file = fopen(filePath.GetBuffer(), "wb");
	if (NULL == in_file)
	{
		CString msg = "打开文件失败！";
		msg += filePath;
		MessageBox(msg);
		return;
	}

	unsigned int len = 1024 * 1024 * 10;
	char * bin_buffer = new char[len];
	char type_mark[512];
	//memset(bin_buffer, 0, len);
	char * p_mark = type_mark;
	char * pTail = bin_buffer;
	unsigned int bin_len = 0;
	unsigned int mark_len = 0;
	std::list<ListNode*>::iterator it = m_tail_list_node.begin();
	for (; it != m_tail_list_node.end(); it++)
	{
		unsigned short type = (*it)->t_type;
		if (1 == type)
		{
			*pTail = (*it)->t_char_node->t_value;
			pTail++;
			*p_mark = '1';
			p_mark++;
			bin_len++;
			mark_len++;
		}
		else if (2 == type)
		{
			unsigned short *p = (unsigned short *)pTail;
			*p = (*it)->t_short_node->t_value;
			pTail += 2;
			*p_mark = '2';
			p_mark++;
			bin_len += 2;
			mark_len++;
		}
		else if (4 == type)
		{
			unsigned int * p = (unsigned int *)pTail;
			*p = (*it)->t_int_node->t_value;
			pTail += 4;
			*p_mark = '4';
			p_mark++;
			bin_len += 4;
			mark_len++;
		}
		else if (8 == type)
		{
			memcpy(pTail, &((*it)->t_long_node->t_value), sizeof(long long));
			pTail += sizeof(long long);
			*p_mark = '8';
			p_mark++;
			bin_len += sizeof(long long);
			mark_len++;
		}
		else if (16 == type)
		{
			memcpy(pTail, (*it)->t_str_node->t_value.GetBuffer(), (*it)->t_str_node->t_value.GetLength());
			pTail += (*it)->t_str_node->t_value.GetLength();
			unsigned int dex = 0x10;
			*p_mark = dex;
			p_mark += 2;
			bin_len += (*it)->t_str_node->t_value.GetLength();
			mark_len += 2;
		}
	}
	pTail = '\0';
	p_mark = '\0';
	char dump_buffer[1024 * 10] = { 0 };
	int dump_len = 1024 * 10;
	dump_len = souitls::dump_hex2(bin_buffer, bin_len, dump_buffer, dump_len);
	fwrite(type_mark, 1, mark_len, in_file);
	fwrite("\n", 1, 1, in_file);
	fwrite(bin_buffer, 1, bin_len, in_file);
	fclose(in_file);
}

//导出协议
void CDlgProtocol::OnBnClickedButtonAppendKvExport()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = FALSE;     //是否打开(否则为保存)  
	CString defaultDir = ".";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "协议体 (*.zh.bin)|*.zh.bin||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY | OFN_OVERWRITEPROMPT, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.zh.bin";
	if (result == IDOK)
	{
		filePath = openFileDlg.GetPathName();
	}
	else
	{
		return;
	}
	FILE * in_file = fopen(filePath.GetBuffer(), "wb");
	if (NULL == in_file)
	{
		CString msg = "打开文件失败！";
		msg += filePath;
		MessageBox(msg);
		return;
	}

	unsigned int len = 1024 * 1024 * 10;
	char * bin_buffer = new char[len];
	char type_mark[512];
	//memset(bin_buffer, 0, len);
	char * p_mark = type_mark;
	char * pTail = bin_buffer;
	unsigned int bin_len = 0;
	unsigned int mark_len = 0;
	//head
	std::list<ListNode*>::iterator it = m_head_list_node.begin();
	for (; it != m_head_list_node.end(); it++)
	{
		unsigned short type = (*it)->t_type;
		if (1 == type)
		{
			*pTail = (*it)->t_char_node->t_value;
			pTail++;
			*p_mark = '1';
			p_mark++;
			bin_len++;
			mark_len++;
		}
		else if (2 == type)
		{
			unsigned short *p = (unsigned short *)pTail;
			*p = (*it)->t_short_node->t_value;
			pTail += 2;
			*p_mark = '2';
			p_mark++;
			bin_len += 2;
			mark_len++;
		}
		else if (4 == type)
		{
			unsigned int * p = (unsigned int *)pTail;
			*p = (*it)->t_int_node->t_value;
			pTail += 4;
			*p_mark = '4';
			p_mark++;
			bin_len += 4;
			mark_len++;
		}
		else if (8 == type)
		{
			memcpy(pTail, &((*it)->t_long_node->t_value), sizeof(long long));
			pTail += sizeof(long long);
			*p_mark = '8';
			p_mark++;
			bin_len += sizeof(long long);
			mark_len++;
		}
		else if (16 == type)
		{
			memcpy(pTail, (*it)->t_str_node->t_value.GetBuffer(), (*it)->t_str_node->t_value.GetLength());
			pTail += (*it)->t_str_node->t_value.GetLength();
			unsigned int dex = 0x10;
			*p_mark = dex;
			p_mark += 2;
			bin_len += (*it)->t_str_node->t_value.GetLength();
			mark_len += 2;
		}
	}
	pTail = '\0';
	p_mark = '\0';
	char dump_buffer[1024 * 10] = { 0 };
	int dump_len = 1024 * 10;
	dump_len = souitls::dump_hex2(bin_buffer, bin_len, dump_buffer, dump_len);
// 	fwrite(type_mark, 1, mark_len, in_file);
// 	fwrite("\n", 1, 1, in_file);
	fwrite(bin_buffer, 1, bin_len, in_file);

	//body
	bin_len = 0;
	pTail = bin_buffer;
	it = m_body_list_node.begin();
	for (; it != m_body_list_node.end(); it++)
	{
		unsigned short type = (*it)->t_type;
		if (1 == type)
		{
			*pTail = (*it)->t_char_node->t_value;
			pTail++;
			bin_len++;
		}
		else if (2 == type)
		{
			unsigned short *p = (unsigned short *)pTail;
			*p = (*it)->t_short_node->t_value;
			pTail += 2;
			bin_len += 2;
		}
		else if (4 == type)
		{
			unsigned int * p = (unsigned int *)pTail;
			*p = (*it)->t_int_node->t_value;
			pTail += 4;
			bin_len += 4;
		}
		else if (8 == type)
		{
			memcpy(pTail, &((*it)->t_long_node->t_value), sizeof(long long));
			pTail += sizeof(long long);
			bin_len += sizeof(long long);
		}
		else if (16 == type)
		{
			memcpy(pTail, (*it)->t_str_node->t_value.GetBuffer(), (*it)->t_str_node->t_value.GetLength());
			pTail += (*it)->t_str_node->t_value.GetLength();
			bin_len += (*it)->t_str_node->t_value.GetLength();
		}
	}
	pTail = '\0';
	fwrite(bin_buffer, 1, bin_len, in_file);


	//tail
	bin_len = 0;
	pTail = bin_buffer;
	it = m_tail_list_node.begin();
	for (; it != m_tail_list_node.end(); it++)
	{
		unsigned short type = (*it)->t_type;
		if (1 == type)
		{
			*pTail = (*it)->t_char_node->t_value;
			pTail++;
			bin_len++;
		}
		else if (2 == type)
		{
			unsigned short *p = (unsigned short *)pTail;
			*p = (*it)->t_short_node->t_value;
			pTail += 2;
			bin_len += 2;
		}
		else if (4 == type)
		{
			unsigned int * p = (unsigned int *)pTail;
			*p = (*it)->t_int_node->t_value;
			pTail += 4;
			bin_len += 4;
		}
		else if (8 == type)
		{
			memcpy(pTail, &((*it)->t_long_node->t_value), sizeof(long long));
			pTail += sizeof(long long);
			bin_len += sizeof(long long);
		}
		else if (16 == type)
		{
			memcpy(pTail, (*it)->t_str_node->t_value.GetBuffer(), (*it)->t_str_node->t_value.GetLength());
			pTail += (*it)->t_str_node->t_value.GetLength();
			bin_len += (*it)->t_str_node->t_value.GetLength();
		}
	}
	pTail = '\0';
	fwrite(bin_buffer, 1, bin_len, in_file);

	fclose(in_file);
}

//添加一个字段
void CDlgProtocol::OnBnClickedInsertKV()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	char * ch_buffer = new char[1024 * 1024];
	unsigned int buf_len = 1024 * 1024;
	int selected = m_key_type.GetCurSel();
	int value_len = 0;
	CString cstrValue = m_field_value;

	//CString tmp;
	if (cstrValue.GetLength() <= 0)
	{
		MessageBox("请输入一个值！");
		return;
	}

	switch (selected)
	{
	case 0:
	{
			  const char * pch = (LPCTSTR)cstrValue;
			  unsigned int number = *pch;
			  if (number > 127 || cstrValue.GetLength() > 1)
			  {
				  MessageBox("越界了！");
				  return;
			  }
			  strncpy_s(ch_buffer, buf_len, pch, cstrValue.GetLength());
			  value_len = cstrValue.GetLength();
			  if (0 == m_protocol_type)
			  {
				  InsertCharNode(number, m_head_list_node);
			  }
			  else if (1 == m_protocol_type)
			  {
				  InsertCharNode(number, m_tail_list_node);
			  }
			  else if (2 == m_protocol_type)
			  {
				  InsertCharNode(number, m_body_list_node);
			  }
			  break;
	}
	case 1:
	{
			  unsigned short number = 0;
			  if (!yg_utils::CheckUShortNumber(cstrValue,number))
			  {
				  MessageBox("越界了！");
				  return;
			  }
			  cstrValue = "";
			  cstrValue.Format("%u", number);
			  strncpy_s(ch_buffer, buf_len, cstrValue.GetBuffer(), cstrValue.GetLength());
			  value_len = cstrValue.GetLength();
			  if (0 == m_protocol_type)
			  {
				  InsertShortNode(number, m_head_list_node);
			  }
			  else if (1 == m_protocol_type)
			  {
				  InsertShortNode(number, m_tail_list_node);
			  }
			  else if (2 == m_protocol_type)
			  {
				  InsertShortNode(number, m_body_list_node);
			  }
			  break;
	}
	case 2:
	{
			  unsigned int number = 0;
			  if (!yg_utils::CheckUIntNumber(cstrValue,number))
			  {
				  MessageBox("越界了！");
				  return;
			  }
			  cstrValue = "";
			  cstrValue.Format("%u", number);
			  strncpy_s(ch_buffer, buf_len, cstrValue.GetBuffer(), cstrValue.GetLength());
			  value_len = cstrValue.GetLength();
			  if (0 == m_protocol_type)
			  {
				  InsertIntNode(number, m_head_list_node);
			  }
			  else if (1 == m_protocol_type)
			  {
				  InsertIntNode(number, m_tail_list_node);
			  }
			  else if (2 == m_protocol_type)
			  {
				  InsertIntNode(number, m_body_list_node);
			  }
			  break;
	}
	case 3:
	{
			  unsigned long number = 0;
			  if (!yg_utils::CheckULongNumber(cstrValue,number))
			  {
				  MessageBox("越界了！");
				  return;
			  }
			  cstrValue = "";
			  cstrValue.Format("%u", number);
			  strncpy_s(ch_buffer, buf_len, cstrValue.GetBuffer(), cstrValue.GetLength());
			  value_len = cstrValue.GetLength();
			  if (0 == m_protocol_type)
			  {
				  InsertLongNode(number, m_head_list_node);
			  }
			  else if (1 == m_protocol_type)
			  {
				  InsertLongNode(number, m_tail_list_node);
			  }
			  else if (2 == m_protocol_type)
			  {
				  InsertLongNode(number, m_body_list_node);
			  }
			  break;
	}
	case 4:
	{
			  strncpy_s(ch_buffer, buf_len, cstrValue.GetBuffer(), cstrValue.GetLength());
			  value_len = cstrValue.GetLength();
			  if (0 == m_protocol_type)
			  {
				  InsertStringNode(cstrValue, m_head_list_node);
			  }
			  else if (1 == m_protocol_type)
			  {
				  InsertStringNode(cstrValue, m_tail_list_node);
			  }
			  else if (2 == m_protocol_type)
			  {
				  InsertStringNode(cstrValue, m_body_list_node);
			  }
			  break;
	}
	default:
	{
			   MessageBox("error");
			   delete[]ch_buffer;
			   return;
	}
	}

	char dump_buffer[1024 * 10] = { 0 };
	int dump_len = 1024 * 10;

	
	if (0 == m_protocol_type)
	{
		m_cstr_head += CString(ch_buffer);
		m_head_protocol.SetFocus();
		//m_head_protocol.GetWindowText(tmp);
		memset(ch_buffer, 0, sizeof(ch_buffer));
		strncpy_s(ch_buffer, buf_len, m_cstr_head.GetBuffer(), m_cstr_head.GetLength());
		souitls::dump_hex2(ch_buffer, m_cstr_head.GetLength(), dump_buffer, dump_len);
		CString msg = m_cstr_head;
		if (msg.GetLength() > 0)
		{
			msg += "\r\n";
		}
		msg += dump_buffer;
		int begin_pos = msg.GetLength();
		m_head_protocol.SetWindowText(msg);
		m_head_protocol.SetSel(begin_pos, msg.GetLength());
	}
	else if (1 == m_protocol_type)
	{
		m_cstr_tail += CString(ch_buffer);
		m_tail_protocol.SetFocus();
		//m_tail_protocol.GetWindowText(tmp);
		memset(ch_buffer, 0, sizeof(ch_buffer));
		strncpy_s(ch_buffer, buf_len, m_cstr_tail.GetBuffer(), m_cstr_tail.GetLength());
		souitls::dump_hex2(ch_buffer, m_cstr_tail.GetLength(), dump_buffer, dump_len);
		CString msg = m_cstr_tail;
		if (msg.GetLength() > 0)
		{
			msg += "\r\n";
		}
		msg += dump_buffer;
		int begin_pos = msg.GetLength();
		m_tail_protocol.SetWindowText(msg);
		m_tail_protocol.SetSel(begin_pos, msg.GetLength());
	}
	else if (2 == m_protocol_type)
	{
		m_cstr_body += CString(ch_buffer);
		m_full_body_protocol.SetFocus();
		//m_full_body_protocol.GetWindowText(tmp);
		memset(ch_buffer, 0, sizeof(ch_buffer));
		strncpy_s(ch_buffer, buf_len, m_cstr_head.GetBuffer(), m_cstr_head.GetLength());
		strncpy_s(ch_buffer + m_cstr_head.GetLength(), buf_len - m_cstr_head.GetLength(), m_cstr_body.GetBuffer(), m_cstr_body.GetLength());
		strncpy_s(ch_buffer + m_cstr_head.GetLength() + m_cstr_body.GetLength(), buf_len - m_cstr_head.GetLength() - m_cstr_body.GetLength(), m_cstr_tail.GetBuffer(), m_cstr_tail.GetLength());
		souitls::dump_hex2(ch_buffer, m_cstr_head.GetLength() + m_cstr_body.GetLength() + m_cstr_tail.GetLength(), dump_buffer, dump_len);
		CString msg = m_cstr_head + m_cstr_body + m_cstr_tail;
		if (msg.GetLength() > 0)
		{
			msg += "\r\n";
		}
		msg += dump_buffer;
		int begin_pos = msg.GetLength();
		m_full_body_protocol.SetWindowText(msg);
		m_full_body_protocol.SetSel(begin_pos, msg.GetLength());
	}

	
	UpdateData(FALSE);
	memset(ch_buffer, 0, sizeof(ch_buffer));
	delete [] ch_buffer,ch_buffer = NULL;
}


void CDlgProtocol::OnBnClickedButtonResetProtocol()
{
	// TODO:  在此添加控件通知处理程序代码
	m_field_value = "";
	m_cstr_head = "";
	m_cstr_tail = "";
	m_cstr_body = "";
	m_head_protocol.SetWindowText("");
	m_tail_protocol.SetWindowText("");
	m_full_body_protocol.SetWindowText("");

	ResetList(m_head_list_node);
	ResetList(m_tail_list_node);
	ResetList(m_body_list_node);
	UpdateData(FALSE);
}

void CDlgProtocol::InsertCharNode(unsigned char t_value, std::list<ListNode*> & t_list_node)
{
	ListNode * list_node = new ListNode;
	list_node->t_type = 1;
	list_node->t_next = NULL;
	CharNode * char_node = new CharNode;
	char_node->t_type = 1;
	char_node->t_value = t_value;
	list_node->t_char_node = char_node;

	t_list_node.push_back(list_node);
}

void CDlgProtocol::InsertShortNode(unsigned short t_value, std::list<ListNode*> & t_list_node)
{
	ListNode * list_node = new ListNode;
	list_node->t_type = 2;
	list_node->t_next = NULL;
	ShortNode * short_node = new ShortNode;
	short_node->t_type = 2;
	short_node->t_value = t_value;
	list_node->t_short_node = short_node;

	t_list_node.push_back(list_node);
}

void CDlgProtocol::InsertIntNode(unsigned int t_value, std::list<ListNode*> & t_list_node)
{
	ListNode * list_node = new ListNode;
	list_node->t_type = 4;
	list_node->t_next = NULL;
	IntNode * int_node = new IntNode;
	int_node->t_type = 4;
	int_node->t_value = t_value;
	list_node->t_int_node = int_node;

	t_list_node.push_back(list_node);
	
}

void CDlgProtocol::InsertLongNode(unsigned long t_value, std::list<ListNode*> & t_list_node)
{
	ListNode * list_node = new ListNode;
	list_node->t_type = 8;
	list_node->t_next = NULL;
	LongNode * long_node = new LongNode;
	long_node->t_type = 8;
	long_node->t_value = t_value;
	list_node->t_long_node = long_node;

	t_list_node.push_back(list_node);
}

void CDlgProtocol::InsertStringNode(CString & t_value, std::list<ListNode*> & t_list_node)
{
	ListNode * list_node = new ListNode;
	list_node->t_type = 16;
	list_node->t_next = NULL;
	StrNode * str_node = new StrNode;
	str_node->t_type = 16;
	str_node->t_value = t_value;
	list_node->t_str_node = str_node;

	t_list_node.push_back(list_node);
}

void CDlgProtocol::ResetList(std::list<ListNode*> & t_list_node)
{
	std::list<ListNode*>::iterator it = t_list_node.begin();

	for (; it != t_list_node.end(); it++)
	{
		unsigned short type = (*it)->t_type;
		if (1 == type)
		{
			CharNode * tmp = (*it)->t_char_node;
			delete tmp;
		}
		else if (2 == type)
		{
			ShortNode * tmp = (*it)->t_short_node;
			delete tmp;
		}
		else if (4 == type)
		{
			IntNode * tmp = (*it)->t_int_node;
			delete tmp;
		}
		else if (8 == type)
		{
			LongNode * tmp = (*it)->t_long_node;
			delete tmp;
		}
		else if (16 == type)
		{
			StrNode * tmp = (*it)->t_str_node;
			delete tmp;
		}
		ListNode * tmp = *it;
		delete tmp;
	}
	t_list_node.clear();
}

void CDlgProtocol::Import(CString filePath,CString & text, CRichEditCtrl & rich_edit, std::list<ListNode*> & list_node)
{
	FILE * in_file = fopen(filePath.GetBuffer(), "rb");
	if (NULL == in_file)
	{
		CString msg = "打开文件失败！";
		msg += filePath;
		MessageBox(msg);
		return;
	}
	int len = 0;
	fseek(in_file, 0, SEEK_SET);
	fseek(in_file, 0, SEEK_END);
	len = ftell(in_file);
	fseek(in_file, 0, SEEK_SET);
	//read the first line
	char head_mark[256] = { 0 };
	fgets(head_mark, 256, in_file);
	len = len - strlen(head_mark);
	char * buffer = new char[len + 1];
	memset(buffer, 0, len + 1);
	int rd_len = fread(buffer, sizeof(char), len, in_file);


	fclose(in_file);
	//begin to parse
	char * pHead = head_mark;
	char * pTail = buffer;
	text.Empty();
	while (pHead != NULL && *pHead && pTail != NULL)
	{
		if (*pHead == '1')
		{
			unsigned char * p = (unsigned char*)pTail;
			text += *p;
			InsertCharNode(*p, list_node);
			pTail++;
		}
		else if (*pHead == '2')
		{
			unsigned short * p = (unsigned short*)pTail;
			char dst_buf[64] = { 0 };
			itoa(*p, dst_buf, 10);
			text += dst_buf;
			InsertShortNode(*p, list_node);
			pTail += 2;
		}
		else if (*pHead == '4')
		{
			unsigned int * p = (unsigned int*)pTail;
			char dst_buf[64] = { 0 };
			itoa(*p, dst_buf, 10);
			text += dst_buf;
			InsertIntNode(*p, list_node);
			pTail += 4;
		}
		else if (*pHead == '8')
		{
			unsigned long long * p = (unsigned long long*)pTail;
			char dst_buf[64] = { 0 };
			itoa(*p, dst_buf, 10);
			text += dst_buf;
			InsertLongNode(*p, list_node);
		}
		else if (*pHead == 0x10)
		{
			char dst_buf[128] = { 0 };
			strcpy_s(dst_buf, 128, pTail);
			text += dst_buf;
			CString cstrValue = dst_buf;
			InsertStringNode(cstrValue, list_node);
		}
		pHead++;
	}

	char dump_buffer[1024 * 10] = { 0 };
	int dump_len = 1024 * 10;
	rich_edit.SetFocus();
	souitls::dump_hex2(buffer, rd_len, dump_buffer, dump_len);
	CString msg = text;
	if (msg.GetLength() > 0)
	{
		msg += "\r\n";
	}
	msg += dump_buffer;
	int begin_pos = msg.GetLength();
	rich_edit.SetWindowText(msg);
	rich_edit.SetSel(begin_pos, msg.GetLength());

	delete buffer;
}
