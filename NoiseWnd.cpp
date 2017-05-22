// NoiseWnd.cpp : 实现文件
//

// CNoiseWnd

#include "stdafx.h"
#include "Noise.h"
#include "NoiseWnd.h"

#include "SpaceFilter.h"
#include "CurveDlg.h"
#include "Settings.h"

#include "FFTDlg.h"
#include "ARTDlg.h"

IMPLEMENT_DYNAMIC(CNoiseWnd, CWnd)

CNoiseWnd::CNoiseWnd()
{
	//位图、绘图区
	m_pBitmap = NULL;
	m_pBmpShow = NULL;
	m_pPaintRect = NULL;

	//按钮
	m_pBnOpenImage = NULL;
	m_pBnSaveImage = NULL;
	m_pBnGauss = NULL;
	m_pBnPepper = NULL;
	m_pBnMedianFilter = NULL;
	m_pBnSAMedianFilter = NULL;
	m_pBnCurve = NULL;
	m_pBnFourier = NULL;
	m_pBnARTDemo = NULL;
	m_pBnSettings = NULL;

	//图像信息
	m_FolderPath = _T("");
	m_FileName = _T("");
	/*m_bmpInfo*/
	m_bytesPixel = 3;
	m_AspectRatio = 1.0;

	m_nWidthCur = m_nWidthSrc = 0;
	m_nHeightCur = m_nHeightSrc = 0;
	m_nRowlenCur = m_nRowlenSrc = 0;
	m_nlenCur = m_nlenSrc = 0;

	//字体
	m_pAppFont = NULL;
	m_FontName = _T("Arial");
	m_nFontSize = 90;

	//非模式对话框
	m_pCurveDlg = NULL;

	//其他
	m_bHaveLoadImage = FALSE;
	m_Mean = 8;
	m_Std = 24;
	m_Pepper = 0.01;
	m_Salt = 0.01;
	m_nFilterSize = 3;
	m_nMaxFilterSize = 7;
}

CNoiseWnd::~CNoiseWnd()
{
	if (m_pBitmap != NULL)            {	delete m_pBitmap;}
	if (m_pBmpShow != NULL)           {	delete m_pBmpShow;}
	if (m_pPaintRect != NULL)         {	delete m_pPaintRect;}

	if (m_pAppFont != NULL)           {	delete m_pAppFont;}

	if (m_pBnOpenImage != NULL)       {	delete m_pBnOpenImage;}
	if (m_pBnSaveImage != NULL)       {	delete m_pBnSaveImage;}
	if (m_pBnGauss != NULL)           {	delete m_pBnGauss;}
	if (m_pBnPepper != NULL)          {	delete m_pBnPepper;}
	if (m_pBnMedianFilter != NULL)    {	delete m_pBnMedianFilter;}
	if (m_pBnSAMedianFilter != NULL)  {	delete m_pBnSAMedianFilter;}
	if (m_pBnCurve != NULL)           { delete m_pBnCurve;}
	if (m_pCurveDlg != NULL)          {	delete m_pCurveDlg;}
	if (m_pBnFourier != NULL)         {	delete m_pBnFourier;}
	if (m_pBnARTDemo != NULL)         {	delete m_pBnARTDemo;}
	if (m_pBnSettings != NULL)        { delete m_pBnSettings;}
}


BEGIN_MESSAGE_MAP(CNoiseWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDB_OPEN_IMAGE, &CNoiseWnd::OnIdbOpenImage)
	ON_BN_CLICKED(IDB_SAVE_IMAGE, &CNoiseWnd::OnIdbSaveImage)
	ON_BN_CLICKED(IDB_GAUSS, &CNoiseWnd::OnIdbGaussDistribution)
	ON_BN_CLICKED(IDB_PEPPER, &CNoiseWnd::OnIdbPepperDistribution)
	ON_BN_CLICKED(IDB_MEDIAN_FILTER, &CNoiseWnd::OnIdbMedianFilter)
	ON_BN_CLICKED(IDB_SA_MEDIAN_FILTER, &CNoiseWnd::OnIdbSelfAdaptionMedianFilter)
	ON_BN_CLICKED(IDB_CURVE_BUTTON, &CNoiseWnd::OnIddCurveDlg)
	ON_BN_CLICKED(IDB_SETTINGS, &CNoiseWnd::OnIdbSettings)
	ON_BN_CLICKED(IDB_FOURIER, &CNoiseWnd::OnIdbFourier)
	ON_BN_CLICKED(IDB_ARTDEMO, &CNoiseWnd::OnIdbArtdemo)
