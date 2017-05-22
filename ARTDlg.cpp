// ARTDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Noise.h"
#include "ARTDlg.h"
#include "afxdialogex.h"
#include "NoiseDlg.h"
#include "NoiseWnd.h"
#include "ARTDlgProjectSetting.h"
#include "ARTDlgScanSetting.h"


// CARTDlg 对话框

IMPLEMENT_DYNAMIC(CARTDlg, CDialogEx)

CARTDlg::CARTDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CARTDlg::IDD, pParent)
{
	m_pBnCreateImage = NULL;
	m_pBnProject = NULL;
	m_pBnScaning = NULL;
	m_pBnPause = NULL;
	m_pBnSingleLine = NULL;
	m_pBnSingleStep = NULL;
	m_pBnRandom = NULL;
	m_pBnARTSolve = NULL;
	m_pBnOK = NULL;
	m_pBnCancel = NULL;
	m_pBnProjectSetting = NULL;
	m_pBnScanSetting = NULL;

	m_pPaintRect = NULL;
	m_pBmpOrigin = NULL;
	m_pBmpProject = NULL;
	m_pBmpBackProject = NULL;

	m_bImageCreated = FALSE;
	m_bProjected = FALSE;
	m_bHaveScaned = FALSE;
	m_bARTPaused = FALSE;
	m_bHaveRebuild = FALSE;
	m_bIterateLByL = -1;
	m_nCurImageFlag = 0;
	m_nIterateLine = 0;
	m_nIterateStep = 0;
	m_nLineGap = 10;
	m_nStepGap = 1000;
}

CARTDlg::~CARTDlg()
{
	if(m_pMyImage != NULL)               delete m_pMyImage;
	if(m_pMyImageCopy != NULL)           delete m_pMyImageCopy;
	if(m_pBig != NULL)                   delete m_pBig;
	if(m_pSmall != NULL)                 delete m_pSmall;
	if (m_pRays != NULL)                 delete [] m_pRays;

	if(m_pBnCreateImage != NULL)         delete m_pBnCreateImage;
	if(m_pBnProject != NULL)             delete m_pBnProject;
	if(m_pBnScaning != NULL)             delete m_pBnScaning;
	if(m_pBnPause != NULL)               delete m_pBnPause;
	if(m_pBnSingleLine != NULL)          delete m_pBnSingleLine;
	if(m_pBnSingleStep != NULL)          delete m_pBnSingleStep;
	if(m_pBnRandom != NULL)              delete m_pBnRandom;
	if(m_pBnARTSolve != NULL)            delete m_pBnARTSolve;
	if(m_pBnOK != NULL)                  delete m_pBnOK;
	if(m_pBnCancel != NULL)              delete m_pBnCancel;
	if(m_pBnProjectSetting != NULL)      delete m_pBnProjectSetting;
	if(m_pBnScanSetting != NULL)         delete m_pBnScanSetting;

	if(m_pPaintRect != NULL)             delete m_pPaintRect;
	if(m_pBmpOrigin != NULL)             delete m_pBmpOrigin;
	if(m_pBmpProject != NULL)            delete m_pBmpProject;
	if(m_pBmpBackProject != NULL)        delete m_pBmpBackProject;
}

void CARTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CARTDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDB_ARTDLG_CREATE_IMAGE, &CARTDlg::OnIdbCreateImage)
	ON_BN_CLICKED(IDB_ARTDLG_PROJECT, &CARTDlg::OnIdbProject)
	ON_BN_CLICKED(IDB_ARTDLG_SCANING, &CARTDlg::OnIdbScaning)
	ON_BN_CLICKED(IDB_ARTDLG_PAUSE, &CARTDlg::OnIdbPause)
	ON_BN_CLICKED(IDB_ARTDLG_SINGLE_LINE, &CARTDlg::OnIdbSingleLine)
	ON_BN_CLICKED(IDB_ARTDLG_SINGLE_STEP, &CARTDlg::OnIdbSingleStep)
	ON_BN_CLICKED(IDB_ARTDLG_RANDOM, &CARTDlg::OnIdbRandom)
	ON_BN_CLICKED(IDB_ARTDLG_SOLVE, &CARTDlg::OnIdbSolve)
	ON_BN_CLICKED(IDB_ARTDLG_OK, &CARTDlg::OnIdbOK)
	ON_BN_CLICKED(IDB_ARTDLG_CANCEL, &CARTDlg::OnIdbCancel)
	ON_BN_CLICKED(IDB_ARTDLG_PROJECT_SETTING, &CARTDlg::OnIdbProjectSetting)
	ON_BN_CLICKED(IDB_ARTDLG_SCAN_SETTING, &CARTDlg::OnIdbScanSetting)
