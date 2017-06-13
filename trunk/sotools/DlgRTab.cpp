// DlgRTab.cpp : 实现文件
//

#include "stdafx.h"
#include "sotools.h"
#include "DlgRTab.h"
#include "afxdialogex.h"


// CDlgRTab 对话框

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


// CDlgRTab 消息处理程序
void CDlgRTab::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
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

	//获取新的客户区的大小
	GetWindowRect(&newRect);
	newDialogSize.x = newRect.right - newRect.left;
	newDialogSize.y = newRect.bottom - newRect.top;

	//得现在的对话框与以往对话框的大小比例
	ratio[0] = (float)newDialogSize.x / m_tab_size.x;
	ratio[1] = (float)newDialogSize.y / m_tab_size.y;

	CRect Rect;
	int woc;

	//左右上角的数据
	CPoint OldTLPoint, NewTLPint;
	CPoint OldBRPoint, NewBRPint;

	//列出所有控件
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);//获取窗体子控件指针

	while (hwndChild)
	{
		//取得ID
		woc = ::GetDlgCtrlID(hwndChild);
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);//将控件大小转换为在对话框中的区域坐标

		OldTLPoint = Rect.TopLeft();//左上角
		NewTLPint.x = long(OldTLPoint.x*ratio[0]);
		NewTLPint.y = long(OldTLPoint.y*ratio[1]);

		OldBRPoint = Rect.BottomRight();//右小角
		NewBRPint.x = long(OldBRPoint.x*ratio[0]);
		NewBRPint.y = long(OldBRPoint.y*ratio[1]);

		Rect.SetRect(NewTLPint, NewBRPint);
		//如果是下拉框
// 		if (woc == IDC_GPS_TYPE || woc == IDC_ZJ_FLAG || woc == IDC_SDDB_SWI || woc == IDC_DEADZONE_TYPE || woc == IDC_DEADZONE_ONETYPE || woc == IDC_DEADZONE_TWOTYPE)
// 			GetDlgItem(woc)->MoveWindow(Rect.left, Rect.top, Rect.Width(), Rect.Height() + 200, TRUE);//+200表示下拉长度
// 		else
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);//获取该窗口下的子控件指针
	}
	//获取视频窗口父窗口的大小
	::GetClientRect(hWnd, Rect);
	m_tab_size = newDialogSize;
}