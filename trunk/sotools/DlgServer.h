#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "../sodeps/win32/socket.h"
#include "../sodeps/win32/IOCPModel.h"

using namespace sockcommu;


// CDlgServer 对话框

class CDlgServer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgServer)

public:
	CDlgServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgServer();

// 对话框数据
	enum { IDD = IDD_DIALOG_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	void GetLocalIPAddress();
	void SendData(CString & data);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
private:
	CListCtrl m_cli_list;
	CString m_snd_data1;
	CString m_snd_data2;
	CRichEditCtrl m_display_pan;
	// 多网卡，绑定的ip
	CComboBox m_bind_ip_box;
	CString m_bind_port;
	CIOCPModel m_iocp_model;

private:
	CString m_def_server_addr;
	unsigned int m_def_port;
	int m_server_sock_fd;
	CSocket m_server_socket;
	unsigned int m_thread_id;
	BOOL m_stop;

	static unsigned __stdcall ServerSocketThd(void * params);
	static int OnAccept(unsigned int fd,void * arg1,void * arg2);
	static int OnRecv(unsigned int fd, void * arg1, void * arg2);
	static int OnErrorMsg(const char * error_msg, unsigned int len, void * arg1);
	static int OnDisconnect(unsigned int fd, void * arg1, void * arg2);
public:

	afx_msg void OnBnClickedButtonServerListen();
	afx_msg void OnBnClickedButtonStopListen();
	afx_msg void OnBnClickedButtonDisconClient();
	afx_msg void OnBnClickedButtonManualSnd1();
	afx_msg void OnBnClickedButtonManualSnd2();
	
	afx_msg void OnCbnDropdownComboIpList();
};
