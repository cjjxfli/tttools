
// sotoolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "sotools.h"
#include "sotoolsDlg.h"
#include "afxdialogex.h"
#include "../sodeps/Client.h"
#include "../sodeps/utils.h"

#pragma comment(lib,"sodeps.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsotoolsDlg 对话框



CsotoolsDlg::CsotoolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsotoolsDlg::IDD, pParent)
	, m_server_addr(_T(""))
	, m_port(0)
	, m_snd_data0(_T(""))
	, m_snd_file_data(_T(""))
	, m_data_type(0)
	, m_rcv_data_type(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_TCP_UDP48/*IDR_MAINFRAME*/);
}

void CsotoolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDR, m_server_addr);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_DATA0, m_snd_data0);
	//DDX_Text(pDX, IDC_RICHEDIT_FILE_CONTENT, m_snd_file_data);
	DDX_Control(pDX, IDC_RICHEDIT_FILE_CONTENT, m_snd_rich_edit);
	DDX_Radio(pDX, IDC_RADIO_BIN_DATA, m_data_type);
	DDX_Control(pDX, IDC_RICHEDIT2_RCV_VIEW, m_rcv_view);
	DDX_Radio(pDX, IDC_RADIO_RCV_BIN_TYPE, m_rcv_data_type);
	DDX_Control(pDX, IDC_TAB_R_VIEW, m_tab_r_view);
}

BEGIN_MESSAGE_MAP(CsotoolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_TCP_CONNECT, &CsotoolsDlg::OnBnClickedButtonTcpConnect)
	ON_BN_CLICKED(IDC_BUTTON_UDP_CONNECT, &CsotoolsDlg::OnBnClickedButtonUdpConnect)
	ON_BN_CLICKED(IDC_BUTTON_SND_DATA0, &CsotoolsDlg::OnBnClickedButtonSndData0)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CsotoolsDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_SND_FILE_CONTENT, &CsotoolsDlg::OnBnClickedButtonSndFileContent)
	ON_BN_CLICKED(IDC_RADIO_BIN_DATA, &CsotoolsDlg::OnBnClickedRadioDataType)
	ON_BN_CLICKED(IDC_RADIO_NORMAL_DATA, &CsotoolsDlg::OnBnClickedRadioDataType)
	ON_BN_CLICKED(IDC_RADIO_RCV_BIN_TYPE, &CsotoolsDlg::OnBnClickedRadioRcvDataType)
	ON_BN_CLICKED(IDC_RADIO_RCV_NORMAL_TYPE, &CsotoolsDlg::OnBnClickedRadioRcvDataType)
//	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_R_VIEW, &CsotoolsDlg::OnTcnSelchangingTabRView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_R_VIEW, &CsotoolsDlg::OnTcnSelchangeTabRView)
END_MESSAGE_MAP()


// CsotoolsDlg 消息处理程序