END_MESSAGE_MAP()


// CARTDlg 消息处理程序


int CARTDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//属性
	m_nWidth = 256;                          //图像宽度
	m_nHeight = 256;                         //图像高度
	m_nPixelQuantity = 256*256;              //像素数
	m_BigRadius = 128;                       //大圆半径
	m_SmallRadius = 64;                     //小圆半径
	m_BigCenter = CMyPoint(128, 128);        //大圆圆心
	m_SmallCenter = CMyPoint(192, 128);      //小圆圆心
	m_BigDecay = 0.3;                        //大圆衰减系数
	m_SmallDecay = 0.7;                      //小圆衰减系数

	//射线
	m_AngleBegin = PRECISION;                //起始角
	m_AngleEnd = 2*PI-PRECISION;             //终止角
	m_nAngleQuantity = 180;                  //角度采样数
	m_DistanceBegin = PRECISION;             //起始距离
	m_DistanceEnd = 2*sqrt(2.0)*m_BigRadius; //终止距离
	m_nDistanceQuantity = 180;               //距离采样数

	m_pBig = new CMyCircle(m_BigCenter, m_BigRadius, m_BigDecay);
	m_pSmall = new CMyCircle(m_SmallCenter, m_SmallRadius, m_SmallDecay);
	m_pMyImage = new CMyImage(m_nWidth, m_nHeight, m_nWidth, m_nHeight);
	m_pMyImageCopy = new CMyImage;

	CRect rect;
	GetClientRect(&rect);
	int nWindowHeight = rect.Height();
	double Space = nWindowHeight/7.0;

	m_pPaintRect = new CRect(150, 10, rect.right-10, rect.bottom-10);

	//建立图像
	m_pBnCreateImage = new CMFCButton;
	m_pBnCreateImage->Create(_T("图像"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, int(Space-12), 70, int(Space+12)), this, IDB_ARTDLG_CREATE_IMAGE);
	m_pBnCreateImage->SetTooltip(_T("建立一副图像"));
	m_pBnCreateImage->SetMouseCursorHand();

	//投影
	m_pBnProject = new CMFCButton;
	m_pBnProject->Create(_T("投影"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, int(Space-12), 140, int(Space+12)), this, IDB_ARTDLG_PROJECT);
	m_pBnProject->SetTooltip(_T("生成投影数据"));
	m_pBnProject->SetMouseCursorHand();

	//交线长
	m_pBnScaning = new CMFCButton;
	m_pBnScaning->Create(_T("交线"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, int(2*Space-12), 70, int(2*Space+12)), this, IDB_ARTDLG_SCANING);
	m_pBnScaning->SetTooltip(_T("计算交线长"));
	m_pBnScaning->SetMouseCursorHand();
	m_pBnScaning->EnableWindow(FALSE);

	//暂停
	m_pBnPause = new CMFCButton;
	m_pBnPause->Create(_T("暂停"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, int(2*Space-12), 140, int(2*Space+12)), this, IDB_ARTDLG_PAUSE);
	m_pBnPause->SetTooltip(_T("暂停当前迭代"));
	m_pBnPause->SetMouseCursorHand();
	m_pBnPause->EnableWindow(FALSE);

	//逐线
	m_pBnSingleLine = new CMFCButton;
	m_pBnSingleLine->Create(_T("逐线"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, int(3*Space-12), 70, int(3*Space+12)), this, IDB_ARTDLG_SINGLE_LINE);
	m_pBnSingleLine->SetTooltip(_T("逐条直线迭代"));
	m_pBnSingleLine->SetMouseCursorHand();
	m_pBnSingleLine->EnableWindow(FALSE);

	//逐步
	m_pBnSingleStep = new CMFCButton;
	m_pBnSingleStep->Create(_T("逐步"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, int(3*Space-12), 140, int(3*Space+12)), this, IDB_ARTDLG_SINGLE_STEP);
	m_pBnSingleStep->SetTooltip(_T("按步数进行迭代"));
	m_pBnSingleStep->SetMouseCursorHand();
	m_pBnSingleStep->EnableWindow(FALSE);

	//随机
	m_pBnRandom = new CMFCButton;
	m_pBnRandom->Create(_T("随机"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, int(4*Space-12), 60, int(4*Space+12)), this, IDB_ARTDLG_RANDOM);
	m_pBnRandom->SetTooltip(_T("随机迭代求解"));
	m_pBnRandom->SetMouseCursorHand();
	m_pBnRandom->EnableWindow(FALSE);

	//立即
	m_pBnARTSolve = new CMFCButton;
	m_pBnARTSolve->Create(_T("立即"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, int(4*Space-12), 140, int(4*Space+12)), this, IDB_ARTDLG_SOLVE);
	m_pBnARTSolve->SetTooltip(_T("立即重建图像"));
	m_pBnARTSolve->SetMouseCursorHand();
	m_pBnARTSolve->EnableWindow(FALSE);

	//投影设置
	m_pBnProjectSetting = new CMFCButton;
	m_pBnProjectSetting->Create(_T("投影设置"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, int(5*Space-12), 70, int(5*Space+12)), this, IDB_ARTDLG_PROJECT_SETTING);
	m_pBnProjectSetting->SetTooltip(_T("投影设置"));
	m_pBnProjectSetting->SetMouseCursorHand();

	//扫描设置
	m_pBnScanSetting = new CMFCButton;
	m_pBnScanSetting->Create(_T("扫描设置"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, int(5*Space-12), 140, int(5*Space+12)), this, IDB_ARTDLG_SCAN_SETTING);
	m_pBnScanSetting->SetTooltip(_T("扫描设置"));
	m_pBnScanSetting->SetMouseCursorHand();

	//确定
	m_pBnOK = new CMFCButton;
	m_pBnOK->Create(_T("确定"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, int(6*Space-12), 70, int(6*Space+12)), this, IDB_ARTDLG_OK);
	m_pBnOK->SetTooltip(_T("OK"));
	m_pBnOK->SetMouseCursorHand();

	//取消
	m_pBnCancel = new CMFCButton;
	m_pBnCancel->Create(_T("取消"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, int(6*Space-12), 140, int(6*Space+12)), this, IDB_ARTDLG_CANCEL);
	m_pBnCancel->SetTooltip(_T("关闭"));
	m_pBnCancel->SetMouseCursorHand();

	//位图
	m_pBmpOrigin = new CBitmap;
	m_pBmpProject = new CBitmap;
	m_pBmpBackProject = new CBitmap;

	return 0;
}

void CARTDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(&rect);
	m_pPaintRect->right = rect.right-10;
	m_pPaintRect->bottom = rect.bottom-10;

	int nWindowHeight = rect.Height();
	double Space = nWindowHeight/7.0;
	Reshape(IDB_ARTDLG_CREATE_IMAGE, Space, 1, 1);
	Reshape(IDB_ARTDLG_PROJECT, Space, 1, 2);
	Reshape(IDB_ARTDLG_SCANING, Space, 2, 1);
	Reshape(IDB_ARTDLG_PAUSE, Space, 2, 2);
	Reshape(IDB_ARTDLG_SINGLE_LINE, Space, 3, 1);
	Reshape(IDB_ARTDLG_SINGLE_STEP, Space, 3, 2);
	Reshape(IDB_ARTDLG_RANDOM, Space, 4, 1);
	Reshape(IDB_ARTDLG_SOLVE, Space, 4, 2);
	Reshape(IDB_ARTDLG_OK, Space, 5, 1);
	Reshape(IDB_ARTDLG_CANCEL, Space, 5, 2);
	Reshape(IDB_ARTDLG_PROJECT_SETTING, Space, 6, 1);
	Reshape(IDB_ARTDLG_SCAN_SETTING, Space, 6, 2);
	Invalidate();
}

