
// GX0313_traversenetDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GX0313_traversenet.h"
#include "GX0313_traversenetDlg.h"
#include "afxdialogex.h"
#include "DrawDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//外部全局类对象
extern traversenet classtest;
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


// CGX0313traversenetDlg 对话框



CGX0313traversenetDlg::CGX0313traversenetDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GX0313_TRAVERSENET_DIALOG, pParent)
	, strShow(_T(""))
	, output(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGX0313traversenetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITshow, strShow);
	DDX_Text(pDX, IDC_EDITout, output);
}

BEGIN_MESSAGE_MAP(CGX0313traversenetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOpenFile, &CGX0313traversenetDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDSaveFile, &CGX0313traversenetDlg::OnBnClickedSavefile)
	ON_BN_CLICKED(IDDrawnet, &CGX0313traversenetDlg::OnBnClickedDrawnet)
	ON_BN_CLICKED(IDHelpAbout, &CGX0313traversenetDlg::OnBnClickedHelpabout)
END_MESSAGE_MAP()


// CGX0313traversenetDlg 消息处理程序

BOOL CGX0313traversenetDlg::OnInitDialog()
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

	//SetBackgroundColor(RGB(255, 255, 255));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGX0313traversenetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGX0313traversenetDlg::OnPaint()
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
HCURSOR CGX0313traversenetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//打开文件按钮
void CGX0313traversenetDlg::OnBnClickedOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	int flag = classtest.ReadFile(strShow);
	UpdateData(false);
	switch (flag)
	{
	case 0://读取出错
		MessageBox(_T("读取文件错误！"), _T("错误"));
		classtest.deletedata();
		return;
	case -1://用户取消打开文件
		return;
	case 1://顺利读取
		output = classtest.ResultStr();
		UpdateData(false);
		return;
	case 2://文件为空
		MessageBox(_T("文件为空！"), _T("错误"));
		return;
	default:
		return;
	}
}

//保存文件按钮
void CGX0313traversenetDlg::OnBnClickedSavefile()
{
	// TODO: 在此添加控件通知处理程序代码
	int flag = classtest.SaveFile();
	switch (flag)
	{
	case 0://没有数据
		MessageBox(_T("没有数据可保存！"), _T("错误"));
		return;
	case 1://顺利保存
		MessageBox(_T("保存成功！"), _T("提示"));
		return;
	case -1://保存失败
		MessageBox(_T("保存文件失败！"), _T("错误"));
		return;
	case 2://用户取消
		return;
	default:
		return;
	}
}

//绘图按钮
void CGX0313traversenetDlg::OnBnClickedDrawnet()
{
	// TODO: 在此添加控件通知处理程序代码
	DrawDlg dlg;//创建绘图对话框类对象
	dlg.SetBackgroundColor(RGB(255, 255, 255));//背景白色
	dlg.DoModal();//运行对话框
}

//关于/帮助按钮
void CGX0313traversenetDlg::OnBnClickedHelpabout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlg;
	dlg.SetBackgroundColor(RGB(255, 255, 255));
	dlg.DoModal();
}