END_MESSAGE_MAP()



// CNoiseWnd 消息处理程序




void CNoiseWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_bHaveLoadImage == FALSE) return;
	if (m_pPaintRect->Width() <= 0 || m_pPaintRect->Height() <= 0) return;

	FitScreen();

	int x = m_pPaintRect->Width()-m_nWidthCur;
	int y = m_pPaintRect->Height()-m_nHeightCur;

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SelectObject(m_pBmpShow);
	dc.BitBlt(m_pPaintRect->left+0.5*x, m_pPaintRect->top+0.5*y, m_nWidthCur, m_nHeightCur, &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
}

int CNoiseWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pBitmap = new CBitmap;
	m_pBmpShow = new CBitmap;

	CRect rect;
	GetClientRect(&rect);
	int nWindowHeight = rect.Height();
	double Space = nWindowHeight/11.0;

	m_pPaintRect = new CRect(100, 25, int(rect.right-25), int(0.8*rect.bottom-25));
	
	m_pAppFont = new CFont;
	m_pAppFont->CreatePointFont(m_nFontSize, m_FontName);

	m_pBnOpenImage = new CMFCButton;
	m_pBnOpenImage->Create(_T("打开"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(Space-12), 75, int(Space+12)), this, IDB_OPEN_IMAGE);
	m_pBnOpenImage->SetFont(m_pAppFont, TRUE);               //设置按钮文字字体
	m_pBnOpenImage->SetTooltip(_T("打开一张图片"));          //设置按钮提示信息
	m_pBnOpenImage->SetMouseCursorHand();                    //鼠标经过时变手势

	m_pBnSaveImage = new CMFCButton;
	m_pBnSaveImage->Create(_T("保存"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(2*Space-12), 75, int(2*Space+12)), this, IDB_SAVE_IMAGE);
	m_pBnSaveImage->SetFont(m_pAppFont, TRUE);
	m_pBnSaveImage->SetTooltip(_T("保存当前图片"));
	m_pBnSaveImage->SetMouseCursorHand();
	m_pBnSaveImage->EnableWindow(FALSE);

	m_pBnGauss = new CMFCButton;
	m_pBnGauss->Create(_T("高斯"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(3*Space-12), 75, int(3*Space+12)), this, IDB_GAUSS);
	m_pBnGauss->SetFont(m_pAppFont, TRUE);
	m_pBnGauss->SetTooltip(_T("添加高斯噪声"));
	m_pBnGauss->SetMouseCursorHand();
	m_pBnGauss->EnableWindow(FALSE);

	m_pBnPepper = new CMFCButton;
	m_pBnPepper->Create(_T("椒盐"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(4*Space-12), 75, int(4*Space+12)), this, IDB_PEPPER);
	m_pBnPepper->SetFont(m_pAppFont, TRUE);
	m_pBnPepper->SetTooltip(_T("添加椒盐噪声"));
	m_pBnPepper->SetMouseCursorHand();
	m_pBnPepper->EnableWindow(FALSE);

	m_pBnMedianFilter = new CMFCButton;
	m_pBnMedianFilter->Create(_T("中值"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(5*Space-12), 75, int(5*Space+12)), this, IDB_MEDIAN_FILTER);
	m_pBnMedianFilter->SetFont(m_pAppFont, TRUE);
	m_pBnMedianFilter->SetTooltip(_T("中值滤波"));
	m_pBnMedianFilter->SetMouseCursorHand();
	m_pBnMedianFilter->EnableWindow(FALSE);

	m_pBnSAMedianFilter = new CMFCButton;
	m_pBnSAMedianFilter->Create(_T("自适应"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(6*Space-12), 75, int(6*Space+12)), this, IDB_SA_MEDIAN_FILTER);
	m_pBnSAMedianFilter->SetFont(m_pAppFont, TRUE);
	m_pBnSAMedianFilter->SetTooltip(_T("自适应中值滤波"));
	m_pBnSAMedianFilter->SetMouseCursorHand();
	m_pBnSAMedianFilter->EnableWindow(FALSE);

	m_pBnCurve = new CMFCButton;
	m_pBnCurve->Create(_T("曲线"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(7*Space-12), 75, int(7*Space+12)), this, IDB_CURVE_BUTTON);
	m_pBnCurve->SetFont(m_pAppFont, TRUE);
	m_pBnCurve->SetTooltip(_T("曲线灰度变换"));
	m_pBnCurve->SetMouseCursorHand();
	m_pBnCurve->EnableWindow(FALSE);

	m_pBnFourier = new CMFCButton;
	m_pBnFourier->Create(_T("FFT"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(8*Space-12), 75, int(8*Space+12)), this, IDB_FOURIER);
	m_pBnFourier->SetFont(m_pAppFont, TRUE);
	m_pBnFourier->SetTooltip(_T("快速傅里叶变换"));
	m_pBnFourier->SetMouseCursorHand();
	m_pBnFourier->EnableWindow(FALSE);
	
	m_pBnARTDemo = new CMFCButton;
	m_pBnARTDemo->Create(_T("ART"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(9*Space-12), 75, int(9*Space+12)), this, IDB_ARTDEMO);
	m_pBnARTDemo->SetFont(m_pAppFont, TRUE);
	m_pBnARTDemo->SetTooltip(_T("模拟ART重建图像"));
	m_pBnARTDemo->SetMouseCursorHand();

	m_pBnSettings = new CMFCButton;
	m_pBnSettings->Create(_T("设置"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(25, int(10*Space-12), 75, int(10*Space+12)), this, IDB_SETTINGS);
	m_pBnSettings->SetFont(m_pAppFont, TRUE);
	m_pBnSettings->SetTooltip(_T("设置程序参数"));
	m_pBnSettings->SetMouseCursorHand();

	return 0;
}

void CNoiseWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	CRect rect;
	GetClientRect(&rect);
	m_pPaintRect->right = rect.right-25;
	m_pPaintRect->bottom = rect.bottom-25;

	int nWindowHeight = rect.Height();
	double Space = nWindowHeight/11.0;
	Reshape(IDB_OPEN_IMAGE, Space, 1);
	Reshape(IDB_SAVE_IMAGE, Space, 2);
	Reshape(IDB_GAUSS, Space, 3);
	Reshape(IDB_PEPPER, Space, 4);
	Reshape(IDB_MEDIAN_FILTER, Space, 5);
	Reshape(IDB_SA_MEDIAN_FILTER, Space, 6);
	Reshape(IDB_CURVE_BUTTON, Space, 7);
	Reshape(IDB_FOURIER, Space, 8);
	Reshape(IDB_ARTDEMO, Space, 9);
	Reshape(IDB_SETTINGS, Space, 10);
	FitScreen();
}

void CNoiseWnd::FitScreen()
{
	if (m_pPaintRect->Width() <= 0 || m_pPaintRect->Height() <= 0) return;
	int x = m_pPaintRect->Width()-m_nWidthSrc;
	int y = m_pPaintRect->Height()-m_nHeightSrc;
	if(x<0 && y<0)
	{
		ZoomBitmap(m_pPaintRect->Width(), m_pPaintRect->Height());
	}
	else if(x>=0 && y<0)
	{
		ZoomBitmap(m_pPaintRect->Height()*m_AspectRatio, m_pPaintRect->Height());
	}
	else if(x<0 && y>=0)
	{
		ZoomBitmap(m_pPaintRect->Width(), m_pPaintRect->Width()/m_AspectRatio);
	}
	else if(m_nWidthSrc != m_nWidthCur && m_nHeightSrc != m_nHeightCur)
	{
		ZoomBitmap(m_nWidthSrc, m_nHeightSrc);
	}
}

void CNoiseWnd::Reshape(int nItemID, double Space, int nNumber)
{
	CWnd *hWnd = GetDlgItem(nItemID);
	if (hWnd != NULL)
		hWnd->MoveWindow(25, int(nNumber*Space-12), 50, 25);
}

//添加高斯噪声
BOOL CNoiseWnd::GaussDistribution()
{
	if (m_bHaveLoadImage == FALSE) return FALSE;
	EnableButtons(FALSE);

	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);

	time_t t;
	srand((unsigned)time(&t));
	for (int i = 0, Temp; i<m_nHeightSrc; i++)
	{
		for (int j = 0; j<m_nWidthSrc; j++)
		{
			BYTE* pixel = dataSrc + j*m_bytesPixel + i*m_nRowlenSrc;
			for (int k = 0; k<m_bytesPixel; k++)
			{
				Temp = GaussNoise(m_Mean, m_Std, GAUSS_METHOD);
				*(pixel+k) = *(pixel+k)+Temp;
			}
		}
	}
	m_pBitmap->SetBitmapBits(m_nlenSrc, dataSrc);
	delete [] dataSrc;
	return TRUE;
}