void CARTDlg::Reshape(int nItemID, double Space, int nNumber, int Col)
{
	CWnd *hWnd = GetDlgItem(nItemID);
	if (hWnd != NULL)
		hWnd->MoveWindow(10+(Col-1)*70, int(nNumber*Space-12), 60, 25);
}

void CARTDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	int x = m_pPaintRect->Width()-m_nWidth;
	int y = m_pPaintRect->Height()-m_nHeight;
	switch (m_nCurImageFlag)
	{
	case ORIGIN       : MemDC.SelectObject(m_pBmpOrigin);
		dc.BitBlt(m_pPaintRect->left+0.5*x, m_pPaintRect->top+0.5*y, m_nWidth, m_nHeight, &MemDC, 0, 0, SRCCOPY);
		break;
	case PROJECT      : MemDC.SelectObject(m_pBmpProject);
		x = m_pPaintRect->Width()-m_nDistanceQuantity;
		y = m_pPaintRect->Height()-m_nAngleQuantity;
		dc.BitBlt(m_pPaintRect->left+0.5*x, m_pPaintRect->top+0.5*y, m_nDistanceQuantity, m_nAngleQuantity, &MemDC, 0, 0, SRCCOPY);
		break;
	case BACK_PROJECT : MemDC.SelectObject(m_pBmpBackProject);
		dc.BitBlt(m_pPaintRect->left+0.5*x, m_pPaintRect->top+0.5*y, m_nWidth, m_nHeight, &MemDC, 0, 0, SRCCOPY);
		break;
	default: return;
	}
	MemDC.DeleteDC();
}


BOOL CARTDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置对话框大小
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 682 + 18, 532 + 36, SWP_SHOWWINDOW);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL CARTDlg::CreateOriginImage()
{
	TYPE* pTemp = new TYPE[m_nPixelQuantity];
	for(LONG i = 0; i<m_nPixelQuantity; i++)
	{
			pTemp[i] = 0;
	}
	for (int j = 0; j<m_nHeight; j++)
	{
		for (int i = 0; i<m_nWidth; i++)
		{
			if (Distance(CMyPoint(i, j), m_BigCenter)>m_BigRadius)
			{//点不在大圆内
				pTemp[i + j*m_nWidth] = 0;
			}
			else if(Distance(CMyPoint(i, j), m_SmallCenter)>m_SmallRadius)
			{//点在大圆内，但不在小圆内
				pTemp[i + j*m_nWidth] = m_BigDecay;
			}
			else
			{//点在小圆内
				pTemp[i + j*m_nWidth] = m_SmallDecay;
			}
		}
	}
	CreateBitmapUsingMatrix(m_pBmpOrigin, pTemp, m_nWidth, m_nHeight);

	delete [] pTemp;
	return TRUE;
}

//创建目标位图
BOOL CARTDlg::CreateTargetImage(CBitmap* Target)
{
	LONG len = m_nWidth*m_nHeight;

	//拷贝出一个副本来
	TYPE* pTemp = new TYPE[len];

	for (int i = 0; i<len; i++)
	{
		pTemp[i] = m_pMyImageCopy->m_PixelArray[i].m_dValue;
	}
	CreateBitmapUsingMatrix(Target, pTemp, m_nWidth, m_nHeight);

	delete [] pTemp;
	return TRUE;
}

