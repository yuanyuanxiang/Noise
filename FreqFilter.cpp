// FreqFilter.cpp : 实现文件
//

#include "stdafx.h"
#include "Noise.h"
#include "FreqFilter.h"
#include "afxdialogex.h"


// CFreqFilter 对话框

IMPLEMENT_DYNAMIC(CFreqFilter, CDialogEx)

CFreqFilter::CFreqFilter(CWnd* pParent /*=NULL*/) : CDialogEx(CFreqFilter::IDD, pParent)
{

}

CFreqFilter::~CFreqFilter()
{
}

void CFreqFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILTER_POWER, m_EditPower);
	DDX_Text(pDX, IDC_FILTER_POWER, m_fPower);
}


BEGIN_MESSAGE_MAP(CFreqFilter, CDialogEx)
	ON_COMMAND(IDC_FILTER_BUTTERWORTHL, &CFreqFilter::OnFilterButterworthl)
	ON_COMMAND(IDC_FILTER_BUTTERWORTHH, &CFreqFilter::OnFilterButterworthh)
	ON_COMMAND(IDC_FILTER_RETRORSE, &CFreqFilter::OnFilterRetrorse)
	ON_COMMAND(IDC_FILTER_WIENER, &CFreqFilter::OnFilterWiener)
	ON_COMMAND(IDC_FILTER_PSE, &CFreqFilter::OnFilterPse)
	ON_COMMAND(IDC_FILTER_MINPOWER, &CFreqFilter::OnFilterMinpower)
END_MESSAGE_MAP()


// CFreqFilter 消息处理程序

//取得上一层对话框的指针(这是指CFFTDLg)
CFFTDlg* CFreqFilter::GetDocument()
{
	CWnd* pWnd = GetParent();

	return (CFFTDlg*)pWnd;
}

//取得上一层对话框的位图(位图是指频谱)
CBitmap* CFreqFilter::GetBitmap()
{
	return GetDocument()->m_pFreq;
}

//初始化本对话框
BOOL CFreqFilter::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nMethod = 0;
	m_fPower = 200.0f;
	m_pFFTDlg = GetDocument();
	m_pBitmap = GetBitmap();

	return TRUE;
}

void CFreqFilter::OnFilterButterworthl(){	m_nMethod = 1;}
void CFreqFilter::OnFilterButterworthh(){	m_nMethod = 2;}
void CFreqFilter::OnFilterRetrorse()    {	m_nMethod = 3;}
void CFreqFilter::OnFilterWiener()      {	m_nMethod = 4;}
void CFreqFilter::OnFilterPse()         {	m_nMethod = 5;}
void CFreqFilter::OnFilterMinpower()    {	m_nMethod = 6;}