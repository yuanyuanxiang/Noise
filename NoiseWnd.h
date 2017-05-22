#pragma once


// CNoiseWnd

class CCurveDlg;

#define GAUSS_METHOD     "BoxMuller"

class CNoiseWnd : public CWnd
{
	DECLARE_DYNAMIC(CNoiseWnd)

public:
	CNoiseWnd();
	virtual ~CNoiseWnd();

public:// 新增成员与函数
	CBitmap*                    m_pBitmap;  //源位图
	CBitmap*                   m_pBmpShow;  //显示位图
	CRect*                   m_pPaintRect;  //绘图区
	BOOL                 m_bHaveLoadImage;  //已加载图像

	//字体
	CFont*                     m_pAppFont;  //窗口字体
	CString                    m_FontName;  //字体名称
	int                       m_nFontSize;  //字体大小

	//按钮
	CMFCButton*            m_pBnOpenImage;  //打开图像
	CMFCButton*            m_pBnSaveImage;  //保存图像
	CMFCButton*                m_pBnGauss;  //高斯噪声
	CMFCButton*               m_pBnPepper;  //椒盐噪声
	CMFCButton*         m_pBnMedianFilter;  //中值滤波器
	CMFCButton*       m_pBnSAMedianFilter;  //自适应中值滤波器
	CMFCButton*                m_pBnCurve;  //曲线
	CMFCButton*              m_pBnFourier;  //FFT
	CMFCButton*              m_pBnARTDemo;  //模拟CT图像重建
	CMFCButton*             m_pBnSettings;  //设置

	//图像基本信息
	CString                  m_FolderPath;  //图像路径
	CString                    m_FileName;  //图像名称
	BITMAP                      m_bmpInfo;  //图像信息
	int                      m_bytesPixel;  //通道数
	double                  m_AspectRatio;  //宽高比
	
	//m_pBitmap
	long                     m_nRowlenSrc;  //位宽
	long                        m_nlenSrc;  //总位数
	int                       m_nWidthSrc;  //图像宽度
	int                      m_nHeightSrc;  //图像高度
	
	//m_pBmpShow
	long                     m_nRowlenCur;  //位宽
	long                        m_nlenCur;  //总位数
	int                       m_nWidthCur;  //图像宽度
	int                      m_nHeightCur;  //图像高度

	//Gauss Noise
	double                         m_Mean;  //均值
	double                          m_Std;  //方差

	//Pepper & Salt Noise
	double                       m_Pepper;  //胡椒噪声
	double                         m_Salt;  //白盐噪声

	//滤波器
	int                     m_nFilterSize;  //滤波器大小
	int                  m_nMaxFilterSize;  //滤波器最大阶数

	//非模式对话框
	CCurveDlg*                m_pCurveDlg;  //曲线对话框

	void Reshape(int nItemID, double Space, int nNumber);             //重绘控件
	BOOL GaussDistribution();                                          //添加高斯噪声
	BOOL PepperDistribution();                                         //添加椒盐噪声
	BOOL MedianFilter();                                               //中值滤波器
	BOOL SelfAdaptionMedianFilter();                                   //自适应中值滤波
	int FilterProcessA(BYTE* dataSrc, int curChannel, int y, int x, int FilterSize);
	                                                                   //进程A
	int FilterProcessB(int Zmin, int Zmax, int Zmed, int Zxy);         //进程B
	void EnableButtons(BOOL ButtonIsEnable = TRUE);                    //按钮是否有效


	CImage* CreateCImageObj();                                         //创建CImage类对象
	void ZoomBitmap(int widthnew, int heightnew);                      //图像放大
	void Create32Bitmap(BYTE* dataSrc, int widthnew, int heightnew);   //创建32位位图
	void Gray2Rgb(BYTE* dataSrc, int widthnew, int heightnew);         //灰度图转RGB
	void Synchronous();                                                //两个位图同步
	void FitScreen();                                                  //使位图适应屏幕

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIdbOpenImage();
	afx_msg void OnIdbSaveImage();
	afx_msg void OnIdbGaussDistribution();
	afx_msg void OnIdbPepperDistribution();
	afx_msg void OnIdbSelfAdaptionMedianFilter();
	afx_msg void OnIddCurveDlg();
	afx_msg void OnIdbMedianFilter();
	afx_msg void OnIdbSettings();
	afx_msg void OnIdbFourier();
	afx_msg void OnIdbArtdemo();
};

double GaussNoise(double nMean = 0, double nStd = 1, char* Method = "BoxMuller");                          //产生高斯噪声
int PepperNoise(double Probability = 0.05, char* PepperOrSalt = "Pepper");                                 //产生椒盐噪声