//通知上一层的窗口
void CARTDlg::InformParent()
{
	if(m_bHaveRebuild == FALSE) return;

	CNoiseDlg* pWnd = (CNoiseDlg*)GetParent();
	CNoiseWnd* pNoiseWnd = pWnd->m_pNoiseWnd;
	CBitmap* pBitmap = pNoiseWnd->m_pBitmap;
	
	//修改NoiseWnd参数:图像信息
	if (pNoiseWnd->m_bHaveLoadImage == TRUE)
	{//如果已经载入图像，再次载入时需先释放内存
		pNoiseWnd->m_bHaveLoadImage = FALSE;
		delete pNoiseWnd->m_pBitmap;
		delete pNoiseWnd->m_pBmpShow;
	}

	m_pBmpBackProject->GetBitmap(&pNoiseWnd->m_bmpInfo);
	pNoiseWnd->m_bytesPixel = pNoiseWnd->m_bmpInfo.bmBitsPixel/8;
	pNoiseWnd->m_AspectRatio = 1.0*pNoiseWnd->m_bmpInfo.bmWidth/pNoiseWnd->m_bmpInfo.bmHeight;

	pNoiseWnd->m_nWidthCur  = pNoiseWnd->m_nWidthSrc  = pNoiseWnd->m_bmpInfo.bmWidth;
	pNoiseWnd->m_nHeightCur = pNoiseWnd->m_nHeightSrc = pNoiseWnd->m_bmpInfo.bmHeight;
	pNoiseWnd->m_nRowlenCur = pNoiseWnd->m_nRowlenSrc = pNoiseWnd->m_bmpInfo.bmWidthBytes;
	pNoiseWnd->m_nlenCur    = pNoiseWnd->m_nlenSrc    = pNoiseWnd->m_nHeightSrc * pNoiseWnd->m_nRowlenCur;

	CreateTargetImage(pBitmap);
	CreateTargetImage(pNoiseWnd->m_pBmpShow);
	pNoiseWnd->InvalidateRect(pNoiseWnd->m_pPaintRect);

	pNoiseWnd->m_bHaveLoadImage = TRUE;
	//启用按钮
	pNoiseWnd->m_pBnSaveImage->EnableWindow(TRUE);
	pNoiseWnd->m_pBnGauss->EnableWindow(TRUE);
	pNoiseWnd->m_pBnPepper->EnableWindow(TRUE);
	pNoiseWnd->m_pBnMedianFilter->EnableWindow(TRUE);;
	pNoiseWnd->m_pBnSAMedianFilter->EnableWindow(TRUE);
	pNoiseWnd->m_pBnCurve->EnableWindow(TRUE);
	pNoiseWnd->m_pBnFourier->EnableWindow(TRUE);
	return;
}

//  *** 消息响应 ***

//生成源图像
afx_msg void CARTDlg::OnIdbCreateImage()
{
	//使显示图像为Origin
	m_nCurImageFlag = ORIGIN;
	//如果没有生成源图像，则创建它
	if (m_bImageCreated == FALSE)
	{
		m_bImageCreated = TRUE;
		CreateOriginImage();
	}
	InvalidateRect(m_pPaintRect);
}

//计算投影
afx_msg void CARTDlg::OnIdbProject()
{
	//使显示图像为Project
	m_nCurImageFlag = PROJECT;
	if (m_bProjected == FALSE)
	{
		m_bProjected = TRUE;
		m_pRays = Demo(m_AngleBegin, m_AngleEnd, m_nAngleQuantity, m_DistanceBegin, m_DistanceEnd, m_nDistanceQuantity, *m_pBig, *m_pSmall);
		TYPE MAX = 2*m_BigRadius*m_BigDecay + 2*m_SmallRadius*m_SmallDecay;
		CreateBitmapUsingRays(m_pBmpProject, m_pRays, MAX, m_nDistanceQuantity, m_nAngleQuantity);
		//启用"交线"按钮
		m_pBnScaning->EnableWindow(TRUE);
	}
	InvalidateRect(m_pPaintRect);
}

//计算交线长
afx_msg void CARTDlg::OnIdbScaning()
{
	//使显示图像为BackProject
	m_nCurImageFlag = PROJECT;
	if (m_bHaveScaned == FALSE)
	{
		m_bHaveScaned = TRUE;
		//下面计算交线
		BeginWaitCursor();
		m_pMyImage->SolveAll(m_pRays, m_nAngleQuantity*m_nDistanceQuantity);
		m_pMyImageCopy->CreateImage(*m_pMyImage);//创建副本
		EndWaitCursor();
		//启用迭代求解按钮
		m_pBnSingleLine->EnableWindow(TRUE);
		m_pBnSingleStep->EnableWindow(TRUE);
		m_pBnRandom->EnableWindow(TRUE);
		m_pBnARTSolve->EnableWindow(TRUE);
	}
	InvalidateRect(m_pPaintRect);
}

