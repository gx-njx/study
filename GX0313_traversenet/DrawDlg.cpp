// DrawDlg.cpp: 实现文件
//

#include "pch.h"
#include "GX0313_traversenet.h"
#include "DrawDlg.h"
#include "afxdialogex.h"


//外部全局类对象
extern traversenet classtest;
// DrawDlg 对话框

IMPLEMENT_DYNAMIC(DrawDlg, CDialogEx)

DrawDlg::DrawDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWDLG, pParent)
{
	size = 20;
	message = 0;
}

DrawDlg::~DrawDlg()
{
}

void DrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(DrawDlg, CDialogEx)
	ON_BN_CLICKED(IDDrawnet, &DrawDlg::OnBnClickedDrawnet)
	ON_BN_CLICKED(IDDrawcircle, &DrawDlg::OnBnClickedDrawcircle)
	ON_BN_CLICKED(IDDrawname, &DrawDlg::OnBnClickedDrawname)
	ON_BN_CLICKED(IDC_BIG, &DrawDlg::OnBnClickedBig)
	ON_BN_CLICKED(IDC_Small, &DrawDlg::OnBnClickedSmall)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// DrawDlg 消息处理程序


void DrawDlg::OnBnClickedDrawnet()
{
	// TODO: 在此添加控件通知处理程序代码
	message = 1;
	Invalidate();
}


void DrawDlg::OnBnClickedDrawcircle()
{
	// TODO: 在此添加控件通知处理程序代码
	message = 2;
	Invalidate();
}


void DrawDlg::OnBnClickedDrawname()
{
	// TODO: 在此添加控件通知处理程序代码
	if (message == 1)message = 3;
	if (message == 2)message = 4;
	Invalidate();
}



void DrawDlg::OnBnClickedBig()
{
	// TODO: 在此添加控件通知处理程序代码
	size++;
	Invalidate();
}


void DrawDlg::OnBnClickedSmall()
{
	// TODO: 在此添加控件通知处理程序代码
	if (size > 0)size--;
	Invalidate();
}


void DrawDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CDC *pDC = GetDC();
	CRect rc;
	GetClientRect(rc);
	switch (message)
	{
	case 0:
		return;
	case 1://导线网
		if (!classtest.Drawnet(pDC, rc)) { MessageBox(_T("没有数据！"), _T("错误")); message = 0; }
		return;
	case 2://误差椭圆
		if (!classtest.Drawcircle(pDC, rc, size)) 
		{
			MessageBox(_T("没有数据！"), _T("错误")); 
			message = 0;
			return;
		}
		else pDC->TextOutW(100, 60, _T("注意：误差椭圆经过缩放处理，并非实际大小。"));
		return;
	case 3://导线网+名
		if (!classtest.Drawnet(pDC, rc)) 
		{
			MessageBox(_T("没有数据！"), _T("错误")); 
			message = 0;
			return;
		}
		if (!classtest.Drawname(pDC, rc))MessageBox(_T("没有数据！"), _T("错误"));
		return;
	case 4://误差椭圆+名
		if (!classtest.Drawcircle(pDC, rc, size))
		{
			MessageBox(_T("没有数据！"), _T("错误"));
			message = 0;
			return;
		}
		else pDC->TextOutW(100, 60, _T("注意：误差椭圆经过缩放处理，并非实际大小。"));
		if (!classtest.Drawname(pDC, rc))MessageBox(_T("没有数据！"), _T("错误"));
		return;
	}
}
