#pragma once


// DrawDlg 对话框

class DrawDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DrawDlg)

public:
	DrawDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DrawDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int size;//椭圆大小控制
	int message;//当前绘图模式
	afx_msg void OnBnClickedDrawnet();
	afx_msg void OnBnClickedDrawcircle();
	afx_msg void OnBnClickedDrawname();
	afx_msg void OnBnClickedBig();
	afx_msg void OnBnClickedSmall();
	afx_msg void OnPaint();
};
