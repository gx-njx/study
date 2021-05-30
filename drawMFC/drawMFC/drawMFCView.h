
// drawMFCView.h: CdrawMFCView 类的接口
//

#pragma once

#include "mainclass.h"

class CdrawMFCView : public CView
{
protected: // 仅从序列化创建
	CdrawMFCView() noexcept;
	DECLARE_DYNCREATE(CdrawMFCView)

// 特性
public:
	CdrawMFCDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CdrawMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	mainclass maindraw;
	afx_msg void OnLineDDA();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLinezhongdian();
	afx_msg void OnLineBresenham();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCircleMid();
	afx_msg void OnCircleBre();
	afx_msg void OnMidEllispe();
	afx_msg void OncolorBlack();
	afx_msg void OncolorWhite();
	afx_msg void OncolorRed();
	afx_msg void OncolorGreen();
	afx_msg void OncolorBlue();
	afx_msg void OncolorSet();
	afx_msg void Ondashline();
	afx_msg void OnScanFill();
	afx_msg void OnSeedFill();
	afx_msg void OnDrawPolygon();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnIMGmove();
	afx_msg void OnIMGrotate();
	afx_msg void OnClipLine();
	afx_msg void OnClipPolygon();
	afx_msg void OnClipPolygon1();
	afx_msg void OnScale();
	afx_msg void OnSymmetry();
};

#ifndef _DEBUG  // drawMFCView.cpp 中的调试版本
inline CdrawMFCDoc* CdrawMFCView::GetDocument() const
   { return reinterpret_cast<CdrawMFCDoc*>(m_pDocument); }
#endif