//添加椒盐噪声
BOOL CNoiseWnd::PepperDistribution()
{
	if (m_bHaveLoadImage == FALSE) return FALSE;
	EnableButtons(FALSE);

	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);

	time_t t;
	srand((unsigned)time(&t));
	//胡椒
	for (int i = 0, Temp; i<m_nHeightSrc; i++)
	{
		for (int j = 0; j<m_nWidthSrc; j++)
		{
			BYTE* pixel = dataSrc + j*m_bytesPixel + i*m_nRowlenSrc;
			Temp = PepperNoise(m_Pepper, "Pepper");
			if (Temp != -1)
			{
				for (int k = 0; k<m_bytesPixel; k++)
				{
					*(pixel+k) = Temp;
				}
			}
		}
	}
	//白盐
	for (int i = 0, Temp; i<m_nHeightSrc; i++)
	{
		for (int j = 0; j<m_nWidthSrc; j++)
		{
			BYTE* pixel = dataSrc + j*m_bytesPixel + i*m_nRowlenSrc;
			Temp = PepperNoise(m_Salt, "Salt");
			if (Temp != -1)
			{
				for (int k = 0; k<m_bytesPixel; k++)
				{
					*(pixel+k) = Temp;
				}
			}
		}
	}
	m_pBitmap->SetBitmapBits(m_nlenSrc, dataSrc);
	delete [] dataSrc;
	return TRUE;
}

BOOL CNoiseWnd::MedianFilter()
{
	if (m_bHaveLoadImage == FALSE) return FALSE;
	EnableButtons(FALSE);

	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);

	int* Temp = new int[m_bytesPixel];
	for(int y = 0; y<m_nHeightSrc; y++)
	{
		for(int x = 0; x<m_nWidthSrc; x++)
		{
			for (int k = 0; k<m_bytesPixel; k++)
			{
				CSpaceFilter MyMedFilter;
				MyMedFilter.Create(dataSrc, m_bmpInfo, k, y, x, m_nFilterSize);
				Temp[k] = MyMedFilter.FindMed();
			}
			BYTE* pixel = dataSrc + x*m_bytesPixel + y*m_nRowlenSrc;
			for (int k = 0; k<m_bytesPixel; k++)
			{
				*(pixel+k) = (BYTE)Temp[k];
			}
		}
	}
	m_pBitmap->SetBitmapBits(m_nlenSrc, dataSrc);
	delete [] Temp;
	delete [] dataSrc;
	return TRUE;
}

//自适应中值滤波
BOOL CNoiseWnd::SelfAdaptionMedianFilter()
{
	if (m_bHaveLoadImage == FALSE) return FALSE;
	EnableButtons(FALSE);

	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);

    int* Temp = new int[m_bytesPixel];
	for(int y = 0; y<m_nHeightSrc; y++)
	{
		for(int x = 0, flag; x<m_nWidthSrc; x++)
		{
			flag = 0;
			for (int k = 0; k<m_bytesPixel; k++)
			{
				Temp[k] = FilterProcessA(dataSrc, k, y, x, m_nFilterSize);
				if (Temp[k] == -1)
				{//有一个通道构造滤波器失败
					flag = 1;
					break;
				}
			}
			if (flag == 0)
			{
				BYTE* pixel = dataSrc + x*m_bytesPixel + y*m_nRowlenSrc;
				for (int k = 0; k<m_bytesPixel; k++)
				{
					*(pixel+k) = (BYTE)Temp[k];
				}
			}
		}
	}
	m_pBitmap->SetBitmapBits(m_nlenSrc, dataSrc);
	delete [] Temp;
	delete [] dataSrc;
	return TRUE;
}

//进程A：dataSrc-原图像数据，curChannel-通道，y、x-坐标，FilterSize-滤波器初始大小
int CNoiseWnd::FilterProcessA(BYTE* dataSrc, int curChannel, int y, int x, int FilterSize)
{
	CSpaceFilter MyMedFilter;
	if(MyMedFilter.Create(dataSrc, m_bmpInfo, curChannel, y, x, FilterSize) == FALSE)
		return -1;

	int Zmin = MyMedFilter.FindMin();
	int Zmax = MyMedFilter.FindMax();
	int Zmed = MyMedFilter.FindMed();
	int Zxy = *(dataSrc + x*m_bytesPixel+curChannel + y*m_nRowlenSrc);
	if (Zmed-Zmin>0 && Zmed-Zmax<0)
	{//转进程B
		return FilterProcessB(Zmin, Zmax, Zmed, Zxy);
	}
	else
	{//增大窗口尺寸
		if (FilterSize<m_nMaxFilterSize) return FilterProcessA(dataSrc, curChannel, y, x, FilterSize+1);
		else                  return Zmed;
	}
}

