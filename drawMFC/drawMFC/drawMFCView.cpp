
// drawMFCView.cpp: CdrawMFCView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "drawMFC.h"
#endif

#include "drawMFCDoc.h"
#include "drawMFCView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdrawMFCView

IMPLEMENT_DYNCREATE(CdrawMFCView, CView)

BEGIN_MESSAGE_MAP(CdrawMFCView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_32771, &CdrawMFCView::OnLineDDA)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32772, &CdrawMFCView::OnLinezhongdian)
	ON_COMMAND(ID_32773, &CdrawMFCView::OnLineBresenham)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32774, &CdrawMFCView::OnCircleMid)
	ON_COMMAND(ID_32775, &CdrawMFCView::OnCircleBre)
	ON_COMMAND(ID_32776, &CdrawMFCView::OnMidEllispe)
	ON_COMMAND(ID_32777, &CdrawMFCView::OncolorBlack)
	ON_COMMAND(ID_32778, &CdrawMFCView::OncolorWhite)
	ON_COMMAND(ID_32779, &CdrawMFCView::OncolorRed)
	ON_COMMAND(ID_32780, &CdrawMFCView::OncolorGreen)
	ON_COMMAND(ID_32781, &CdrawMFCView::OncolorBlue)
	ON_COMMAND(ID_32782, &CdrawMFCView::OncolorSet)
	ON_COMMAND(ID_32783, &CdrawMFCView::Ondashline)
	ON_COMMAND(ID_32784, &CdrawMFCView::OnScanFill)
	ON_COMMAND(ID_32785, &CdrawMFCView::OnSeedFill)
	ON_COMMAND(ID_32786, &CdrawMFCView::OnDrawPolygon)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_32787, &CdrawMFCView::OnIMGmove)
	ON_COMMAND(ID_32788, &CdrawMFCView::OnIMGrotate)
	ON_COMMAND(ID_32789, &CdrawMFCView::OnClipLine)
	ON_COMMAND(ID_32790, &CdrawMFCView::OnClipPolygon)
	ON_COMMAND(ID_32791, &CdrawMFCView::OnClipPolygon1)
	ON_COMMAND(ID_32792, &CdrawMFCView::OnScale)
	ON_COMMAND(ID_32793, &CdrawMFCView::OnSymmetry)
END_MESSAGE_MAP()

// CdrawMFCView 构造/析构
//mainclass maindraw;
CdrawMFCView::CdrawMFCView() noexcept
{
	// TODO: 在此处添加构造代码

}

CdrawMFCView::~CdrawMFCView()
{
}

BOOL CdrawMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CdrawMFCView 绘图

void CdrawMFCView::OnDraw(CDC* pDC)
{
	CdrawMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//c++;
	CRect rect;
	GetClientRect(rect);
	maindraw.setDC(pDC);
	maindraw.setsize(&rect);
}


// CdrawMFCView 打印

BOOL CdrawMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CdrawMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CdrawMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CdrawMFCView 诊断

#ifdef _DEBUG
void CdrawMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CdrawMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CdrawMFCDoc* CdrawMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CdrawMFCDoc)));
	return (CdrawMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CdrawMFCView 消息处理程序



void CdrawMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	maindraw.setDC(GetDC());
	maindraw.setmpdown(point);
	CView::OnLButtonDown(nFlags, point);
}


void CdrawMFCView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	maindraw.setDC(GetDC());
	maindraw.setmpup(point);
	CView::OnLButtonUp(nFlags, point);
}


void CdrawMFCView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	maindraw.setDC(GetDC());
	maindraw.setmpmove(point);
	CView::OnMouseMove(nFlags, point);
}



void CdrawMFCView::OnLineDDA()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(1);
}

void CdrawMFCView::OnLinezhongdian()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(2);
}


void CdrawMFCView::OnLineBresenham()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(3);
}


void CdrawMFCView::OnCircleMid()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(4);
}


void CdrawMFCView::OnCircleBre()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(5);
}


void CdrawMFCView::OnMidEllispe()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(6);
}


void CdrawMFCView::OncolorBlack()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setcolor(RGB(0, 0, 0));
}


void CdrawMFCView::OncolorWhite()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setcolor(RGB(255, 255, 255));
}


void CdrawMFCView::OncolorRed()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setcolor(RGB(255, 0, 0));
}


void CdrawMFCView::OncolorGreen()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setcolor(RGB(0, 255, 0));
}


void CdrawMFCView::OncolorBlue()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setcolor(RGB(0, 0, 255));
}


void CdrawMFCView::OncolorSet()
{
	// TODO: 在此添加命令处理程序代码
	CColorDialog setClrDlg;
	int clr;
	if (IDOK == setClrDlg.DoModal())
	{
	     clr = setClrDlg.m_cc.rgbResult;            // 保存用户选择的颜色
		 maindraw.setcolor(clr);
	}
}


void CdrawMFCView::Ondashline()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(7);
}


void CdrawMFCView::OnScanFill()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(8);
}


void CdrawMFCView::OnSeedFill()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(9);
}


void CdrawMFCView::OnDrawPolygon()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(10);
}


void CdrawMFCView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	maindraw.setDC(GetDC());
	maindraw.setmpdbc(point);
	CView::OnLButtonDblClk(nFlags, point);
}


void CdrawMFCView::OnIMGmove()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(11);
}


void CdrawMFCView::OnIMGrotate()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(12);
}

void CdrawMFCView::OnScale()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(13);
}


void CdrawMFCView::OnSymmetry()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(14);
}

void CdrawMFCView::OnClipLine()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(15);
}

void CdrawMFCView::OnClipPolygon1()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(16);
}

void CdrawMFCView::OnClipPolygon()
{
	// TODO: 在此添加命令处理程序代码
	maindraw.setDC(GetDC());
	maindraw.setchoose(17);
}




