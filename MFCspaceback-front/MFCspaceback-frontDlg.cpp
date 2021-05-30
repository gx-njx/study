
// MFCspaceback-frontDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCspaceback-front.h"
#include "MFCspaceback-frontDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMFCspacebackfrontDlg 对话框



CMFCspacebackfrontDlg::CMFCspacebackfrontDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCSPACEBACKFRONT_DIALOG, pParent)
	, input(_T(""))
	, output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCspacebackfrontDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITinput, input);
	DDX_Text(pDX, IDC_EDIToutput, output);
}

BEGIN_MESSAGE_MAP(CMFCspacebackfrontDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCAL, &CMFCspacebackfrontDlg::OnBnClickedCal)
	ON_BN_CLICKED(IDOPENFILE, &CMFCspacebackfrontDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDSAVEFILE, &CMFCspacebackfrontDlg::OnBnClickedSavefile)
END_MESSAGE_MAP()


// CMFCspacebackfrontDlg 消息处理程序

BOOL CMFCspacebackfrontDlg::OnInitDialog()
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

	input = _T("4,5\r\n")
		_T("16.012, 79.963, -73.93, 78.706, 5083.205, 5852.099, 527.925\r\n")
		_T("88.56, 81.134, -5.252, 78.184, 5780.02, 5906.365, 571.549\r\n")
		_T("13.362, -79.37, -79.122, -78.879, 5210.879, 4258.446, 461.81\r\n")
		_T("82.24, -80.027, -9.887, -80.089, 5909.264, 4314.283, 455.484\r\n")
		_T("51.758, 80.555, -39.953, 78.463\r\n")
		_T("14.618, -0.231, -76.006, 0.036\r\n")
		_T("49.88, -0.782, -42.201, -1.022\r\n")
		_T("86.14, -1.346, -7.706, -2.112\r\n")
		_T("48.035, -79.962, -44.438, -79.736\r\n")
		_T("0, 0, 0.15\r\n")
		_T("以上为输入格式示例，可以直接计算\r\n")
		_T("第一行分别为已知控制点个数和待求点个数\r\n")
		_T("后面依次每行一个点，最后是内方位元素");
	UpdateData(false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCspacebackfrontDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCspacebackfrontDlg::OnPaint()
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
HCURSOR CMFCspacebackfrontDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCspacebackfrontDlg::OnBnClickedCal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	test.mainfunc(input, output);
	UpdateData(false);
}


void CMFCspacebackfrontDlg::OnBnClickedOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	test.readfile(input);
	UpdateData(false);
}


void CMFCspacebackfrontDlg::OnBnClickedSavefile()
{
	// TODO: 在此添加控件通知处理程序代码
	test.savefile();
}