int CNoiseWnd::FilterProcessB(int Zmin, int Zmax, int Zmed, int Zxy)
{
	if (Zxy-Zmin>0 && Zxy-Zmax<0) return Zxy;
	else                          return Zmed;
}

void CNoiseWnd::EnableButtons(BOOL ButtonIsEnable)
{
	m_pBnOpenImage->EnableWindow(ButtonIsEnable);
	m_pBnSaveImage->EnableWindow(ButtonIsEnable);
	m_pBnGauss->EnableWindow(ButtonIsEnable);
	m_pBnPepper->EnableWindow(ButtonIsEnable);
	m_pBnMedianFilter->EnableWindow(ButtonIsEnable);
	m_pBnSAMedianFilter->EnableWindow(ButtonIsEnable);
	m_pBnCurve->EnableWindow(ButtonIsEnable);
}

//创建一个当前图像的副本图像:CImage类对象
CImage* CNoiseWnd::CreateCImageObj()
{
	CImage* image = new CImage;
	image->Create(m_nWidthSrc, m_nHeightSrc, 24);

	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);

	for (int y = 0; y<m_nHeightSrc; y++)
	{
		for (int x = 0; x<m_nWidthSrc; x++)
		{
			BYTE* pixel = dataSrc + x*m_bytesPixel + y*m_nRowlenSrc;
			image->SetPixelRGB(x, y, *(pixel+2), *(pixel+1), *pixel);
		}
	}
	delete [] dataSrc;
	return image;
}

void CNoiseWnd::ZoomBitmap(int widthnew, int heightnew)
{
	//获取原图像尺寸数据
	long width = m_nWidthSrc;
	long height = m_nHeightSrc;
	long byteswidth = m_nRowlenSrc;

	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);

	//设置缩放后的图像尺寸数据
	m_nWidthCur = widthnew;
	m_nHeightCur = heightnew;
	m_nRowlenCur = m_nWidthCur * m_bytesPixel;
	m_nlenCur = m_nHeightCur * m_nRowlenCur;
	//计算缩放比例
	float ratewidth = 1.0f * m_nWidthCur / width;
	float rateheight = 1.0f * m_nHeightCur/ height;
	//申请空间用于保存目标图像数据及中间结果
	BYTE* dataStep1 = new BYTE[height * m_nRowlenCur];
	BYTE* dataDes = new BYTE[m_nlenCur];

	//双二次插值法
	int r = 0, c = 0, k = 0;
	//先行向插值
	for(r = 0; r < height; r++)
	{
		BYTE* pDes = dataStep1 + r * m_nRowlenCur;
		for(c = 0; c < m_nWidthCur; c++)
		{
			int col = (int)(c / ratewidth);
			float fRateX = c / ratewidth - col;
			BYTE* pSrc = dataSrc + r * byteswidth + col * m_bytesPixel;
			if (col < width-1)
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)(*(pSrc+k) * (1-fRateX) + *(pSrc+m_bytesPixel+k) * fRateX);
				}
			}
			else
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)*(pSrc+k);
				}
			}
			pDes += m_bytesPixel;
		}
	}
	//再纵向插值
	for (c = 0; c < m_nWidthCur; c++)
	{
		BYTE* pDes = dataDes + c * m_bytesPixel;
		for (r = 0; r < m_nHeightCur; r++)
		{
			int row = (int)(r / rateheight);
			float fRateY = r / rateheight - row;
			BYTE* pSrc = dataStep1 + row * m_nRowlenCur + c * m_bytesPixel;
			if(row < height-1)
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)(*(pSrc+k) * (1-fRateY) + *(pSrc+m_nRowlenCur+k) * fRateY);
				}
			}
			else
			{
				for (k = 0; k<m_bytesPixel; k++)
				{
					*(pDes+k) = (BYTE)*(pSrc+k);
				}
			}
			pDes += m_nRowlenCur;
		}
	}
	Create32Bitmap(dataDes, widthnew, heightnew);
	delete [] dataSrc;
	delete [] dataStep1;
	delete [] dataDes;
}