BOOL CsotoolsDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO:  在此添加额外的初始化代码

	m_is_connected = FALSE;
	m_data_type = SND_BIN_DATA;
	m_rcv_data_type = SND_BIN_DATA;
	
	m_tab_r_view.InsertItem(0, "协议数据包");  //添加参数一选项卡 
	m_tab_r_view.InsertItem(1, "服务器");  //添加参数二选项卡 
	m_tab_r_view.InsertItem(2, "小工具");
	m_tab_r_view.InsertItem(3, "关于我们");    //添加结果选项卡

	m_dlg_protocol.Create(IDD_DIALOG_PROTOCOL, &m_tab_r_view/*GetDlgItem(IDC_TAB_R_VIEW)*/);
	m_dlg_server.Create(IDD_DIALOG_SERVER, &m_tab_r_view);
	m_dlg_tools.Create(IDD_DIALOG_MYTOOLS, &m_tab_r_view);
	m_dlg_about.Create(IDD_DIALOG_ABOUT, &m_tab_r_view);

	CRect rs;
	CRect dlg_rs;
	CRect chart_rs;
	GetDlgItem(IDC_RICHEDIT2_RCV_VIEW)->GetClientRect(&chart_rs);
	this->GetClientRect(&dlg_rs);
	m_tab_r_view.GetClientRect(&rs);
	//调整子对话框在父窗口中的位置 
	rs.top += 50;
	rs.bottom = dlg_rs.bottom - 50;
	rs.left = chart_rs.right + 200;
	rs.right = dlg_rs.right - 10;

	m_tab_r_view.MoveWindow(&rs);
	rs.left = 10;// chart_rs.right + 10;
	m_dlg_about.MoveWindow(&rs);
	m_dlg_server.MoveWindow(&rs);
	m_dlg_protocol.MoveWindow(&rs);
	m_dlg_tools.MoveWindow(&rs);

	m_dlg_protocol.ShowWindow(TRUE);
	m_dlg_server.ShowWindow(FALSE);
	m_dlg_about.ShowWindow(FALSE);
	m_dlg_tools.ShowWindow(FALSE);
	m_cur_sel_page = m_tab_r_view.SetCurSel(0);

	m_pdialog[0] = &m_dlg_protocol;
	m_pdialog[1] = &m_dlg_server;
	m_pdialog[2] = &m_dlg_tools;
	m_pdialog[3] = &m_dlg_about;

	CRect rect;
	GetClientRect(&rect);     //取客户区大小    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	m_cur_sel_page = 0;

	init_dlg_item();

	m_is_tcp = TRUE;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsotoolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsotoolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CsotoolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsotoolsDlg::OnBnClickedButtonTcpConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	
	if (m_is_connected)
	{
		m_client.close();
		m_is_connected = FALSE;
		GetDlgItem(IDC_BUTTON_TCP_CONNECT)->SetWindowText("TCP连接");
		m_is_tcp = FALSE;
		return;
	}
	UpdateData(TRUE);
	m_client.init(TCP_SOCKET);
	CString ip = m_server_addr;
	unsigned int port = m_port;
	int ret = m_client.connect(ip.GetBuffer(), port);
	if (ret == 0)
	{
		m_is_connected = TRUE;
		GetDlgItem(IDC_BUTTON_TCP_CONNECT)->SetWindowText("断开TCP连接");
	}
	else
	{
		MessageBox("连接服务器失败！！");
		return;
	}
	m_is_tcp = TRUE;
}


void CsotoolsDlg::OnBnClickedButtonUdpConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	m_is_tcp = FALSE;
	if (m_is_connected)
	{
		m_client.close();
		m_is_connected = FALSE;
		GetDlgItem(IDC_BUTTON_UDP_CONNECT)->SetWindowText("UDP连接");
		return;
	}
	UpdateData(TRUE);
	m_client.init(UDP_SOCKET);
	CString ip = m_server_addr;
	unsigned int port = m_port;
	int ret = m_client.connect(ip.GetBuffer(), port);
	if (ret == 0)
	{
		m_is_connected = TRUE;
		GetDlgItem(IDC_BUTTON_UDP_CONNECT)->SetWindowText("断开UDP连接");
	}
}


