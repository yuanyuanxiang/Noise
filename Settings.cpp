// Settings.cpp : 实现文件
//

#include "stdafx.h"
#include "Noise.h"
#include "Settings.h"
#include "afxdialogex.h"


// CSettings 对话框

IMPLEMENT_DYNAMIC(CSettings, CDialogEx)

CSettings::CSettings(CWnd* pParent /*=NULL*/) : CDialogEx(CSettings::IDD, pParent)
{
	m_Mean = 8;
	m_Std = 24;
	m_Pepper = 0.01;
	m_Salt = 0.01;
	m_nFilterSize = 3;
	m_nMaxFilterSize = 7;
}

CSettings::~CSettings()
{
}

void CSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAUSS_MEAN, m_CtrlGaussMean);
	DDX_Control(pDX, IDC_GAUSS_STD, m_CtrlGaussStd);
	DDX_Control(pDX, IDC_PEPPER_PROBALITITY, m_CtrlPepper);
	DDX_Control(pDX, IDC_SALT_PROBALITITY, m_CtrlSalt);
	DDX_Control(pDX, IDC_FILTER_SIZE, m_CtrlFilterSize);
	DDX_Control(pDX, IDC_MAX_FILTER_SIZE, m_CtrlMaxFilterSize);
	DDX_Text(pDX, IDC_GAUSS_MEAN, m_Mean);
	DDX_Text(pDX, IDC_GAUSS_STD, m_Std);
	DDX_Text(pDX, IDC_PEPPER_PROBALITITY, m_Pepper);
	DDX_Text(pDX, IDC_SALT_PROBALITITY, m_Salt);
	DDX_Text(pDX, IDC_FILTER_SIZE, m_nFilterSize);
	DDV_MinMaxInt(pDX, m_nFilterSize, 1, 32);
	DDX_Text(pDX, IDC_MAX_FILTER_SIZE, m_nMaxFilterSize);
	DDV_MinMaxInt(pDX, m_nMaxFilterSize, 1, 32);
}


BEGIN_MESSAGE_MAP(CSettings, CDialogEx)
END_MESSAGE_MAP()


// CSettings 消息处理程序