//创建32位位图:dataSrc - 数据源,widthnew,height - 新的宽与高
void CNoiseWnd::Create32Bitmap(BYTE* dataSrc, int widthnew, int heightnew)
{
	//申请空间用于保存32位位图数据
	DWORD len32 = 4 * widthnew * heightnew;
	BYTE* data32 = new BYTE[len32];
	//复制位图数据
	BYTE* pSrcData = dataSrc;
	BYTE* pDesData = data32;
	for(int r = 0; r < heightnew; r++)
	{
		pSrcData = dataSrc + r * m_nRowlenCur;
		for(int c = 0; c <widthnew; c++)
		{
			memcpy(pDesData, pSrcData, m_bytesPixel);
			pSrcData += m_bytesPixel;
			pDesData += 4;
		}
	}
	//创建32位位图
	m_pBmpShow->DeleteObject();
	m_pBmpShow->CreateBitmap(widthnew, heightnew, 1, 32, data32);

	delete[] data32;
}

void CNoiseWnd::Gray2Rgb(BYTE* dataSrc, int widthnew, int heightnew)
{
	//申请空间用于保存24位位图数据
	DWORD len24 = 3 * widthnew * heightnew;
	BYTE* data24 = new BYTE[len24];
	//复制位图数据
	BYTE* pSrcData = dataSrc;
	BYTE* pDesData = data24;
	for(int r = 0; r < heightnew; r++)
	{
		pSrcData = dataSrc + r * m_nRowlenCur;
		for(int c = 0; c <widthnew; c++)
		{
			*(pDesData)   = *pSrcData;
			*(pDesData+1) = *pSrcData;
			*(pDesData+2) = *pSrcData;
			pSrcData += m_bytesPixel;
			pDesData += 3;
		}
	}
	//创建24位位图
	m_pBitmap->DeleteObject();
	m_pBitmap->CreateBitmap(widthnew, heightnew, 1, 24, data24);
	//修改图像信息
	m_pBitmap->GetBitmap(&m_bmpInfo);
	m_bytesPixel = m_bmpInfo.bmBitsPixel/8;

	m_nRowlenCur = m_nRowlenSrc = m_bmpInfo.bmWidthBytes;
	m_nlenCur = m_nlenSrc = m_nHeightSrc * m_nRowlenCur;

	delete[] data24;
}

void CNoiseWnd::Synchronous()
{
	BYTE* dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);
	Create32Bitmap(dataSrc, m_nWidthSrc, m_nHeightSrc);
	InvalidateRect(m_pPaintRect);
	delete [] dataSrc;
}

//打开图像
void CNoiseWnd::OnIdbOpenImage()
{
	// TODO: 在此添加命令处理程序代码
	if (m_bHaveLoadImage == TRUE)
	{//如果已经载入图像，再次载入时需先释放内存
		m_bHaveLoadImage = FALSE;
		delete m_pBitmap;
		delete m_pBmpShow;
		m_pBitmap = new CBitmap;
		m_pBmpShow = new CBitmap;
	}
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY,
		TEXT("所有文件(*.*)|*.* | 图像文件(*.jpg;*.gif;*.png;*.tif;*.bmp)|*.jpg;*.gif;*.png;*.tif;*.bmp||"), NULL);
	hFileDlg.m_ofn.nFilterIndex = 2;
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("打开图像文件...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;

	if(hFileDlg.DoModal() != IDOK) return;

	m_FolderPath = hFileDlg.GetFolderPath();
	m_FileName = hFileDlg.GetFileName();
	CString FilePath = hFileDlg.GetPathName();
	CImage image;
	image.Load(FilePath);
	if(image.IsNull())
	{
		AfxMessageBox(_T("加载文件时出错!"));
		return;
	}
	//启用按钮
	m_pBnSaveImage->EnableWindow(TRUE);
	m_pBnGauss->EnableWindow(TRUE);
	m_pBnPepper->EnableWindow(TRUE);
	m_pBnMedianFilter->EnableWindow(TRUE);;
	m_pBnSAMedianFilter->EnableWindow(TRUE);
	m_pBnCurve->EnableWindow(TRUE);
	m_pBnFourier->EnableWindow(TRUE);
	//获取位图
	m_bHaveLoadImage = TRUE;
	HBITMAP hBitmap = image.Detach();
	m_pBitmap->Attach(hBitmap);
	//图像信息
	m_pBitmap->GetBitmap(&m_bmpInfo);
	m_bytesPixel = m_bmpInfo.bmBitsPixel/8;
	m_AspectRatio = 1.0*m_bmpInfo.bmWidth/m_bmpInfo.bmHeight;

	m_nWidthCur  = m_nWidthSrc  = m_bmpInfo.bmWidth;
	m_nHeightCur = m_nHeightSrc = m_bmpInfo.bmHeight;
	m_nRowlenCur = m_nRowlenSrc = m_bmpInfo.bmWidthBytes;
	m_nlenCur    = m_nlenSrc    = m_nHeightSrc * m_nRowlenCur;
	
	//转换灰度图像
	if (m_bytesPixel < 3)
	{
		BYTE *dataSrc = new BYTE[m_nlenSrc];
		m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);
		Gray2Rgb(dataSrc, m_nWidthSrc, m_nHeightSrc);
		delete [] dataSrc;
		dataSrc = NULL;
	}

	//将m_pBitmap拷贝给m_pBmpShow
	BYTE *dataSrc = new BYTE[m_nlenSrc];
	m_pBitmap->GetBitmapBits(m_nlenSrc, dataSrc);
	Create32Bitmap(dataSrc, m_nWidthSrc, m_nHeightSrc);
	//m_pBmpShow->CreateBitmap(m_nWidthSrc, m_nHeightSrc, 1, 24, dataSrc);
	m_pBitmap->SetBitmapBits(m_nlenSrc, dataSrc);
	delete[]dataSrc;
	dataSrc = NULL;

	FitScreen();

	InvalidateRect(m_pPaintRect);
	UpdateWindow();
}

