#pragma once
class Line
{
public:
	CPoint stp, endp;
	int incolor;
	Line();
	Line(CPoint sp, CPoint ep);//构造函数设置起止点
	~Line();
	void DDA(CDC *pDC, int color);//DDA算法
	void midpoint(CDC *pDC, int color);//中点算法
	void Bresenham(CDC *pDC, int color);//Bresenham算法
	void dashline(CDC *pDC, int color);//虚线算法
};

