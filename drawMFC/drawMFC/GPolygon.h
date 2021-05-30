#pragma once

struct Edge
{
	int xs, xe, ys, ye;//起点和终点坐标
};

class GPolygon
{
public:
	CArray<CPoint, CPoint&>points;
	bool sure;//确定顶点个数
	int nsure;//顶点个数
	int incolor, fillcolor;
	GPolygon();
	GPolygon(int n);
	GPolygon(CPoint p);
	bool add(CPoint p);//添加顶点，自动闭合
	void draw(CDC *pDC, int color);//绘制边界
	void operator =(const GPolygon &gp);//复制（重载=）
	void scanfill(CDC *pDC, int color);//扫描线填充
	~GPolygon();
};

