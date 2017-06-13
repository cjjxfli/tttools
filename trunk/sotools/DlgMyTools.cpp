// DlgMyTools.cpp : 实现文件
//

#include "stdafx.h"
#include "sotools.h"
#include "DlgMyTools.h"
#include "afxdialogex.h"

#include "../sodeps/utils.h"
#include "../sodeps/win32/socket.h"


// DlgMyTools 对话框

IMPLEMENT_DYNAMIC(DlgMyTools, CDialogEx)

DlgMyTools::DlgMyTools(CWnd* pParent /*=NULL*/)
	: CDialogEx(DlgMyTools::IDD, pParent)
	, m_netorder_number(_T(""))
	, m_localorder_number(_T(""))
	, m_number_ip(_T(""))
	, m_str_ip(_T(""))
{

}

DlgMyTools::~DlgMyTools()
{
}

void DlgMyTools::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_BYTE_TYPE, m_byte_type);
	DDX_Text(pDX, IDC_EDIT_NET_BYTE_VALUE, m_netorder_number);
	DDX_Text(pDX, IDC_EDIT_HOST_BYTE_VALUE, m_localorder_number);
	DDX_Text(pDX, IDC_EDIT_IP_NUMBER, m_number_ip);
	DDX_Text(pDX, IDC_EDIT_IP_DOT, m_str_ip);
}


BEGIN_MESSAGE_MAP(DlgMyTools, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_BYTE_TYPE, &DlgMyTools::OnBnClickedByteType)
	ON_BN_CLICKED(IDC_RADIO_INT_TYPE, &DlgMyTools::OnBnClickedByteType)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_NET2HOST, &DlgMyTools::OnBnClickedButtonNet2host)
	ON_BN_CLICKED(IDC_BUTTON_HOST2NET, &DlgMyTools::OnBnClickedButtonHost2net)
	ON_BN_CLICKED(IDC_BUTTON_IP_NUM2DOT, &DlgMyTools::OnBnClickedButtonIpNum2dot)
	ON_BN_CLICKED(IDC_BUTTON_IP_DOT2NUM, &DlgMyTools::OnBnClickedButtonIpDot2num)
	//ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// DlgMyTools 消息处理程序
BOOL DlgMyTools::CheckUShortNumber(CString cstrNum, unsigned short & out_number)
{
	//先按unsigned long类型处理
	char   *stop_at = NULL;
	unsigned long number = strtoul(cstrNum.GetBuffer(),&stop_at,0);
	if (number > 32767)
	{
		return FALSE;
	}
	out_number = (unsigned short)number;
	return TRUE;
}

BOOL DlgMyTools::CheckUIntNumber(CString cstrNum, unsigned int & out_number)
{
	//先按unsigned long类型处理
	char   *stop_at = NULL;
	unsigned long number = strtoul(cstrNum.GetBuffer(), &stop_at, 0);
	if (number > 2147483647)
	{
		return FALSE;
	}
	out_number = (unsigned int)number;
	return TRUE;
}

BOOL DlgMyTools::CheckULongNumber(CString cstrNum, unsigned long & out_number)
{
	//先按unsigned long类型处理
	char   *stop_at = NULL;
	unsigned long number = strtoul(cstrNum.GetBuffer(), &stop_at, 0);
	if (number > 9223372036854775807)
	{
		return FALSE;
	}
	out_number = number;
	return TRUE;
}


void DlgMyTools::OnBnClickedByteType()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch (m_byte_type)
	{
	case BYTE_SHORT_TYPE:
	{
						 break;
	}
	case BYTE_INT_TYPE:
	{
							break;
	}
	default:
		break;
	}
}


void DlgMyTools::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
}


