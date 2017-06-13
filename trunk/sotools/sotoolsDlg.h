
// sotoolsDlg.h : 头文件
//

#pragma once
#include "../sodeps/Client.h"
#include "afxcmn.h"
#include "DlgAbout.h"
#include "DlgProtocol.h"
#include "DlgServer.h"
#include "DlgMyTools.h"

#define SND_BIN_DATA 0
#define SND_NORMAL_DATA 1

// CsotoolsDlg 对话框
class CsotoolsDlg : public CDialogEx
{
// 构造
public:
	CsotoolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOTOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void init_dlg_item();
	void set_rich_text_color(COLORREF rgb);
	void resize();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CClient m_client;
	BOOL m_is_connected;
	CString m_file_name;
private:
	CString m_server_addr;
	CString m_snd_data0;
	CString m_snd_file_data;
	CString m_snd_file_data_dump;
	int m_port;
	// 文件发送内容
	CRichEditCtrl m_snd_rich_edit;
	// 待发送数据类型
	int m_data_type;
	// 接收数据面板
	CRichEditCtrl m_rcv_view;
	// 接收的数据类型
	int m_rcv_data_type;

	CTabCtrl m_tab_r_view;
	CDlgAbout m_dlg_about;
	CDlgProtocol m_dlg_protocol;
	CDlgServer m_dlg_server;
	DlgMyTools m_dlg_tools;
	POINT old;

	CDialogEx* m_pdialog[4];
	int m_cur_sel_page;

	BOOL m_is_tcp;
public:
	afx_msg void OnBnClickedButtonTcpConnect();
	afx_msg void OnBnClickedButtonUdpConnect();
	afx_msg void OnBnClickedButtonSndData0();
	afx_msg void OnBnClickedButtonOpenFile();
	afx_msg void OnBnClickedButtonSndFileContent();
	afx_msg void OnBnClickedRadioDataType();
	afx_msg void OnBnClickedRadioRcvDataType();


//	afx_msg void OnTcnSelchangingTabRView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTcnSelchangeTabRView(NMHDR *pNMHDR, LRESULT *pResult);
};
