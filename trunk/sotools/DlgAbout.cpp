// DlgAbout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sotools.h"
#include "DlgAbout.h"
#include "afxdialogex.h"

#define ID_HAND MAKEINTRESOURCE(32649)

// CDlgAbout �Ի���

IMPLEMENT_DYNAMIC(CDlgAbout, CDialogEx)

CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAbout::IDD, pParent)
{

}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgAbout, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_STN_CLICKED(IDC_STATIC_AUTHOR_WEB, &CDlgAbout::OnStnClickedStaticAuthorWeb)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CDlgAbout ��Ϣ�������

HBRUSH CDlgAbout::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	

	switch (pWnd->GetDlgCtrlID())
	{
 	case IDC_STATIC_AUTHOR_EMAIL:
 	case IDC_STATIC_AUTHOR_QQ:
	case IDC_STATIC_AUTHOR_WEB:
/*	case CTLCOLOR_STATIC:*/
	{
//  								 CFont * cFont = new CFont;
//  								 cFont->CreateFont(24, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
//  									 ANSI_CHARSET, OUT_DEFAULT_PRECIS,
//  									 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
//  									 DEFAULT_PITCH&FF_SWISS, "Arial");
// 								 pDC->SetBkMode(TRANSPARENT);
// 								 COLORREF clr = RGB(0x8A, 0x2B, 0xE2);
// 								 pDC->SetTextColor(clr);
// 								 pWnd->SetFont(cFont);
// 								 clr = RGB(0x7D, 0x7D, 0xFF);
// 								 HBRUSH B = CreateSolidBrush(clr);
// 								 return (HBRUSH)B;
								 COLORREF clr = RGB(0x8A, 0x2B, 0xE2);
								 pDC->SetBkMode(TRANSPARENT);
								 pDC->SetTextColor(clr);
								 //pWnd->SetFont(cFont);
								 pDC->SelectObject(&m_font);//����Ϊ15�����壬�����п�
								 //return m_brush;
								 return hbr;//CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	default:
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	return (HBRUSH)::GetStockObject(WHITE_BRUSH);
}


void CDlgAbout::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
}


BOOL CDlgAbout::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_font.CreatePointFont(150, "Arial");//����15�����壬�����п�
	m_brush.CreateSolidBrush(RGB(0, 255, 0));//��ˢΪ��ɫ

	GetDlgItem(IDC_STATIC_AUTHOR_WEB)->GetWindowRect(&m_pRectLink);
	ScreenToClient(&m_pRectLink);

	//about dlb
	CString tmp;
	char buffer[256];
	LoadString(AfxGetInstanceHandle(), IDS_ABOUT_MAIL, buffer, 256);
	tmp = buffer;
	GetDlgItem(IDC_STATIC_AUTHOR_EMAIL)->SetWindowText(tmp);
	buffer[0] = '\0';
	LoadString(AfxGetInstanceHandle(), IDS_ABOUT_QQ, buffer, 256);
	tmp = buffer;
	GetDlgItem(IDC_STATIC_AUTHOR_QQ)->SetWindowText(tmp);
	buffer[0] = '\0';
	LoadString(AfxGetInstanceHandle(), IDS_ABOUT_AUTHOR_WEB,buffer,256);
	tmp = buffer;
	GetDlgItem(IDC_STATIC_AUTHOR_WEB)->SetWindowText(tmp);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CDlgAbout::OnStnClickedStaticAuthorWeb()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char buffer[256];
	buffer[0] = '\0';
	LoadString(AfxGetInstanceHandle(), IDS_ABOUT_AUTHOR_WEB, buffer, 256);
	ShellExecute(0, NULL, buffer, NULL, NULL, SW_NORMAL);
}


void CDlgAbout::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
#if 0
	if (point.x > m_pRectLink.left && point.x < m_pRectLink.right && point.y > m_pRectLink.top && point.y < m_pRectLink.bottom)
	{
		HCURSOR hCursor;
		hCursor = AfxGetApp()->LoadCursor(IDC_HANDLINK);
		SetCursor(hCursor);
	}
	else
	{
		CDialogEx::OnMouseMove(nFlags, point);
	}
#endif
	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CDlgAbout::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pWnd == GetDlgItem(IDC_STATIC_AUTHOR_WEB))
	{
		SetCursor(LoadCursor(NULL, ID_HAND));
		return TRUE;
	}
	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
