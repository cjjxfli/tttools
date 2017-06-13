// DlgServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sotools.h"
#include "DlgServer.h"
#include "afxdialogex.h"
#include "../sodeps/commu.h"

#include <Winsock2.h> 


// CDlgServer �Ի���

IMPLEMENT_DYNAMIC(CDlgServer, CDialogEx)

CDlgServer::CDlgServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgServer::IDD, pParent)
	, m_bind_port(_T(""))
	, m_snd_data1(_T(""))
	, m_snd_data2(_T(""))
{

}

CDlgServer::~CDlgServer()
{
}

void CDlgServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENT, m_cli_list);
	DDX_Control(pDX, IDC_COMBO_IP_LIST, m_bind_ip_box);
	DDX_Text(pDX, IDC_EDIT_SERVER_PORT, m_bind_port);
	DDX_Text(pDX, IDC_EDIT_SND_CLI1, m_snd_data1);
	DDX_Text(pDX, IDC_EDIT_SND_CLI2, m_snd_data2);
	DDX_Control(pDX, IDC_RICHEDIT_SND_RCV_CS, m_display_pan);
}


BEGIN_MESSAGE_MAP(CDlgServer, CDialogEx)
	//ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SERVER_LISTEN, &CDlgServer::OnBnClickedButtonServerListen)
	ON_BN_CLICKED(IDC_BUTTON_STOP_LISTEN, &CDlgServer::OnBnClickedButtonStopListen)
	ON_BN_CLICKED(IDC_BUTTON_DISCON_CLIENT, &CDlgServer::OnBnClickedButtonDisconClient)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_SND1, &CDlgServer::OnBnClickedButtonManualSnd1)
	ON_BN_CLICKED(IDC_BUTTON_MANUAL_SND2, &CDlgServer::OnBnClickedButtonManualSnd2)
	ON_CBN_DROPDOWN(IDC_COMBO_IP_LIST, &CDlgServer::OnCbnDropdownComboIpList)
END_MESSAGE_MAP()


// CDlgServer ��Ϣ�������
HBRUSH CDlgServer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}

void CDlgServer::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
}


BOOL CDlgServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	DWORD dwStyle = m_cli_list.GetExtendedStyle(); 
	dwStyle |= LVS_EX_FULLROWSELECT; //�������ĳ����Ԫʽ��һ���ж�ѡ��
	dwStyle |= LVS_EX_GRIDLINES; //���������
	dwStyle |= LVS_EX_CHECKBOXES;
	m_cli_list.SetExtendedStyle(dwStyle);

	m_cli_list.InsertColumn(0, _T(""), LVCFMT_LEFT, 20);
	m_cli_list.InsertColumn(1, _T("id"), LVCFMT_LEFT, 25); 
	m_cli_list.InsertColumn(2, _T("����fd"), LVCFMT_LEFT, 65);
	m_cli_list.InsertColumn(3, _T("����ip"), LVCFMT_LEFT, 110);
	m_cli_list.InsertColumn(4, _T("���Ӷ˿�"), LVCFMT_LEFT, 80);
	m_cli_list.InsertColumn(5, _T("״̬"), LVCFMT_LEFT, 80);

	GetLocalIPAddress();
	m_thread_id = 0;
	m_stop = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgServer::OnBnClickedButtonServerListen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_bind_port.IsEmpty())
	{
		MessageBox("������һ���Ϸ��Ķ˿ڣ�");
		return;
	}
	m_bind_ip_box.GetLBText(m_bind_ip_box.GetCurSel(), m_def_server_addr);
	m_def_port = atoi(m_bind_port.GetBuffer());
	m_iocp_model.SetServerAddr(m_def_server_addr.GetBuffer());
	m_iocp_model.SetPort(m_def_port);
	m_iocp_model.reg_cb(CB_CONNECTED,OnAccept,this);
	m_iocp_model.reg_cb(CB_DISCONNECT, OnDisconnect, this);
	m_iocp_model.reg_cb(CB_RECVDATA, OnRecv, this);
	m_iocp_model.reg_cb_error(OnErrorMsg,this);
	if (!m_iocp_model.Start())
	{
		MessageBox("����������ʧ�ܣ�");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SERVER_LISTEN)->EnableWindow(FALSE);
	}
