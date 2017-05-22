
#pragma once


// CFFT 对话框

#include "NoiseWnd.h"
#include <cmath>
#include <complex>
using namespace std;


#define HDIB                         HANDLE 
#define PI                           3.141592653
#define WIDTHBYTES(bits)             (((bits) + 31) / 32 * 4)

//按钮资源
#define IDB_FFTDLG_OK                201
#define IDB_FFTDLG_CANCEL            202
#define IDB_FFTDLG_FFT               203
#define IDB_FFTDLG_FILTER            204
#define IDB_FFTDLG_INVFFT            205

#define FILTER_BUTTERWORTHL 1    // - 巴特沃斯低通滤波器
#define FILTER_BUTTERWORTHH 2    // - 巴特沃斯高通滤波器
#define FILTER_RESTRORSE    3    // - 巴特沃斯低通滤波器的逆滤波
#define FILTER_WIENER       4    // - 有约束恢复的维纳滤波
#define FILTER_PSE          5    // - 有约束恢复的功率谱均衡滤波
#define FILTER_MINPOWER     6    // - 有约束恢复的最小平方滤波

class CFFTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFFTDlg)

public:
	CFFTDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFFTDlg();

// 对话框数据
	enum { IDD = IDD_FFT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public://新增成员与函数
	CNoiseWnd*                        m_pNoiseWnd;  //指针
	CBitmap*                            m_pBitmap;  //指针
	CBitmap*                              m_pFreq;  //FFT频谱
	CRect*                           m_pPaintRect;  //绘图区
	BYTE*                               m_dataSrc;  //数据源
	BYTE*                             m_data24Bit;  //除掉alpha通道剩下的数据
	int                              m_bytesPixel;  //通道
	int                             m_bmBitsPixel;  //位数

	//当前图像信息
	int                               m_nWidthCur;  //宽
	int                              m_nHeightCur;  //高
	LONG                             m_nRowlenCur;  //位宽
	LONG                                m_nlenCur;  //长度

	//源图像信息
	int                               m_nWidthSrc;  //宽
	int                              m_nHeightSrc;  //高
	LONG                             m_nRowlenSrc;  //位宽
	LONG                                m_nlenSrc;  //长度

	//24位图像信息
	int                             m_nWidth24Bit;  //宽
	int                            m_nHeight24Bit;  //高
	LONG                           m_nRowlen24Bit;  //位宽
	LONG                              m_nlen24Bit;  //长度

	//按钮
	CMFCButton*                           m_pBnOK;  //确定
	CMFCButton*                       m_pBnCancel;  //取消
	CMFCButton*                          m_pBnFFT;  //傅里叶变换
	CMFCButton*                       m_pBnFilter;  //滤波

	//滤波方法及谱
	int                                 m_nMethod;
	float                                m_fPower;
	
	CNoiseWnd* GetDocument();                       //获取主文档
	CBitmap* GetBitmap();                           //获取位图指针
	void GetBitmapInfo();                           //获取源位图信息
	void GetBitmapData();                           //获取源位图数据
	void Get24BitData();                            //获取源位图24位数据
	void Create32Bitmap(BYTE* dataSrc, int channel, int widthnew, int heightnew);
	                                                //创建32位图像
	void ZoomBitmap(int widthnew, int heightnew);   //缩放
	virtual BOOL OnInitDialog();                    //初始化

	void Reshape(int nItemID, double Space, int nNumber);

	/*** 变换函数 ***/
	void DIBIDWT(HDIB hDIB);
	void IDWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize);
	void DIBDWT(HDIB hDIB);
	int a(int nX, int nXSize);
	void DWT(LPBYTE lpData, int nX, int nY, int nXSize, int nYSize);
	BOOL IFreqWALSH(double* f, double* F, LONG lWidth, LONG lHeight);
	BOOL FreqWALSH(double* f, double* F, LONG lWidth, LONG lHeight);
	BOOL DIBWalsh(HDIB hDIB);
	void IWALSH(double* F, double* f, int r);
	BOOL IFreqDCT(double* f, double* F, LONG lWidth, LONG lHeight);
	void WALSH(double* f, double* F, int r);
	BOOL FreqDCT(double* f, double* F, LONG lWidth, LONG lHeight);
	BOOL DIBDCT(HDIB hDIB);
	void IDCT(double* F,double* f, int r);
	void DCT(double* f, double* F, int r);
	BOOL IFourier(LPBYTE TD, LONG lWidth, LONG lHeight, complex<double> * FD);
	BOOL Fourier(complex<double> * TD, LONG lWidth, LONG lHeight, complex<double> * FD);
	BOOL DIBFourier(HDIB hDIB);
	void IFFT(complex<double> * FD, complex<double> * TD, int r);
	void FFT(complex<double> * TD, complex<double> * FD, int r);

	/*** 频域滤波 ***/
	void MinPower(HDIB hDIB, float fD);
	void PSE_Filter(HDIB hDIB, float fD);
	void WienerFilter(HDIB hDIB, float fD);
	void RetrorseFilter(HDIB hDIB,float fD);
	void MutualFilter(HDIB hDIB, CRect* pRect, int nSum);
	void ButterworthH(HDIB hDIB, float fD);
	void ButterworthL(HDIB hDIB, float fD);

	//消息映射
	afx_msg void OnIdbFftdlgOk();
	afx_msg void OnIdbFftdlgCancel();
	afx_msg void OnIdbFftdlgFft();
	afx_msg void OnIdbFftdlgFilter();
};