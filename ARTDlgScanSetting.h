#pragma once
#include "afxwin.h"


// CARTDlgScanSetting 对话框

class CARTDlgScanSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CARTDlgScanSetting)

public:
	CARTDlgScanSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CARTDlgScanSetting();

// 对话框数据
	enum { IDD = IDD_ARTDLG_SCAN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_CtrlAngleBegin;
	CEdit m_CtrlAngleEnd;
	CEdit m_CtrlAngleQuantity;
	CEdit m_CtrlDistanceBegin;
	CEdit m_CtrlDistanceEnd;
	CEdit m_CtrlDistanceQuantity;
	double m_AngleBegin;
	double m_AngleEnd;
	int m_nAngleQuantity;
	double m_DistanceBegin;
	double m_DistanceEnd;
	int m_nDistanceQuantity;
};
