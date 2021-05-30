
// GX8211180313Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GX8211180313.h"
#include "GX8211180313Dlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Featurecv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGX8211180313Dlg 对话框



CGX8211180313Dlg::CGX8211180313Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GX8211180313_DIALOG, pParent)
	, im2t(0)
	, Mwinsize(0)
	, Mt(0)
	, Matchwinsize(0)
	, Matcht(0)
	, Matchx1(0)
	, Matchx2(0)
	, demXsize(0)
	, demYsize(0)
	, demR(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGX8211180313Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITim2t, im2t);
	DDX_Text(pDX, IDC_EDITMwinsize, Mwinsize);
	DDX_Text(pDX, IDC_EDITMt, Mt);
	DDX_Text(pDX, IDC_EDITMatchwinsize, Matchwinsize);
	DDX_Text(pDX, IDC_EDITMatcht, Matcht);
	DDX_Text(pDX, IDC_EDITMatchx1, Matchx1);
	DDX_Text(pDX, IDC_EDITMatchx2, Matchx2);
	DDX_Text(pDX, IDC_EDITDEMXsize, demXsize);
	DDX_Text(pDX, IDC_EDITDEMYsize, demYsize);
	DDX_Text(pDX, IDC_EDITDEMR, demR);
}

BEGIN_MESSAGE_MAP(CGX8211180313Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGX8211180313Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTONim2, &CGX8211180313Dlg::OnBnClickedButtonim2)
	ON_BN_CLICKED(IDC_BUTTONMorevec, &CGX8211180313Dlg::OnBnClickedButtonmorevec)
	ON_BN_CLICKED(IDC_BUTTONmatch, &CGX8211180313Dlg::OnBnClickedButtonmatch)
	ON_BN_CLICKED(IDC_DEMload, &CGX8211180313Dlg::OnBnClickedDemload)
END_MESSAGE_MAP()


// CGX8211180313Dlg 消息处理程序

BOOL CGX8211180313Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	im2t = 127;
	Mwinsize = 5;
	Mt = 1000;
	Matchwinsize = 9;
	Matcht = 0.9;
	Matchx1 = 480;
	Matchx2 = 540;
	demXsize = 1143;
	demYsize = 858;
	demR = 10;
	UpdateData(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGX8211180313Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGX8211180313Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGX8211180313Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGX8211180313Dlg::OnBnClickedButton1()
{
	USES_CONVERSION;
	CFileDialog getfile(TRUE, _T("jpg"));
	if (getfile.DoModal() != IDOK)return;
	CString pathname = getfile.GetPathName();
	//打开影像	
	//IMREAD_GRAYSCALE  以灰度形式打开
	Mat m_srcimg = imread(W2A(pathname), IMREAD_GRAYSCALE);
	//Mat m_srcimg = imread("data\\uavbuilding.jpg", IMREAD_GRAYSCALE);
	imshow("src image", m_srcimg); //显示
	waitKey(); //等待操作，将窗口关闭\回车\ESC等操作会运行以下的程序
}


void CGX8211180313Dlg::OnBnClickedButtonim2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	USES_CONVERSION;
	CFileDialog getfile(TRUE, _T("jpg"));
	if (getfile.DoModal() != IDOK)return;
	CString pathname = getfile.GetPathName();
	Mat img = imread(W2A(pathname), IMREAD_GRAYSCALE);
	Mat outimg;
	outimg.create(img.rows, img.cols, img.type());
	for (int i = 0; i < outimg.rows; i++) {
		for (int j = 0; j < outimg.cols; j++) {
			int graytmp = img.at<uchar>(i, j);
			if (graytmp > im2t) {
				outimg.at<uchar>(i, j) = 255;
			}
			else {
				outimg.at<uchar>(i, j) = 0;
			}
		}
	}imshow("im2image", outimg); //显示
	waitKey(); //等待操作，将窗口关闭\回车\ESC等操作会运行以下的程序
}


