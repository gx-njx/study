#pragma once
#include "Line.h"
#include "Circle.h"
#include "GPolygon.h"
class mainclass
{
	CDC *pDC, b1DC, b2DC;//输出画板，内部画板
	CBitmap bmp1, bmp2;//内部画图
	CRect rect;
	CArray<Line, Line&>linesarr;//动态线数组
	CArray<Circle, Circle&>circlesarr;//动态圆数组
	CArray<GPolygon, GPolygon&>polygonsarr;//动态多边形数组
	GPolygon clip;//裁剪多边形
	int color, choose, alpre;//颜色,选择项，连续画图状态
	bool pre, newpre, mmove;//画图准备，初始化检测，移动检测
	CPoint mpup, mpdown, mpmove, mpdbc;//鼠标松开，按下点，移动点，双击点
public:
	mainclass();//初始化
	~mainclass();
	void setDC(CDC *dc);//设置输出画板
	void setsize(CRect *rect);//设置客户区大小
	void setmpup(CPoint mp);//设置鼠标松开点
	void setmpdown(CPoint mp);//设置鼠标按下点
	void setmpmove(CPoint mp);//设置鼠标过程位置
	void setmpdbc(CPoint mp);//设置鼠标双击点
	void setcolor(COLORREF col);//设置颜色
	void setchoose(int cho);//设置选项
	void draw();//绘图
	void DCout();//输出
	void redraw();//重新绘图
	void SeedFill(CPoint mp, int ncolor);//种子嵌套填充
	void ImgMove(CPoint mp1, CPoint mp2);//平移变换
	void ImgRotate(CPoint mp1, CPoint mp2);//旋转变换
	void ImgScale(CPoint mp1, CPoint mp2);//缩放变换
	void ImgSymmetry(CPoint mp1, CPoint mp2);//对称变换
	void rotate(CPoint &mp, double the, CPoint cen);//旋转辅助函数
	void scale(CPoint &mp, double the, CPoint cen);//缩放辅助函数
	void symmetry(CPoint &mp, CPoint mp1, CPoint mp2);//对称辅助函数
	void ClipLine(CPoint mp1, CPoint mp2);//线段裁剪
	void ClipPolygon1(CPoint mp1, CPoint mp2);//矩形多边形裁剪
	void ClipPolygon(GPolygon &pgin, GPolygon &pgc);//任意多边形裁剪
	bool crossline1(CPoint sp1, CPoint ep1, CPoint sp2, CPoint ep2, CPoint &cp);//求两直线交点（不包括端点）
	bool crossline2(CPoint sp1, CPoint ep1, CPoint sp2, CPoint ep2, CPoint &cp);//求两直线交点（包括端点）
};