//暂停
afx_msg void CARTDlg::OnIdbPause()
{
	if (m_bIterateLByL == -1) return;
	m_nCurImageFlag = BACK_PROJECT;
	if (m_bARTPaused == FALSE)
	{
		switch (m_bIterateLByL)
		{//暂停迭代
		case TRUE:
			KillTimer(IDB_ARTDLG_SINGLE_LINE);
			break;
		case FALSE:
			KillTimer(IDB_ARTDLG_SINGLE_STEP);
			break;
		default:
			break;
		}
		m_pBnPause->SetWindowTextW(L"继续");
	}
	else
	{
		switch (m_bIterateLByL)
		{//继续迭代
		case TRUE  : 
			SetTimer(IDB_ARTDLG_SINGLE_LINE, m_nLineGap, NULL);
			break;
		case FALSE : 
			SetTimer(IDB_ARTDLG_SINGLE_STEP, m_nStepGap, NULL);
			break;
		default:break;
		}
		m_pBnPause->SetWindowTextW(L"暂停");
	}
	m_bARTPaused = !m_bARTPaused;
}

afx_msg void CARTDlg::OnIdbSingleLine()
{
	if (m_bIterateLByL != TRUE)
	{//这表明切换了按步数求解，先将副本数据还原
		m_pMyImageCopy->Assign(*m_pMyImage);
		KillTimer(IDB_ARTDLG_SINGLE_STEP);
	}
	m_bIterateLByL = TRUE;
	m_pBnPause->EnableWindow(TRUE);
	m_pBnSingleLine->EnableWindow(FALSE);
	m_pBnSingleStep->EnableWindow(TRUE);
	m_pBnRandom->EnableWindow(TRUE);
	m_pBnARTSolve->EnableWindow(TRUE);
	m_nCurImageFlag = BACK_PROJECT;
	SetTimer(IDB_ARTDLG_SINGLE_LINE, m_nLineGap, NULL);
}

afx_msg void CARTDlg::OnIdbSingleStep()
{
	if (m_bIterateLByL != FALSE)
	{//这表明切换了按步数求解，先将副本数据还原
		m_pMyImageCopy->Assign(*m_pMyImage);
		KillTimer(IDB_ARTDLG_SINGLE_LINE);
	}
	m_bIterateLByL = FALSE;
	m_pBnPause->EnableWindow(TRUE);
	m_pBnSingleLine->EnableWindow(TRUE);
	m_pBnSingleStep->EnableWindow(FALSE);
	m_pBnRandom->EnableWindow(TRUE);
	m_pBnARTSolve->EnableWindow(TRUE);
	m_nCurImageFlag = BACK_PROJECT;
	SetTimer(IDB_ARTDLG_SINGLE_STEP, m_nStepGap, NULL);
}

afx_msg void CARTDlg::OnIdbRandom()
{
	if (m_bIterateLByL != -1)
	{//这表明切换了按步数求解，先将副本数据还原
		m_pMyImageCopy->Assign(*m_pMyImage);
	}
	m_bIterateLByL = -1;
	m_pBnPause->EnableWindow(TRUE);
	m_pBnSingleLine->EnableWindow(TRUE);
	m_pBnSingleStep->EnableWindow(TRUE);
	m_pBnRandom->EnableWindow(FALSE);
	m_pBnARTSolve->EnableWindow(TRUE);
	m_nCurImageFlag = BACK_PROJECT;
}

afx_msg void CARTDlg::OnIdbSolve()
{
	if (m_bIterateLByL != -2)
	{//这表明切换了按步数求解，先将副本数据还原
		m_pMyImageCopy->Assign(*m_pMyImage);
		KillTimer(IDB_ARTDLG_SINGLE_LINE);
		KillTimer(IDB_ARTDLG_SINGLE_STEP);
	}
	m_bIterateLByL = -2;
	//关闭暂停按钮
	m_pBnPause->EnableWindow(FALSE);
	m_pBnSingleLine->EnableWindow(TRUE);
	m_pBnSingleStep->EnableWindow(TRUE);
	m_pBnRandom->EnableWindow(TRUE);
	m_pBnARTSolve->EnableWindow(FALSE);
	m_nCurImageFlag = BACK_PROJECT;
	if (m_bHaveRebuild == FALSE)
	{
		m_bHaveRebuild = TRUE;
		BeginWaitCursor();
		m_pMyImageCopy->ARTSolution();
		EndWaitCursor();
		CreateTargetImage(m_pBmpBackProject);
	}
	InvalidateRect(m_pPaintRect);
}

