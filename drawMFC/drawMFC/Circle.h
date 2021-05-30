#pragma once
class Circle
{
public:
	CPoint center;//圆心
	int R;//半径
	int a, b;//长短轴
	bool ellispe;//是否椭圆
	int incolor;
	Circle();
	Circle(CPoint po, int r);//圆心半径
	Circle(CPoint po1, CPoint po2);//两点框椭圆
	~Circle();
	void CirclePoints(int x, int y, int color, CDC *pDC);//利用对称画圆
	void midpoint(CDC *pDC, int color);//中点算法
	void Bresenham(CDC *pDC, int color);//Bresenham算法
	void EllipsePoints(int x, int y, int color, CDC *pDC);//利用对称画椭圆
	void Midpointellispe(CDC *pDC, int color);//中点画椭圆
};

