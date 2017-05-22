// CurveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Noise.h"
#include "CurveDlg.h"
#include "afxdialogex.h"


// CCurveDlg 对话框

IMPLEMENT_DYNAMIC(CCurveDlg, CDialogEx)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CCurveDlg::IDD, pParent)
{
	m_pWndCurve = NULL;
	m_pBkgndBrush = NULL;
}

CCurveDlg::~CCurveDlg()
{
	if(m_pWndCurve != NULL)       delete m_pWndCurve;
	if (m_pBkgndBrush != NULL)    delete m_pBkgndBrush;
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CCurveDlg 消息处理程序


int CCurveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//曲线窗
	CRect rect;
	GetClientRect(&rect);
	m_pWndCurve = new CCurveWnd;
	m_pWndCurve->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, IDW_CURVE_WINDOW);

	//背景刷
	m_pBkgndBrush = new CBrush;
	m_pBkgndBrush->CreateSolidBrush(RGB(230, 240, 250));

	return 0;
}


void CCurveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CWnd *hWnd = GetDlgItem(IDW_CURVE_WINDOW);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(0, 0, cx, cy);
	}
}


BOOL CCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//WindowSize
	int nDlgWidth = 365+18;
	int nDlgHeight = 375+36;
	::SetWindowPos(this->m_hWnd, HWND_TOP, 100, 100, nDlgWidth, nDlgHeight, SWP_SHOWWINDOW);
	//CenterWindow();

	return TRUE;
}

//重载EraseBackground，擦除背景的时候什么也不做，2014.4.24
//试图解决对话框的大小被改变时有闪屏的问题。
//true表示已处理背景刷新，false表示需要在OnPaint里处理
BOOL CCurveDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CRect rect;
	GetClientRect(&rect);
	pDC->FillRect(&rect, m_pBkgndBrush);
	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}

void CCurveDlg::OnPaint()
{
	CPaintDC dc(this);
}