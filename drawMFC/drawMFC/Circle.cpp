#include "pch.h"
#include "Circle.h"

Circle::Circle()
{
	incolor = -1;
}

Circle::Circle(CPoint po, int r)
{
	center = po;
	R = r;
	ellispe = false;
	incolor = -1;
}

Circle::Circle(CPoint po1, CPoint po2)
{
	center.x = int((po1.x + po2.x)*0.5 + 0.5);
	center.y = int((po1.y + po2.y)*0.5 + 0.5);
	R = int(max(abs(po1.x - po2.x), abs(po1.y - po2.y))*0.5 + 0.5);
	a = int(abs(po1.x - po2.x)*0.5 + 0.5);
	b = int(abs(po1.y - po2.y)*0.5 + 0.5);
	ellispe = true;//��Բ
	incolor = -1;
}

Circle::~Circle()
{
}

void Circle::CirclePoints(int x, int y, int color, CDC * pDC)
{
	if (color == -1)color = incolor;//��¼��ǰ����ɫ
	else incolor = color;
	//�㷨ֻ��Ҫ��1/8��Բ���������öԳƻ���
	//�����ڳ�������ʱ�Զ�����
	pDC->SetPixel(center.x + x, center.y + y, color);
	pDC->SetPixel(center.x - x, center.y + y, color);
	pDC->SetPixel(center.x + x, center.y - y, color);
	pDC->SetPixel(center.x - x, center.y - y, color);
	pDC->SetPixel(center.x + y, center.y + x, color);
	pDC->SetPixel(center.x - y, center.y + x, color);
	pDC->SetPixel(center.x + y, center.y - x, color);
	pDC->SetPixel(center.x - y, center.y - x, color);
}

void Circle::midpoint(CDC * pDC, int color)
{
	float d;
	int x = 0; int y = R;//�ȼ���Բ����ԭ��
	d = 1.25f - R;//��ʼֵ
	CirclePoints(x, y, color, pDC);
	while (x <= y)//�ڵ�һ���޵��ϰ���ķ�֮һԲ�ڣ���ֻҪ1/8��Բ
	{
		if (d < 0)
			d += 2 * x + 3;
		else
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		CirclePoints(x, y, color, pDC);
	}
}

void Circle::Bresenham(CDC * pDC, int color)
{
	int x, y, d;
	x = 0; y = R;//�ȼ���Բ����ԭ��
	d = 3 - 2 * R;//��ʼֵ
	while (x < y)//�ڵ�һ���޵��ϰ���ķ�֮һԲ�ڣ���ֻҪ1/8��Բ
	{
		CirclePoints(x, y, color, pDC);
		if (d < 0)
			d = d + 4 * x + 6;
		else
		{
			d = d + 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
	if (x == y)
		CirclePoints(x, y, color, pDC);
}

void Circle::EllipsePoints(int x, int y, int color, CDC * pDC)
{
	if (color == -1)color = incolor;//��¼��ǰ����ɫ
	else incolor = color;
	//�ĸ����޶Գ�
	pDC->SetPixel(center.x + x, center.y + y, color);
	pDC->SetPixel(center.x - x, center.y + y, color);
	pDC->SetPixel(center.x - x, center.y - y, color);
	pDC->SetPixel(center.x + x, center.y - y, color);
}

void Circle::Midpointellispe(CDC * pDC, int color)
{
	int x, y;
	double d1, d2;
	x = 0;
	y = b;
	d1 = b * b + a * a*(-b + 0.25);//��ʼֵ
	EllipsePoints(x, y, color, pDC);
	while (b*b*(x + 1) < a*a*(y - 0.5))//����б��Ϊ�C1 �ĵ㴦���������һ���㣬���ȺŸı䷽����˵����Բ�����ϲ���ת���²���
	{
		if (d1 <= 0)
		{
			d1 += b * b*(2 * x + 3);
		}
		else
		{
			d1 += b * b*(2 * x + 3) + a * a*(-2 * y + 2);
			y--;
		}
		x++;
		EllipsePoints(x, y, color, pDC);
	}
	//�²�����Բ
	d2 = sqrt(b*(x + 0.5)) + a * (y - 1) - a * b;//��ʼֵ
	while (y > 0)
	{
		if (d2 < 0)
		{
			d2 += b * b*(2 * x + 2) + a * a*(-2 * y + 3);
			x++;
		}
		else
		{
			d2 += a * a*(-2 * y + 3);
		}
		y--;
		EllipsePoints(x, y, color, pDC);
	}
}
