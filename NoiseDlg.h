
// NoiseDlg.h : 头文件
//

#pragma once

#include "NoiseWnd.h"
#include "resource.h"

// CNoiseDlg 对话框
class CNoiseDlg : public CDialogEx
{
// 构造
public:
	CNoiseDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CNoiseDlg();

// 对话框数据
	enum { IDD = IDD_NOISE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:// 新增成员与函数
	CNoiseWnd*          m_pNoiseWnd;                    //子窗口类对象

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
