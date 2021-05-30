
// GX8211180313Dlg.h: 头文件
//

#pragma once
#include"DEM.h"

// CGX8211180313Dlg 对话框
class CGX8211180313Dlg : public CDialogEx
{
// 构造
public:
	CGX8211180313Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GX8211180313_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonim2();
	afx_msg void OnBnClickedButtonmorevec();
	// 二值化阈值
	int im2t;
	// Morevec窗口大小
	int Mwinsize;
	// Morevec阈值
	double Mt;
	afx_msg void OnBnClickedButtonmatch();
	int Matchwinsize;
	double Matcht;
	int Matchx1;
	int Matchx2;
	DEM dem;
	afx_msg void OnBnClickedDemload();
	int demXsize;
	int demYsize;
	double demR;
};
