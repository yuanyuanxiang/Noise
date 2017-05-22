#pragma once
#include "MyImage.h"


// CARTDlg 对话框

#define IDB_ARTDLG_CREATE_IMAGE                301
#define IDB_ARTDLG_PROJECT                     302
#define IDB_ARTDLG_SCANING                     303
#define IDB_ARTDLG_PAUSE                       304
#define IDB_ARTDLG_SINGLE_LINE                 305
#define IDB_ARTDLG_SINGLE_STEP                 306
#define IDB_ARTDLG_RANDOM                      307
#define IDB_ARTDLG_SOLVE                       308
#define IDB_ARTDLG_OK                          309
#define IDB_ARTDLG_CANCEL                      310
#define IDB_ARTDLG_PROJECT_SETTING             311
#define IDB_ARTDLG_SCAN_SETTING                312

#define ORIGIN                                 0
#define PROJECT                                1
#define BACK_PROJECT                           2

class CARTDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CARTDlg)

public:
	CARTDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CARTDlg();

// 对话框数据
	enum { IDD = IDD_ARTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public://新增成员与函数
	//对象
	CMyCircle*                                 m_pBig;  //大圆
	CMyCircle*                               m_pSmall;  //小圆
	CMyImage*                              m_pMyImage;  //待重建图像
	CMyImage*                          m_pMyImageCopy;  //带重建图像副本
	CMyRay*                                   m_pRays;  //射线

	int                                      m_nWidth;  //宽
	int                                     m_nHeight;  //高
	LONG                             m_nPixelQuantity;  //像素数

	//属性
	TYPE                                  m_BigRadius;  //大圆半径
	TYPE                                m_SmallRadius;  //小圆半径
	CMyPoint                              m_BigCenter;  //大圆圆心
	CMyPoint                            m_SmallCenter;  //小圆圆心
	TYPE                                   m_BigDecay;  //大圆衰减系数
	TYPE                                 m_SmallDecay;  //小圆衰减系数

	//射线
	TYPE                                 m_AngleBegin;  //起始角
	TYPE                                   m_AngleEnd;  //终止角
	int                              m_nAngleQuantity;  //角度采样数
	TYPE                              m_DistanceBegin;  //起始距离
	TYPE                                m_DistanceEnd;  //终止距离
	int                           m_nDistanceQuantity;  //距离采样数

	//按钮
	CMFCButton*                      m_pBnCreateImage;  //建立图像
	CMFCButton*                          m_pBnProject;  //投影
	CMFCButton*                          m_pBnScaning;  //扫描
	CMFCButton*                            m_pBnPause;  //暂停
	CMFCButton*                       m_pBnSingleLine;  //逐线
	CMFCButton*                       m_pBnSingleStep;  //逐步
	CMFCButton*                           m_pBnRandom;  //随机
	CMFCButton*                         m_pBnARTSolve;  //立即

	CMFCButton*                   m_pBnProjectSetting;  //投影设置
	CMFCButton*                      m_pBnScanSetting;  //扫描设置
	CMFCButton*                               m_pBnOK;  //确定
	CMFCButton*                           m_pBnCancel;  //取消
	
	//位图
	CRect*                               m_pPaintRect;  //绘图区
	CBitmap*                             m_pBmpOrigin;  //原图
	CBitmap*                            m_pBmpProject;  //投影图
	CBitmap*                        m_pBmpBackProject;  //反投影

	//其他
	BOOL                              m_bImageCreated;  //已经生成源图像
	BOOL                                 m_bProjected;  //获得投影
	BOOL                                m_bHaveScaned;  //已经扫描
	BOOL                                 m_bARTPaused;  //迭代暂停
	BOOL                               m_bHaveRebuild;  //已经重建
	BOOL                               m_bIterateLByL;  //逐线迭代
	
	int                               m_nCurImageFlag;  //当前显示的图
	int                                m_nIterateLine;  //当前迭代直线
	int                                m_nIterateStep;  //当前迭代步数
	int                                    m_nLineGap;  //每线迭代间隔
	int                                    m_nStepGap;  //每步迭代间隔

	virtual BOOL OnInitDialog();
	void Reshape(int nItemID, double Space, int nNumber, int Col = 1);  //重绘控件
	BOOL CreateOriginImage();                                           //建立一幅图像
	BOOL CreateTargetImage(CBitmap* Target);                            //重建出来的图像

	void InformParent();                                                //通知父窗口

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();

	afx_msg void OnIdbCreateImage();
	afx_msg void OnIdbProject();
	afx_msg void OnIdbScaning();
	afx_msg void OnIdbPause();
	afx_msg void OnIdbSingleLine();
	afx_msg void OnIdbSingleStep();
	afx_msg void OnIdbRandom();
	afx_msg void OnIdbSolve();
	afx_msg void OnIdbOK();
	afx_msg void OnIdbCancel();
	afx_msg void OnIdbProjectSetting();
	afx_msg void OnIdbScanSetting();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//其他
void Create32Bitmap(CBitmap* pBmp, TYPE* pType, int width, int height);
void CreateBitmapUsingMatrix(CBitmap* pBmp, TYPE* pType, int width, int height);
void CreateBitmapUsingRays(CBitmap*pBmp, CMyRay* pRays, TYPE MAX, int DistanceQuantity, int AngleQuantity);