#pragma once

// 对话框：频率域滤波

// CFreqFilter 对话框

#include "FFTDlg.h"
#include "afxwin.h"

class CFreqFilter : public CDialogEx
{
	DECLARE_DYNAMIC(CFreqFilter)

public:
	CFreqFilter(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFreqFilter();

// 对话框数据
	enum { IDD = IDD_FREQFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public://新增成员与函数
	int                            m_nMethod;       //滤波方法
	float                           m_fPower;       //谱
	CEdit                        m_EditPower;       //Edit控件
	CFFTDlg*                       m_pFFTDlg;       //上一层对话框指针
	CBitmap*                       m_pBitmap;       //上一层对话框位图
	CFFTDlg* GetDocument();                         //获取上一层对话框指针
	CBitmap* GetBitmap();                           //获取上一层对话框位图
	virtual BOOL OnInitDialog();                    //对话框初始化
	afx_msg void OnFilterButterworthl();
	afx_msg void OnFilterButterworthh();
	afx_msg void OnFilterRetrorse();
	afx_msg void OnFilterWiener();
	afx_msg void OnFilterPse();
	afx_msg void OnFilterMinpower();
};