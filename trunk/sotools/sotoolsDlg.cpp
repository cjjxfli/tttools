
// sotoolsDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CsotoolsDlg �Ի���



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


// CsotoolsDlg ��Ϣ�������

BOOL CsotoolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MAXIMIZE);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	m_is_connected = FALSE;
	m_data_type = SND_BIN_DATA;
	m_rcv_data_type = SND_BIN_DATA;
	
	m_tab_r_view.InsertItem(0, "Э�����ݰ�");  //��Ӳ���һѡ� 
	m_tab_r_view.InsertItem(1, "������");  //��Ӳ�����ѡ� 
	m_tab_r_view.InsertItem(2, "С����");
	m_tab_r_view.InsertItem(3, "��������");    //��ӽ��ѡ�

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
	//�����ӶԻ����ڸ������е�λ�� 
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
	GetClientRect(&rect);     //ȡ�ͻ�����С    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	m_cur_sel_page = 0;

	init_dlg_item();

	m_is_tcp = TRUE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsotoolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsotoolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsotoolsDlg::OnBnClickedButtonTcpConnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	if (m_is_connected)
	{
		m_client.close();
		m_is_connected = FALSE;
		GetDlgItem(IDC_BUTTON_TCP_CONNECT)->SetWindowText("TCP����");
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
		GetDlgItem(IDC_BUTTON_TCP_CONNECT)->SetWindowText("�Ͽ�TCP����");
	}
	else
	{
		MessageBox("���ӷ�����ʧ�ܣ���");
		return;
	}
	m_is_tcp = TRUE;
}


void CsotoolsDlg::OnBnClickedButtonUdpConnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_is_tcp = FALSE;
	if (m_is_connected)
	{
		m_client.close();
		m_is_connected = FALSE;
		GetDlgItem(IDC_BUTTON_UDP_CONNECT)->SetWindowText("UDP����");
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
		GetDlgItem(IDC_BUTTON_UDP_CONNECT)->SetWindowText("�Ͽ�UDP����");
	}
}


void CsotoolsDlg::OnBnClickedButtonSndData0()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!m_is_connected)
	{
		MessageBox("�������ӷ�����", "", MB_OK);
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
		tmp = "\r\n���ͳɹ���\r\n";
		msg += tmp;
		m_snd_rich_edit.SetWindowText(msg);
		m_snd_rich_edit.SetSel(begin_pos, msg.GetLength());
		set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
	}
	else
	{
		int begin_pos = msg.GetLength();
		tmp = "\r\n����ʧ�ܣ�\r\n";
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
			tmp = "\r\n��������\r\n" + CString(dump_buf) + "\r\n�ɹ���\r\n";
			msg += tmp;
			m_rcv_view.SetWindowText(msg);
			m_rcv_view.SetSel(begin_pos, msg.GetLength());
			set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
		}
		else if (m_rcv_data_type == SND_NORMAL_DATA)
		{
			tmp = "\r\n��������\r\n" + CString(dump_buf) + "\r\n�ɹ���\r\n";
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	BOOL isOpen = TRUE;     //�Ƿ��(����Ϊ����)  
	CString defaultDir = ".";   //Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = "";         //Ĭ�ϴ򿪵��ļ���  
	CString filter = "�ļ� (*.txt; *.bin; *.data;*.cap)|*.txt; *.bin; *.data;*.cap|(*.*)|*.*||";   //�ļ����ǵ�����  
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
		msg += "���ļ���" + m_file_name;
		msg += "�ɹ���\r\n";
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (!m_is_connected)
	{
		MessageBox("�������ӷ�����", "", MB_OK);
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
			tmp = "\r\n�����ļ�" + m_file_name + "�ɹ���\r\n";
			msg += tmp;
			m_snd_rich_edit.SetWindowText(msg);
			m_snd_rich_edit.SetSel(begin_pos,msg.GetLength());
			set_rich_text_color(RGB(0x7c,0xcd,0x7c));
		}
		else
		{
			int begin_pos = msg.GetLength();
			tmp = "\r\n�����ļ�" + m_file_name + "ʧ�ܣ�\r\n";
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
				tmp = "\r\n��������\r\n" + CString(dump_buf) + "\r\n�ɹ���\r\n";
				msg += tmp;
				m_rcv_view.SetWindowText(msg);
				m_rcv_view.SetSel(begin_pos, msg.GetLength());
				set_rich_text_color(RGB(0x7c, 0xcd, 0x7c));
			}
			else if (m_rcv_data_type == SND_NORMAL_DATA)
			{
				tmp = "\r\n��������\r\n" + CString(dump_buf) + "\r\n�ɹ���\r\n";
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
	cf.yHeight = 20 * 18;//���ָ߶�
	cf.crTextColor = RGB(0xff, 0x0, 0xff); //������ɫ
	strcpy(cf.szFaceName, _T("����"));//��������

// 	m_snd_rich_edit.SetSel(1, 5); //���ô�������
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
	cf.yHeight = 20 * 18;//���ָ߶�
	cf.crTextColor = rgb; //������ɫ
	m_snd_rich_edit.SetSelectionCharFormat(cf);
	m_rcv_view.SetSelectionCharFormat(cf);
}



void CsotoolsDlg::OnBnClickedRadioDataType()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ˴������Ϣ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_pdialog[m_cur_sel_page]->ShowWindow(SW_HIDE);
	m_cur_sel_page = m_tab_r_view.GetCurSel();
	m_pdialog[m_cur_sel_page]->ShowWindow(SW_SHOW);
}
