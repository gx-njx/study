#include "pch.h"
#include "Line.h"

Line::Line()
{
	incolor = -1;
}

Line::Line(CPoint sp, CPoint ep)
{
	stp = sp;
	endp = ep;
	incolor = -1;
}

Line::~Line()
{
}

void Line::DDA(CDC * pDC, int color)
{
	if (color == -1)color = incolor;
	else incolor = color;
	int dx = abs(endp.x - stp.x);
	int dy = abs(endp.y - stp.y);
	int s1, s2;
	if (endp.x >= stp.x) s1 = 1; else s1 = -1;
	if (endp.y >= stp.y) s2 = 1; else s2 = -1;//步进方向的确定

	float x = (float)stp.x;
	float y = (float)stp.y;

	float k = (float)dy / dx;
	if (k <= 1)
	{
		for (int i = 0; i <= dx; i++)
		{
			x += s1;
			pDC->SetPixel((int)x, (int)(y + 0.5), color);
			y += s2 * k;
		}
	}
	else
	{
		for (int i = 0; i <= dy; i++)
		{
			y += s2;
			pDC->SetPixel((int)(x + 0.5), (int)y, color);
			x += s1 * 1 / k;
		}
	}
}

void Line::midpoint(CDC * pDC, int color)
{
	if (color == -1)color = incolor;
	else incolor = color;
	int a = abs(endp.y - stp.y);
	int b = abs(endp.x - stp.x);

	int xs = 1; int ys = 1; int interchange;
	if (endp.x < stp.x) xs = -1;
	if (endp.y < stp.y) ys = -1;//步进方向修正

	if (a > b)
	{
		int temp = a;
		a = b;
		b = temp;
		interchange = 1;//以y轴为主轴进行步进
	}
	else interchange = 0;

	int x = stp.x;
	int y = stp.y;

	int d = -2 * a + b;//初始值
	int delt1 = -2 * a;//d>0增量
	int delt2 = 2 * (-a + b);//d<0增量

	for (int i = 0; i < b; i++)
	{
		pDC->SetPixel(x, y, color);
		if (d < 0)
		{
			x += xs; y += ys;
			d += delt2;
		}
		else
		{
			if (interchange == 0) x += xs; else y += ys;
			d += delt1;
		}

	}
}

void Line::Bresenham(CDC * pDC, int color)
{
	if (color == -1)
		color = incolor;
	else 
		incolor = color;
	int dx = abs(endp.x - stp.x);
	int dy = abs(endp.y - stp.y);
	int s1, s2, interchange;
	if (endp.x >= stp.x) s1 = 1; else s1 = -1;
	if (endp.y >= stp.y) s2 = 1; else s2 = -1;//步进方向

	if (dy > dx)
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;//以y轴为主轴进行步进
	}
	else interchange = 0;

	int f = -dx;//初始值
	int x = stp.x; int y = stp.y;

	for (int i = 0; i <= dx; i++)
	{
		pDC->SetPixel(x, y, color);
		if (f < 0)
		{
			if (interchange == 1) y += s2; else x += s1;
			f = f + 2 * dy;
		}
		if (f >= 0)
		{
			if (interchange == 1) x += s1; else y += s2;
			f = f - 2 * dx;
		}
	}
}

void Line::dashline(CDC * pDC, int color)
{
	if (color == -1)color = incolor;
	else incolor = color;
	int dx = abs(endp.x - stp.x);
	int dy = abs(endp.y - stp.y);
	int s1, s2, interchange;
	if (endp.x >= stp.x) s1 = 1; else s1 = -1;
	if (endp.y >= stp.y) s2 = 1; else s2 = -1;//步进方向

	if (dy > dx)
	{
		int temp = dx;
		dx = dy;
		dy = temp;
		interchange = 1;//以y轴为主轴进行步进
	}
	else interchange = 0;

	int f = -dx;//初始值
	int x = stp.x; int y = stp.y;

	int n = 0;//虚线记录

	for (int i = 0; i <= dx; i++)
	{
		n++;
		if (n > 6)n = 1;
		if (n < 4)pDC->SetPixel(x, y, color);//当n<4时画点
		if (f < 0)
		{
			if (interchange == 1) y += s2; else x += s1;
			f = f + 2 * dy;
		}
		if (f >= 0)
		{
			if (interchange == 1) x += s1; else y += s2;
			f = f - 2 * dx;
		}
	}
}
