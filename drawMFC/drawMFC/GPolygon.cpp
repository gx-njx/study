#include "pch.h"
#include "GPolygon.h"

GPolygon::GPolygon()
{
	sure = false;
	nsure = 0;
	incolor = -1;
	fillcolor = -1;
}

GPolygon::GPolygon(int n)
{
	if (n < 3)sure = false; return;
	points.SetSize(n);
	nsure = n;
	sure = true;
	incolor = -1;
	fillcolor = -1;
}

GPolygon::GPolygon(CPoint p)
{
	points.Add(p);
	incolor = -1;
	fillcolor = -1;
}

bool GPolygon::add(CPoint p)
{
	if (points.GetSize() < 3) { points.Add(p); return true; }
	if (fabs(p.x - points[0].x) < 10 && fabs(p.y - points[0].y) < 10)return false;
	if (points.GetSize() == nsure - 1) { points.Add(p); return false; }
	points.Add(p); return true;
}

void GPolygon::draw(CDC * pDC, int color)
{
	if (color == -1)color = incolor;
	else incolor = color;
	if (fillcolor != -1)scanfill(pDC, fillcolor);
	if (points.GetSize() < 2)return;
	pDC->MoveTo(points[0]);
	for (int i = 1; i < points.GetSize(); i++)
	{
		pDC->LineTo(points[i]);
	}
	pDC->LineTo(points[0]);
}

void GPolygon::operator=(const GPolygon & gp)
{
	incolor = gp.incolor;
	fillcolor = gp.fillcolor;
	nsure = gp.nsure;
	sure = gp.sure;
	points.Copy(gp.points);
}

void GPolygon::scanfill(CDC * pDC, int color)
{
	fillcolor = color;
	int xmax, xmin, ymax, ymin;
	ymax = ymin = points[0].y; xmin = xmax = points[0].x;
	Edge *edges = new Edge[points.GetSize()];
	for (int i = 0; i < points.GetSize(); i++)//建立边表，记录极值
	{
		xmax = xmax > points[i].x ? xmax : points[i].x;
		xmin = xmin < points[i].x ? xmin : points[i].x;
		ymax = ymax > points[i].y ? ymax : points[i].y;
		ymin = ymin < points[i].y ? ymin : points[i].y;
		if (i == points.GetSize() - 1)
		{
			edges[i].xs = points[i].x;
			edges[i].ys = points[i].y;
			edges[i].xe = points[0].x;
			edges[i].ye = points[0].y;
			continue;
		}
		edges[i].xs = points[i].x;
		edges[i].ys = points[i].y;
		edges[i].xe = points[i + 1].x;
		edges[i].ye = points[i + 1].y;
	}
	xmin--; ymin--; xmax++; ymax++;
	int *cx = new int[points.GetSize()];//交点x
	for (int scany = ymin; scany < ymax; scany++)//每一条扫描线
	{
		int n = 0;
		for (int i = 0; i < points.GetSize(); i++)//寻找交点
		{
			if (scany == edges[i].ys)//顶点
			{
				int ii = i - 1;
				if (ii < 0)ii = points.GetSize() - 1;
				if (edges[i].ye == edges[i].ys)
				{
					i += 1;
					if (i == points.GetSize())i = 0;
				}
				if (max(edges[i].ye, edges[ii].ys) > edges[i].ys && min(edges[i].ye, edges[ii].ys) < edges[i].ys)
				{
					cx[n] = edges[i].xs;
					n++;
				}
				continue;
			}
			int tema = max(edges[i].ys, edges[i].ye);
			int temi = min(edges[i].ys, edges[i].ye);
			if (scany<tema && scany>temi)//找到交点
			{
				cx[n] = int(edges[i].xe + (edges[i].xs - edges[i].xe)*(scany - edges[i].ye) / double(edges[i].ys - edges[i].ye) + 0.5);
				n++;
			}
		}
		for (int i = 0; i < n; i++)//排序
		{
			for (int ii = i + 1; ii < n; ii++)
			{
				if (cx[i] > cx[ii])
				{
					int tem = cx[i];
					cx[i] = cx[ii];
					cx[ii] = tem;
				}
			}
		}
		for (int i = 0; i < n; i += 2)//画线
		{
			for (int ii = cx[i]; ii < cx[i + 1]; ii++)
			{
				pDC->SetPixel(ii, scany, color);
			}
		}
	}
	delete[] edges;
	delete[] cx;
}

GPolygon::~GPolygon()
{
}