afx_msg void CARTDlg::OnIdbOK()
{
	//通知主窗口,将重建出来的图像拷贝给它
	InformParent();
	::SendMessage(this->GetSafeHwnd(),WM_CLOSE,0,0);
}

afx_msg void CARTDlg::OnIdbCancel()
{
	::SendMessage(this->GetSafeHwnd(),WM_CLOSE,0,0);
}

//为需要模拟重建的图像设置参数
afx_msg void CARTDlg::OnIdbProjectSetting()
{
	CARTDlgProjectSetting dlg;
	dlg.m_nWidth       = m_nWidth;
	dlg.m_nHeight      = m_nHeight;
	dlg.m_BigRadius    = m_BigRadius;
	dlg.m_BigDecay     = m_BigDecay;
	dlg.m_BigCenterX   = m_BigCenter.x;
	dlg.m_BigCenterY   = m_BigCenter.y;
	dlg.m_SmallRadius  = m_SmallRadius;
	dlg.m_SmallDecay   = m_SmallDecay;
	dlg.m_SmallCenterX = m_SmallCenter.x;
	dlg.m_SmallCenterY = m_SmallCenter.y;
	if (dlg.DoModal() == IDOK)
	{
		m_nWidth        = dlg.m_nWidth;
		m_nHeight       = dlg.m_nHeight;
		m_BigRadius     = dlg.m_BigRadius;
		m_BigDecay      = dlg.m_BigDecay;
		m_BigCenter.x   = dlg.m_BigCenterX;
		m_BigCenter.y   = dlg.m_BigCenterY;
		m_SmallRadius   = dlg.m_SmallRadius;
		m_SmallDecay    = dlg.m_SmallDecay;
		m_SmallCenter.x = dlg.m_SmallCenterX;
		m_SmallCenter.y = dlg.m_SmallCenterY;

		m_nPixelQuantity = m_nWidth*m_nHeight;

		delete m_pBig;
		delete m_pSmall;
		delete m_pMyImage;
		delete m_pMyImageCopy;
		m_pBig = new CMyCircle(m_BigCenter, m_BigRadius, m_BigDecay);
		m_pSmall = new CMyCircle(m_SmallCenter, m_SmallRadius, m_SmallDecay);
		m_pMyImage = new CMyImage(m_nWidth, m_nHeight, m_nWidth, m_nHeight);
		m_pMyImageCopy = new CMyImage;
		if (m_pRays != NULL)
		{//delete之后置为NULL，以防出现问题.2014.4.28注
			delete [] m_pRays;
			m_pRays = NULL;
		}
		m_bImageCreated = FALSE;
		m_bProjected = FALSE;
		m_bHaveScaned = FALSE;
		m_bARTPaused = FALSE;
		m_bHaveRebuild = FALSE;
		m_bIterateLByL = FALSE;
	}
}

//为角度采样、距离采样设置参数
afx_msg void CARTDlg::OnIdbScanSetting()
{
	CARTDlgScanSetting dlg;
	dlg.m_AngleBegin        = m_AngleBegin*180/PI;
	dlg.m_AngleEnd          = m_AngleEnd*180/PI;
	dlg.m_nAngleQuantity    = m_nAngleQuantity;
	dlg.m_DistanceBegin     = m_DistanceBegin;
	dlg.m_DistanceEnd       = m_DistanceEnd;
	dlg.m_nDistanceQuantity = m_nDistanceQuantity;
	if (dlg.DoModal() == IDOK)
	{
		m_AngleBegin        = dlg.m_AngleBegin*PI/180;
		m_AngleEnd          = dlg.m_AngleEnd*PI/180;
		m_nAngleQuantity    = dlg.m_nAngleQuantity;
		m_DistanceBegin     = dlg.m_DistanceBegin;
		m_DistanceEnd       = dlg.m_DistanceEnd;
		m_nDistanceQuantity = dlg.m_nDistanceQuantity;
		if (m_pRays != NULL)
		{
			delete [] m_pRays;
			m_pRays = NULL;
		}
		m_bImageCreated = FALSE;
		m_bProjected = FALSE;
		m_bHaveScaned = FALSE;
		m_bARTPaused = FALSE;
		m_bHaveRebuild = FALSE;
		m_bIterateLByL = FALSE;
	}
}

void CARTDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case IDB_ARTDLG_SINGLE_LINE:
		if (m_nIterateLine >= 1e8)
		{//迭代终止
			KillTimer(IDB_ARTDLG_SINGLE_LINE);
			m_bHaveRebuild = TRUE;
			m_pBnPause->SetWindowTextW(L"结束");
			m_pBnPause->EnableWindow(FALSE);
			return;
		}
		m_pMyImageCopy->ARTSingleLine(m_nIterateLine++);
		if (m_nIterateLine == m_pMyImageCopy->m_nQuantityOfPixel) m_nIterateLine = 0;
		if (m_nIterateLine % 100 == 0)
		{
			CreateTargetImage(m_pBmpBackProject);
			InvalidateRect(m_pPaintRect);
		}
		break;
	case IDB_ARTDLG_SINGLE_STEP:
		if (m_nIterateStep >= 24)
		{//迭代终止
			KillTimer(IDB_ARTDLG_SINGLE_STEP);
			m_bHaveRebuild = TRUE;
			m_pBnPause->SetWindowTextW(L"结束");
			m_pBnPause->EnableWindow(FALSE);
			return;
		}
		m_pMyImageCopy->ARTSingleStep();
		m_nIterateStep++;
		CreateTargetImage(m_pBmpBackProject);
		InvalidateRect(m_pPaintRect);
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


//
//  *** 其他 ***
//

//创建32位位图
void Create32Bitmap(CBitmap* pBmp, TYPE* pType, int width, int height)
{
	//申请空间用于保存32位位图数据
	DWORD len32 = 4 * width*height;
	BYTE* data32 = new BYTE[len32];
	//复制位图数据
	BYTE* pDesData = data32;
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c <width; c++)
		{
			//拷贝数据
			*pDesData = *(pDesData+1) = *(pDesData+2) = (BYTE)pType[c + (height-1-r) * width];
			pDesData += 4;
		}
	}
	//创建32位位图
	pBmp->DeleteObject();
	pBmp->CreateBitmap(width, height, 1, 32, data32);

	delete[] data32;
}

//基于二维矩阵创建位图
void CreateBitmapUsingMatrix(CBitmap* pBmp, TYPE* pType, int width, int height)
{
	//规定化
	LONG len = width*height;
	TYPE MAX = 0;
	for (LONG i = 0; i<len; i++)
	{
		if (pType[i]>MAX)
		{
			MAX = pType[i];
		}
	}
	if (MAX == 0) return;

	//归一化
	for (int i = 0; i<len; i++)
	{
		pType[i] /= MAX;
		pType[i] *= 255;
		if (pType[i]<0)
		{
			pType[i] = 0;
			continue;
		}
		if (pType[i]>255)
		{
			pType[i] = 255;
			continue;
		}
	}
	Create32Bitmap(pBmp, pType, width, height);
}

//基于 “射线，采样角度，采样间距” 创建一个位图
 void CreateBitmapUsingRays(CBitmap*pBmp, CMyRay* pRays, TYPE MAX, int DistanceQuantity, int AngleQuantity)
{
	//拷贝出来一个副本
	LONG len = AngleQuantity*DistanceQuantity;
	TYPE* pTemp = new TYPE[len], AVG = 0, STD = 0;

	//剔除异常值,归一化
	for (long i = 0; i<len; i++)
	{
		//将异常值置为0
		if (pRays[i].m_dMeasured > MAX) pRays[i].m_dMeasured = 0;
		pTemp[i] = pRays[i].m_dMeasured;
		pTemp[i] /= MAX;
		pTemp[i] *= 255;
		if (pTemp[i]<0)
		{
			pTemp[i] = 0;
			continue;
		}
		if (pTemp[i]>255)
		{
			pTemp[i] = 255;
			continue;
		}
	}
	Create32Bitmap(pBmp, pTemp, DistanceQuantity, AngleQuantity);
	delete [] pTemp;
}