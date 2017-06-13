// DlgRTab.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sotools.h"
#include "DlgRTab.h"
#include "afxdialogex.h"


// CDlgRTab �Ի���

IMPLEMENT_DYNAMIC(CDlgRTab, CDialogEx)

CDlgRTab::CDlgRTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRTab::IDD, pParent)
{

}

CDlgRTab::~CDlgRTab()
{
}

void CDlgRTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgRTab, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgRTab ��Ϣ�������
void CDlgRTab::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED)
	{
		resize();
	}

}

void CDlgRTab::resize()
{
	CWnd hWnd;
	float	ratio[2];
	POINT	newDialogSize;
	CRect	newRect;

	//��ȡ�µĿͻ����Ĵ�С
	GetWindowRect(&newRect);
	newDialogSize.x = newRect.right - newRect.left;
	newDialogSize.y = newRect.bottom - newRect.top;

	//�����ڵĶԻ����������Ի���Ĵ�С����
	ratio[0] = (float)newDialogSize.x / m_tab_size.x;
	ratio[1] = (float)newDialogSize.y / m_tab_size.y;

	CRect Rect;
	int woc;

	//�����Ͻǵ�����
	CPoint OldTLPoint, NewTLPint;
	CPoint OldBRPoint, NewBRPint;

	//�г����пؼ�
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);//��ȡ�����ӿؼ�ָ��

	while (hwndChild)
	{
		//ȡ��ID
		woc = ::GetDlgCtrlID(hwndChild);
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������

		OldTLPoint = Rect.TopLeft();//���Ͻ�
		NewTLPint.x = long(OldTLPoint.x*ratio[0]);
		NewTLPint.y = long(OldTLPoint.y*ratio[1]);

		OldBRPoint = Rect.BottomRight();//��С��
		NewBRPint.x = long(OldBRPoint.x*ratio[0]);
		NewBRPint.y = long(OldBRPoint.y*ratio[1]);

		Rect.SetRect(NewTLPint, NewBRPint);
		//�����������
// 		if (woc == IDC_GPS_TYPE || woc == IDC_ZJ_FLAG || woc == IDC_SDDB_SWI || woc == IDC_DEADZONE_TYPE || woc == IDC_DEADZONE_ONETYPE || woc == IDC_DEADZONE_TWOTYPE)
// 			GetDlgItem(woc)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height() + 200, TRUE);//+200��ʾ��������
// 		else
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);//��ȡ�ô����µ��ӿؼ�ָ��
	}
	//��ȡ��Ƶ���ڸ����ڵĴ�С
	::GetClientRect(hWnd, Rect);
	m_tab_size = newDialogSize;
}