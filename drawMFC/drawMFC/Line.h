#pragma once
class Line
{
public:
	CPoint stp, endp;
	int incolor;
	Line();
	Line(CPoint sp, CPoint ep);//���캯��������ֹ��
	~Line();
	void DDA(CDC *pDC, int color);//DDA�㷨
	void midpoint(CDC *pDC, int color);//�е��㷨
	void Bresenham(CDC *pDC, int color);//Bresenham�㷨
	void dashline(CDC *pDC, int color);//�����㷨
};