//保存图像
void CNoiseWnd::OnIdbSaveImage()
{
	// TODO: 在此添加命令处理程序代码
	if (!m_bHaveLoadImage) return;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString CurTime;
	CurTime.Format(_T("%04d%02d%02d%02d%02d%02d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	CString strFileName = _T("");
	CString strExtension = _T("");

	CFileDialog dlg(FALSE, NULL, m_FileName.Left(m_FileName.Find(_T("."))) + CurTime, NULL, 
		TEXT("BMP 位图文件(*.bmp)|*.bmp|JPEG 图像文件(*.jpg)|*.jpg|PNG 图像文件(*.png)|*.png|TIF 文件格式(*.tif)|*.tif|GIF 图像文件(*.gif)|*.gif||"), NULL);
	dlg.m_ofn.hwndOwner = m_hWnd;
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	dlg.m_ofn.lpstrTitle = TEXT("保存图像文件...\0");
	dlg.m_ofn.nMaxFile = MAX_PATH;

	if ( dlg.DoModal() != IDOK) return;

	strFileName += dlg.GetPathName();

	switch (dlg.m_ofn.nFilterIndex)
	{
	case 1 : strExtension = _T("bmp"); break;
	case 2 : strExtension = _T("jpg"); break;
	case 3 : strExtension = _T("png"); break;
	case 4 : strExtension = _T("tif"); break;
	case 5 : strExtension = _T("gif"); break;
	default:                           break;
	}
	if (strFileName.Right(4) != _T(".bmp") && strFileName.Right(4) != _T(".jpg") &&
		strFileName.Right(4) != _T(".png") && strFileName.Right(4) != _T(".tif") && strFileName.Right(4) != _T(".gif"))
	{
		strFileName = strFileName + '.' + strExtension;
	}

	//创建一份m_pBitmap的副本
	CImage* image = CreateCImageObj();
	HRESULT hr = image->Save(strFileName); 
	if (hr == S_OK)	MessageBox(_T("内容已成功保存到\n"+strFileName), _T("保存结果"), NULL);
	else            MessageBox(_T("保存图片失败！"));

	delete image;
}

//按钮的响应
void CNoiseWnd::OnIdbGaussDistribution()
{
	// TODO: 在此添加命令处理程序代码
	if(!GaussDistribution()) return;
	EnableButtons(TRUE);
	Synchronous();
}

//按钮的响应
void CNoiseWnd::OnIdbPepperDistribution()
{
	// TODO: 在此添加命令处理程序代码
	if(!PepperDistribution()) return;
	EnableButtons(TRUE);
	Synchronous();
}

void CNoiseWnd::OnIdbMedianFilter()
{
	// TODO: 在此添加命令处理程序代码
	if (!MedianFilter()) return;
	EnableButtons(TRUE);
	Synchronous();
}

//按钮的响应
void CNoiseWnd::OnIdbSelfAdaptionMedianFilter()
{
	// TODO: 在此添加命令处理程序代码
	if (!SelfAdaptionMedianFilter()) return;
	EnableButtons(TRUE);
	Synchronous();
}

//
// *** 其他函数 ***
//

/************************************************************************/
/*  对于正态分布，利用反函数的方法来获取正态分布序列显然是很麻烦的，牵涉到
    很复杂的积分微分运算，同时为了方便，我们取，即标准正态分布。因此这里介
	绍了两种算法：
	第一种：
	    Box和Muller在1958年给出了由均匀分布的随机变量生成正态分布的随机变量
	的算法。设U1, U2是区间 (0, 1)上均匀分布的随机变量，且相互独立。令  
	X1=sqrt(-2*log(U1)) * cos(2*PI*U2); 
	X2=sqrt(-2*log(U1)) * sin(2*PI*U2);  
	那么X1, X2服从N(0,1)分布，且相互独立。
	p = rand()%100;//产生从0-32767的任意一个值
	b[j] = double(p)/double(100);
	a[j] = sqrt(-2*log(a[j]))*cos(2*3.1415926*b[j]);
	第二种：
	    近似生成标准正态分布，独立同分布的多个随机变量和的分布趋近于正态
	分布,取k个均匀分布的(0,1)随机变量，则它们的和近似服从正态分布。实践中,
	取k=12,(因为D(xi)=1/12),则新的随机变量y=x1+x2+...+x12-0.5*12，可以求出
	数学期望E(y)=0,方差D(y)=12*1/12=1，因此可以近似描述标准正态分布。 */
/************************************************************************/

//nMean-高斯分布的均值, nStd-高斯分布的标准差,Method:使用BoxMuller方法或CentralLimitTheorem方法
double GaussNoise(double nMean, double nStd, char* Method)
{
	if (Method == "Central limit theorem")
	{
		double sum = 0; 
		for(int i = 0; i<12; i++) sum += (double)rand()/RAND_MAX;
		//计算机中rand()函数为0~32767（2^15-1）
		//故sum＋为0～12之间的均匀随机变量
		return nMean+nStd*(sum-6);
	}
	else if(Method == "BoxMuller")
	{
		return nMean+nStd*(sqrt(-2*log((double)rand()/RAND_MAX))*cos(2*3.1415926*(double)rand()/RAND_MAX));
	}
	return -1;
}

//产生椒盐噪声:Probability是概率, PepperOrSalt - "Pepper" 或者 "Salt"
int PepperNoise(double Probability, char* PepperOrSalt)
{
	double p = (double)rand()/RAND_MAX;
	if (PepperOrSalt == "Pepper" && 1-Probability<=p && p<=1)
		return 0;
	if (PepperOrSalt == "Salt" && 1-Probability<=p && p<=1)
		return 255;
	return -1;
}

//弹出曲线对话框
void CNoiseWnd::OnIddCurveDlg()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pCurveDlg != NULL) delete m_pCurveDlg;
	m_pCurveDlg = new CCurveDlg;
	m_pCurveDlg->Create(IDD_CURVE_DLG);
	m_pCurveDlg->ShowWindow(SW_RESTORE);
}

void CNoiseWnd::OnIdbFourier()
{
	// TODO: 在此添加命令处理程序代码
	CFFTDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
}

void CNoiseWnd::OnIdbArtdemo()
{
	CARTDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
	}
}

//弹出参数设置对话框
void CNoiseWnd::OnIdbSettings()
{
	// TODO: 在此添加命令处理程序代码
	CSettings dlg;
	dlg.m_Mean           = m_Mean;
	dlg.m_Std            = m_Std;
	dlg.m_Pepper         = m_Pepper;
	dlg.m_Salt           = m_Salt;
	dlg.m_nFilterSize    = m_nFilterSize;
	dlg.m_nMaxFilterSize = m_nMaxFilterSize;
	if (dlg.DoModal() == IDOK)
	{
		m_Mean           = dlg.m_Mean;
		m_Std            = dlg.m_Std;
		m_Pepper         = dlg.m_Pepper;
		m_Salt           = dlg.m_Salt;
		m_nFilterSize    = dlg.m_nFilterSize;
		m_nMaxFilterSize = dlg.m_nMaxFilterSize;
	}
}