void CGX8211180313Dlg::OnBnClickedButtonmorevec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	USES_CONVERSION;
	CFileDialog getfile(TRUE, _T("jpg"));
	if (getfile.DoModal() != IDOK)return;
	CString pathname = getfile.GetPathName();
	Mat img = imread(W2A(pathname), IMREAD_GRAYSCALE);
	Featurecv fcv;
	vector<Point3i> point;
	fcv.Morevec(img, Mwinsize, Mt, point);
	cvtColor(img, img, COLOR_GRAY2BGR);//灰图转彩图
	CString outstr, temstr;
	int count = 0;
	temstr.Format(_T("窗口大小：%d 兴趣值阈值：%d\n"), Mwinsize, int(Mt));
	outstr += temstr;
	for (int i = 0; i < point.size(); i++) {
		circle(img, Point(point[i].y, point[i].x), 4, Scalar(0, 0, 255));
		count++;
		temstr.Format(_T("%d 坐标：（%d，%d）兴趣值：%d\n"), count, point[i].x, point[i].y, point[i].z);
		outstr += temstr;
	}
	imshow("MorevecImage", img);
	waitKey();
	CFileDialog savefile(FALSE, _T("txt"), _T("Morevec特征结果"));
	if (savefile.DoModal() != IDOK)return;
	pathname = savefile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))return;
	setlocale(LC_ALL, "chs");//没有这句将无法输出中文
	file.WriteString(outstr);
	file.Close();
	MessageBox(_T("已保存"));
}


void CGX8211180313Dlg::OnBnClickedButtonmatch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	USES_CONVERSION;
	CFileDialog getfile(TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("jpg|*.jpg||"));
	if (getfile.DoModal() != IDOK)return;
	CString pathname = getfile.GetPathName();
	Mat img1 = imread(W2A(pathname), IMREAD_GRAYSCALE);
	Mat img1c = imread(W2A(pathname));//彩图，用于绘制同名点连线
	if (getfile.DoModal() != IDOK)return;
	pathname = getfile.GetPathName();
	Mat img2 = imread(W2A(pathname), IMREAD_GRAYSCALE);
	Mat img2c = imread(W2A(pathname));
	Mat img1h;
	img1h.create(img1.rows + img2.rows, max(img1.cols, img2.cols), img1c.type());
	img2c.rowRange(0, img2c.rows).copyTo(img1h.rowRange(0, img2c.rows));
	img1c.rowRange(0, img1c.rows).copyTo(img1h.rowRange(img2c.rows, img1h.rows));//指定行复制，完成图片上下排列
	Featurecv fcv;
	vector<Point3i> point;//储存特征点
	fcv.Morevec(img1, Mwinsize, Mt, point);
	CString outstr, temstr;
	int count = 0;
	for (int i = 0; i < point.size(); i++) {
		if (point[i].y < Matchwinsize || point[i].y > img1.cols - Matchwinsize || point[i].x < Matchwinsize || point[i].x > img1.rows - Matchwinsize) {
			continue;//超出图片边界
		}
		Point2i p2;
		fcv.MatchPointX(img1, img2, Point2i(point[i].x, point[i].y), p2, Matchwinsize, Matcht, Matchx1, Matchx2);//匹配同名点
		if (p2.y == 0) {//没有找到同名点
			continue;
		}
		temstr.Format(_T("%d %d %d %d\n"), point[i].y, point[i].x, p2.y, p2.x);//opencv中坐标xy相反
		outstr = outstr + temstr;
		count++;
		circle(img1h, Point2i(point[i].y, point[i].x), 4, Scalar(0, 0, 255));//颜色BGR
		circle(img1h, Point2i(p2.y , p2.x + img1.rows), 4, Scalar(0, 0, 255));
		line(img1h, Point2i(point[i].y, point[i].x + img1.rows), Point2i(p2.y, p2.x), Scalar(0, 255, 0));//连线
	}
	temstr.Format(_T("%d\n"), count);
	outstr = temstr + outstr;
	namedWindow("MatchImage", WINDOW_AUTOSIZE);
	moveWindow("MatchImage", 0, 0);//指定位置，防止图片窗口显示在屏幕外面
	Mat img1hsmall;
	resize(img1h, img1hsmall, Size(), 0.15, 0.15);//缩放后展示匹配结果
	imshow("MatchImage", img1hsmall);
	waitKey();
	CFileDialog savefile(FALSE, _T("txt"), _T("match_point"));
	if (savefile.DoModal() != IDOK)return;
	pathname = savefile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))return;
	setlocale(LC_ALL, "chs");//没有这句将无法输出中文
	file.WriteString(outstr);//输出同名点坐标
	file.Close();
	CFileDialog savefile2(FALSE, _T("jpg"), _T("MatchImage"));
	if (savefile2.DoModal() != IDOK)return;
	pathname = savefile2.GetPathName();
	imwrite(W2A(pathname), img1h);//保存图片到文件
}


void CGX8211180313Dlg::OnBnClickedDemload()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(1);
	dem.deleteData();
	dem.openDatafile();
	dem.creatNet(demXsize, demYsize, demR);
	dem.saveDatafile();
}