HBRUSH DlgMyTools::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_EDIT_NET_BYTE_VALUE:
	case IDC_EDIT_HOST_BYTE_VALUE:
	case IDC_EDIT_IP_NUMBER:
	case IDC_EDIT_IP_DOT:
	{
									COLORREF clr = RGB(0x8A, 0x2B, 0xE2);
									pDC->SetBkMode(TRANSPARENT);
									pDC->SetTextColor(clr);
									pDC->SelectObject(m_pfont);
									return hbr;
	}
	default:
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}


BOOL DlgMyTools::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton*)GetDlgItem(IDC_RADIO_BYTE_TYPE))->SetCheck(TRUE);
	m_byte_type = 0;


	m_pfont = new CFont;
	CRect rc;
	GetDlgItem(IDC_EDIT_NET_BYTE_VALUE)->GetWindowRect(&rc);
	int font_size = rc.bottom - rc.top - 5;
	m_pfont->CreateFont(16, 4, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH&FF_SWISS, "Arial");
	::CreateCaret(GetDlgItem(IDC_EDIT_NET_BYTE_VALUE)->GetSafeHwnd(), NULL, 2, font_size);
	::ShowCaret(GetDlgItem(IDC_EDIT_NET_BYTE_VALUE)->GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

//网站字节序数值转本地字节序
void DlgMyTools::OnBnClickedButtonNet2host()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	char dst_buf[256];
	if (m_byte_type == BYTE_SHORT_TYPE)
	{
		unsigned short number = 0;
		if (!CheckUShortNumber(m_netorder_number,number))
		{
			MessageBox("您传入的数太大了！！");
			return;
		}
		number = ntohs(number);
		m_localorder_number = itoa(number, dst_buf, 10);
	}
	else if (m_byte_type == BYTE_INT_TYPE)
	{
		unsigned int number = 0;// atol(m_netorder_number.GetBuffer());
		if (!CheckUIntNumber(m_netorder_number,number))
		{
			MessageBox("您传入的数太大了！！");
			return;
		}
		number = ntohl(number);
		m_localorder_number = ltoa(number, dst_buf, 10);
	}
	
	UpdateData(FALSE);
}

//本地字节序数值转网络字节序
void DlgMyTools::OnBnClickedButtonHost2net()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	char dst_buf[256];
	if (m_byte_type == BYTE_SHORT_TYPE)
	{
		unsigned short number = 0;// atoi(m_localorder_number.GetBuffer());
		if (!CheckUShortNumber(m_localorder_number,number))
		{
			MessageBox("您传入的数太大了！！");
			return;
		}
		number = htons(number);
		m_netorder_number = itoa(number, dst_buf, 10);
	}
	else if (m_byte_type == BYTE_INT_TYPE)
	{
		unsigned int number = 0;// atol(m_localorder_number.GetBuffer());
		if (!CheckUIntNumber(m_localorder_number,number))
		{
			MessageBox("您传入的数太大了！！");
			return;
		}
		number = htonl(number);
		m_netorder_number = ltoa(number, dst_buf, 10);
	}
	UpdateData(FALSE);
}

//网络字节序ip转字符串
void DlgMyTools::OnBnClickedButtonIpNum2dot()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	char sz_ip[64] = { 0 };
	unsigned int number = 0;
	if (!CheckUIntNumber(m_number_ip,number))
	{
		MessageBox("您传入的IP太大了！！");
		return;
	}
	souitls::ip2str_r(number, sz_ip);
	m_str_ip = sz_ip;
	UpdateData(FALSE);
}


//字符串ip转网络字节序
void DlgMyTools::OnBnClickedButtonIpDot2num()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	sockcommu::ip_4byte_t ip;
	sockcommu::CSocketAddr::in_s2n(m_str_ip.GetBuffer(), ip);
	unsigned int number_ip = *((unsigned int*)(&ip));
	number_ip = ntohl(number_ip);
	char dst_buf[64] = { 0 };
	itoa(number_ip, dst_buf, 10);
	m_number_ip = dst_buf;
	UpdateData(FALSE);
}


BOOL DlgMyTools::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