void CsotoolsDlg::OnBnClickedButtonSndData0()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_is_connected)
	{
		MessageBox("请先连接服务器", "", MB_OK);
		UpdateData(FALSE);
		return;
	}
	UpdateData(TRUE);

	CString tmp;
	m_snd_rich_edit.SetFocus();
	m_snd_rich_edit.GetWindowText(tmp);
	CString msg = tmp;
	m_snd_rich_edit.SetFocus();
	char dump_buf[1024 * 32] = { 0 };
	int buf_size = 1024 * 32;
	unsigned int snd_len = 0;

	//send
	int ret = m_client.send(m_snd_data0.GetBuffer(), m_snd_data0.GetLength(), snd_len);
	if (m_data_type == SND_BIN_DATA)
	{
		souitls::dump_hex2(m_snd_data0.GetBuffer(), m_snd_data0.GetLength(), dump_buf, sizeof(dump_buf));
		m_snd_file_data_dump = CString(dump_buf);
	}
	else if (m_data_type == SND_NORMAL_DATA)
	{
		m_snd_file_data_dump = CString(m_snd_data0);
	}
	if (ret > 0)
	{
		msg += m_snd_file_data_dump;
		int begin_pos = msg.GetLength();
		tmp = "\r\n发送成功！\r\n";
		msg += tmp;
		m_snd_rich_edit.SetWindowText(msg);
		m_snd_rich_edit.SetSel(begin_pos, msg.GetLength());
		set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
	}
	else
	{
		int begin_pos = msg.GetLength();
		tmp = "\r\n发送失败！\r\n";
		msg += tmp;
		m_snd_rich_edit.SetWindowText(msg);
		m_snd_rich_edit.SetSel(begin_pos, msg.GetLength());
		set_rich_text_color(RGB(0xff, 0xff, 0x40));
		if (m_is_tcp)
		{
			OnBnClickedButtonTcpConnect();
		}
		else
		{
			OnBnClickedButtonUdpConnect();
		}

		return;
	}
	m_snd_rich_edit.SetSel(msg.GetLength(), msg.GetLength());

	//recv
	unsigned int rcv_len = 0;
	memset(dump_buf, 0, sizeof(dump_buf));
	for (int i = 0; i < 5; i++)
	{
		ret = m_client.receive(dump_buf, buf_size, rcv_len);
		if (ret > 0)
		{
			break;;
		}
		Sleep(100);
	}

	if (ret > 0)
	{
		CString tmp;
		m_rcv_view.GetWindowText(tmp);
		CString msg = tmp;
		int begin_pos = msg.GetLength();
		if (m_rcv_data_type == SND_BIN_DATA)
		{
			CString str_buffer = CString(dump_buf);
			souitls::dump_hex2(str_buffer.GetBuffer(), (uint32_t)rcv_len, dump_buf, sizeof(dump_buf));
			tmp = "\r\n接收数据\r\n" + CString(dump_buf) + "\r\n成功！\r\n";
			msg += tmp;
			m_rcv_view.SetWindowText(msg);
			m_rcv_view.SetSel(begin_pos, msg.GetLength());
			set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
		}
		else if (m_rcv_data_type == SND_NORMAL_DATA)
		{
			tmp = "\r\n接收数据\r\n" + CString(dump_buf) + "\r\n成功！\r\n";
			msg += tmp;
			m_rcv_view.SetWindowText(msg);
			m_rcv_view.SetSel(begin_pos, msg.GetLength());
			set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
		}
	}
	UpdateData(FALSE);
}


void CsotoolsDlg::OnBnClickedButtonOpenFile()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = ".";   //默认打开的文件路径  
	CString fileName = "";         //默认打开的文件名  
	CString filter = "文件 (*.txt; *.bin; *.data;*.cap)|*.txt; *.bin; *.data;*.cap|(*.*)|*.*||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = ".";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\test.doc";
	if (result == IDOK) 
	{
		filePath = openFileDlg.GetPathName();
		m_file_name = filePath;
	}
	FILE * in_file = fopen(m_file_name.GetBuffer(), "rb");
	if (NULL != in_file)
	{
		CString tmp;
		m_snd_rich_edit.SetFocus();
		m_snd_rich_edit.GetWindowText(tmp);
		CString msg = tmp;
		msg += "打开文件：" + m_file_name;
		msg += "成功！\r\n";
		m_snd_rich_edit.SetWindowText(msg);
		m_snd_file_data.Empty();
		int len = 0;
		fseek(in_file, 0, SEEK_SET);
		fseek(in_file, 0, SEEK_END);
		len = ftell(in_file);
		fseek(in_file, 0, SEEK_SET);
		char * buffer = new char[len + 1];
		int rd_len = fread(buffer, sizeof(char), len, in_file);
		fclose(in_file);
		
		m_snd_file_data = buffer;
		if (m_data_type == SND_BIN_DATA)
		{
			char dump_buf[1024 * 32] = { 0 };
			souitls::dump_hex2(buffer, (uint32_t)rd_len, dump_buf, sizeof(dump_buf));
			m_snd_file_data_dump = CString(dump_buf);
		}
		else if (m_data_type == SND_NORMAL_DATA)
		{
			m_snd_file_data_dump = CString(buffer);
		}
		
		unsigned int snd_len = 0;
		
		msg += m_snd_file_data_dump;
		m_snd_rich_edit.SetWindowText(msg);
		m_snd_rich_edit.SetSel(msg.GetLength(), msg.GetLength());

		UpdateData(FALSE);
		delete buffer;
	}
}