#if 0
	if (0 != m_server_socket.init())
	{
		return;
	}
	m_server_sock_fd = m_server_socket.create(TCP_SOCKET);
	if (m_server_sock_fd <= 0)
	{
		return;
	}
	if (0 != m_server_socket.bind(m_server_sock_fd,m_def_server_addr.GetBuffer(),m_def_port))
	{
		return;
	}
	_beginthreadex(NULL, 0, &CDlgServer::ServerSocketThd, this, 0, &m_thread_id);
#endif
}


void CDlgServer::OnBnClickedButtonStopListen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_iocp_model.Stop();
	GetDlgItem(IDC_BUTTON_SERVER_LISTEN)->EnableWindow(TRUE);
#if 0
	m_stop = TRUE;
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE,m_thread_id);
	WaitForSingleObject(hThread, 2000);
	CloseHandle(hThread);
	m_server_socket.close(m_server_sock_fd);
	m_server_sock_fd = 0;
#endif
}


void CDlgServer::OnBnClickedButtonDisconClient()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDlgServer::OnBnClickedButtonManualSnd1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SendData(m_snd_data1);
}


void CDlgServer::OnBnClickedButtonManualSnd2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SendData(m_snd_data2);
}


void CDlgServer::GetLocalIPAddress()
{
	//1.��ʼ��wsa
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return ;
	}
	//2.��ȡ������
	char hostname[256];
	ret = gethostname(hostname, sizeof(hostname));
	if (ret == SOCKET_ERROR)
	{
		return;
	}
	//3.��ȡ����ip
	HOSTENT* host = gethostbyname(hostname);
	if (host == NULL)
	{
		return ;
	}
	//4.���ת��Ϊchar*����������
	int cnt = 0;
	cnt = host->h_length < 10 ? host->h_length : 10;
	for (int i = 0; i < cnt; i++)
	{
		in_addr* addr = (in_addr*)*host->h_addr_list;
		CString cstrIP = inet_ntoa(addr[i]);
		//m_bind_ip_box.InsertString(i,cstrIP);
		m_bind_ip_box.AddString( cstrIP);
	}
	if (cnt > 0)
	{
		m_bind_ip_box.SetCurSel(0);
	}
	UpdateData(FALSE);
}

void CDlgServer::OnCbnDropdownComboIpList()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������//����������ĸ߶�  
	CRect rc;
	//����������Ի����У�����ʹ�� GetDlgItem(IDC_COMBO)->GetClientRect(&rc); ��ȡ�ؼ��Ĵ�С  
	GetClientRect(&rc);
	//SetWindowPos�������棬����������е�100���ͱ�ʾ�����б���ʾ�ĸ߶�  
	SetWindowPos(NULL, 0, 0, rc.Width(), rc.Height() + 500, SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);

}

unsigned CDlgServer::ServerSocketThd(void * params)
{
	CDlgServer * this_obj = (CDlgServer*)params;
	while (!this_obj->m_stop)
	{
		int fd = this_obj->m_server_socket.accept(this_obj->m_server_sock_fd);
		ip_4byte_t peer_address;
		port_t peer_port;
		this_obj->m_server_socket.get_peer_name(fd, peer_address, peer_port);
		int item_cnt = this_obj->m_cli_list.GetItemCount();
		char dst_buf[64] = { 0 };
		itoa(item_cnt, dst_buf, 10);
		int row = this_obj->m_cli_list.InsertItem(item_cnt++,dst_buf);
		this_obj->m_cli_list.SetItemText(row, 1, "192.168.1.1");
		sprintf_s(dst_buf, 64, "%d", peer_port);
		this_obj->m_cli_list.SetItemText(row, 2,dst_buf);
		Sleep(100);
	}
	return 0;
}

int CDlgServer::OnAccept(unsigned int fd, void * arg1, void * arg2)
{
	CDlgServer * this_obj = (CDlgServer*)arg2;
	sockaddr_in * paddr = (sockaddr_in*)arg1;
	CString cstrClient = inet_ntoa(paddr->sin_addr);
	unsigned int port = ntohs(paddr->sin_port);
	int item_cnt = this_obj->m_cli_list.GetItemCount();
	char dst_buf[64] = { 0 };
	itoa(item_cnt, dst_buf, 10);
	int row = this_obj->m_cli_list.InsertItem(item_cnt++, "");
	this_obj->m_cli_list.SetItemText(row, 1, dst_buf);
	sprintf_s(dst_buf, 64, "%d", fd);
	this_obj->m_cli_list.SetItemText(row, 2, dst_buf);
	this_obj->m_cli_list.SetItemText(row, 3, cstrClient);
	memset(dst_buf, 0, sizeof(char)* sizeof(dst_buf));
	sprintf_s(dst_buf, 64, "%d", port);
	this_obj->m_cli_list.SetItemText(row, 4, dst_buf);
	this_obj->m_cli_list.SetItemText(row, 5, "On");
	return 0;
}

