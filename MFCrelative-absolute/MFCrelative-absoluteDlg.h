
// MFCrelative-absoluteDlg.h: 头文件
//

#pragma once

#include "RAphotocal.h"

// CMFCrelativeabsoluteDlg 对话框
class CMFCrelativeabsoluteDlg : public CDialogEx
{
// 构造
public:
	CMFCrelativeabsoluteDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCRELATIVEABSOLUTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	RAphotocal test;
	afx_msg void OnBnClickedCal();
	CString input;
	CString output;
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedSavefile();
};