void CsotoolsDlg::OnBnClickedButtonSndFileContent()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_is_connected)
	{
		MessageBox("请先连接服务器", "", MB_OK);
		UpdateData(FALSE);
		return;
	}
	{
		CString tmp;
		m_snd_rich_edit.SetFocus();
		m_snd_rich_edit.GetWindowText(tmp);
		CString msg = tmp;
		m_snd_rich_edit.SetFocus();
		unsigned int snd_len = 0;

		int ret = m_client.send(m_snd_file_data.GetBuffer(), m_snd_file_data.GetLength(), snd_len);
		if (ret > 0)
		{
			int begin_pos = msg.GetLength();
			tmp = "\r\n发送文件" + m_file_name + "成功！\r\n";
			msg += tmp;
			m_snd_rich_edit.SetWindowText(msg);
			m_snd_rich_edit.SetSel(begin_pos,msg.GetLength());
			set_rich_text_color(RGB(0x7c,0xcd,0x7c));
		}
		else
		{
			int begin_pos = msg.GetLength();
			tmp = "\r\n发送文件" + m_file_name + "失败！\r\n";
			msg += tmp;
			m_snd_rich_edit.SetWindowText(msg);
			m_snd_rich_edit.SetSel(begin_pos, msg.GetLength());
			set_rich_text_color(RGB(0xff, 0xff, 0x40));
			if (m_is_tcp)
			{
				OnBnClickedButtonTcpConnect();
			}
			else
			{
				OnBnClickedButtonUdpConnect();
			}
			
			return;
		}
		m_snd_rich_edit.SetSel(msg.GetLength(), msg.GetLength());

		char dump_buf[1024 * 32] = { 0 };
		int buf_size = 1024 * 32;
		unsigned int rcv_len = 0;
		for (int i = 0; i < 5; i++)
		{
			ret = m_client.receive(dump_buf, buf_size, rcv_len);
			if (ret > 0)
			{
				break;;
			}
			Sleep(100);
		}
		
		if (ret > 0)
		{
			CString tmp;
			m_rcv_view.GetWindowText(tmp);
			CString msg = tmp;
			int begin_pos = msg.GetLength();
			if (m_rcv_data_type == SND_BIN_DATA)
			{
				CString str_buffer = CString(dump_buf);
				souitls::dump_hex2(str_buffer.GetBuffer(), (uint32_t)rcv_len, dump_buf, sizeof(dump_buf));
				tmp = "\r\n接收数据\r\n" + CString(dump_buf) + "\r\n成功！\r\n";
				msg += tmp;
				m_rcv_view.SetWindowText(msg);
				m_rcv_view.SetSel(begin_pos, msg.GetLength());
				set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
			}
			else if (m_rcv_data_type == SND_NORMAL_DATA)
			{
				tmp = "\r\n接收数据\r\n" + CString(dump_buf) + "\r\n成功！\r\n";
				msg += tmp;
				m_rcv_view.SetWindowText(msg);
				m_rcv_view.SetSel(begin_pos, msg.GetLength());
				set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
			}
		}
		UpdateData(FALSE);
	}
}

void CsotoolsDlg::init_dlg_item()
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 20 * 18;//文字高度
	cf.crTextColor = RGB(0xff, 0x0, 0xff); //文字颜色
	strcpy(cf.szFaceName, _T("隶书"));//设置字体

// 	m_snd_rich_edit.SetSel(1, 5); //设置处理区域
// 	m_snd_rich_edit.SetSelectionCharFormat(cf);
	m_snd_rich_edit.SetDefaultCharFormat(cf);
	m_rcv_view.SetDefaultCharFormat(cf);
}

void CsotoolsDlg::set_rich_text_color(COLORREF rgb)
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE |
		CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 20 * 18;//文字高度
	cf.crTextColor = rgb; //文字颜色
	m_snd_rich_edit.SetSelectionCharFormat(cf);
	m_rcv_view.SetSelectionCharFormat(cf);
}



void CsotoolsDlg::OnBnClickedRadioDataType()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch (m_data_type)
	{
	case SND_BIN_DATA:
	{
			  break;
	}
	case SND_NORMAL_DATA:
	{
			  break;
	}
	default:
		break;
	}
}


void CsotoolsDlg::OnBnClickedRadioRcvDataType()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch (m_rcv_data_type)
	{
	case SND_BIN_DATA:
	{
						 break;
	}
	case SND_NORMAL_DATA:
	{
						break;
	}
	default:
		break;
	}
}


void CsotoolsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		resize();
	}

}


void CsotoolsDlg::resize()
{
}

void CsotoolsDlg::OnTcnSelchangeTabRView(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	m_pdialog[m_cur_sel_page]->ShowWindow(SW_HIDE);
	m_cur_sel_page = m_tab_r_view.GetCurSel();
	m_pdialog[m_cur_sel_page]->ShowWindow(SW_SHOW);
}