int CDlgServer::OnDisconnect(unsigned int fd, void * arg1, void * arg2)
{
	CDlgServer * this_obj = (CDlgServer*)arg2;
	sockaddr_in * paddr = (sockaddr_in*)arg1;
	CString cstrClient = inet_ntoa(paddr->sin_addr);
	unsigned int port = ntohs(paddr->sin_port);
	int item_cnt = this_obj->m_cli_list.GetItemCount();
	char dst_buf[64] = { 0 };
	itoa(item_cnt, dst_buf, 10);
	int row = this_obj->m_cli_list.InsertItem(item_cnt++, "");
	this_obj->m_cli_list.SetItemText(row, 1, dst_buf);
	sprintf_s(dst_buf, 64, "%d", fd);
	this_obj->m_cli_list.SetItemText(row, 2, dst_buf);
	this_obj->m_cli_list.SetItemText(row, 3, cstrClient);
	memset(dst_buf, 0, sizeof(char)* sizeof(dst_buf));
	sprintf_s(dst_buf, 64, "%d", port);
	this_obj->m_cli_list.SetItemText(row, 4, dst_buf);
	this_obj->m_cli_list.SetItemText(row, 5, "Off");
	return 0;
}

int CDlgServer::OnErrorMsg(const char * error_msg, unsigned int len, void * arg1)
{
	CDlgServer * this_obj = (CDlgServer*)arg1;
	this_obj->m_display_pan.SetFocus();
	CString text;
	this_obj->GetDlgItem(IDC_RICHEDIT_SND_RCV_CS)->GetWindowText(text);
	CString msg = text;
	if (msg.GetLength() > 0)
	{
		msg += "\r\n";
	}
	msg += error_msg;
	int begin_pos = msg.GetLength();
	this_obj->m_display_pan.SetWindowText(msg);
	this_obj->m_display_pan.SetSel(begin_pos, msg.GetLength());
	return 0;
}

int CDlgServer::OnRecv(unsigned int fd, void * arg1, void * arg2)
{
	blob_type * blob_data = (blob_type*)arg1;
	CDlgServer * this_obj = (CDlgServer*)arg2;
	if (blob_data->len > 0)
	{
		this_obj->m_display_pan.SetFocus();
		CString text;
		this_obj->GetDlgItem(IDC_RICHEDIT_SND_RCV_CS)->GetWindowText(text);
		CString msg = text;
		if (msg.GetLength() > 0)
		{
			msg += "\r\n";
		}
		msg += "�յ���";
		msg += blob_data->data;
		int begin_pos = msg.GetLength();
		this_obj->m_display_pan.SetWindowText(msg);
		this_obj->m_display_pan.SetSel(begin_pos, msg.GetLength());
	}
	return 0;
}

void CDlgServer::SendData(CString & data)
{
	TCHAR szBuf[100];
	/*    memset(szBuf, 0, sizeof(szBuf));*/
	// LVITEM ���ݽṹ, ������ȡCListCtrl���б����ֵ
	LVITEM lvi;
	lvi.pszText = szBuf;            //���item���ı�����
	lvi.iItem = -1;                    //�����±�
	lvi.iSubItem = 2;
	lvi.mask = LVIF_TEXT;
	lvi.cchTextMax = 1024;
	CString str;

	//��ȡѡ�е��б���
	for (int i = 0; i < m_cli_list.GetItemCount(); i++)
	{
		if (LVIS_SELECTED == m_cli_list.GetItemState(i, LVIS_SELECTED))
		{
			lvi.iItem = i;
			m_cli_list.GetItem(&lvi);
			int fd = atoi(lvi.pszText);
			m_iocp_model.DoSend(fd, data.GetBuffer(), data.GetLength());
			return;
		}
	}
	MessageBox("��ѡ����Ҫ�������ݵĿͻ������ӣ�");
}