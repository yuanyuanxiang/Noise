// ARTDlgScanSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "Noise.h"
#include "ARTDlgScanSetting.h"
#include "afxdialogex.h"


// CARTDlgScanSetting 对话框

IMPLEMENT_DYNAMIC(CARTDlgScanSetting, CDialogEx)

CARTDlgScanSetting::CARTDlgScanSetting(CWnd* pParent /*=NULL*/)	: CDialogEx(CARTDlgScanSetting::IDD, pParent)
{
	m_AngleBegin = PRECISION;                //起始角
	m_AngleEnd = 2*PI-PRECISION;             //终止角
	m_nAngleQuantity = 180;                  //角度采样数
	m_DistanceBegin = PRECISION;             //起始距离
	m_DistanceEnd = 768;                     //终止距离
	m_nDistanceQuantity = 180;               //距离采样数
}

CARTDlgScanSetting::~CARTDlgScanSetting()
{
}

void CARTDlgScanSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ARTDLG_ANGLE_BEGIN, m_AngleBegin);
	DDX_Text(pDX, IDC_ARTDLG_ANGLE_END, m_AngleEnd);
	DDX_Text(pDX, IDC_ARTDLG_ANGLE_QUANTITY, m_nAngleQuantity);
	DDV_MinMaxInt(pDX, m_nAngleQuantity, 8, 1024);
	DDX_Text(pDX, IDC_ARTDLG_DISTANCE_BEGIN, m_DistanceBegin);
	DDX_Text(pDX, IDC_ARTDLG_DISTANCE_END, m_DistanceEnd);
	DDX_Text(pDX, IDC_ARTDLG_DISTANCE_QUANTITY, m_nDistanceQuantity);
	DDV_MinMaxInt(pDX, m_nDistanceQuantity, 8, 1024);
}


BEGIN_MESSAGE_MAP(CARTDlgScanSetting, CDialogEx)
END_MESSAGE_MAP()


// CARTDlgScanSetting 消息处理程序
