#pragma once
class Circle
{
public:
	CPoint center;//Բ��
	int R;//�뾶
	int a, b;//������
	bool ellispe;//�Ƿ���Բ
	int incolor;
	Circle();
	Circle(CPoint po, int r);//Բ�İ뾶
	Circle(CPoint po1, CPoint po2);//�������Բ
	~Circle();
	void CirclePoints(int x, int y, int color, CDC *pDC);//���öԳƻ�Բ
	void midpoint(CDC *pDC, int color);//�е��㷨
	void Bresenham(CDC *pDC, int color);//Bresenham�㷨
	void EllipsePoints(int x, int y, int color, CDC *pDC);//���öԳƻ���Բ
	void Midpointellispe(CDC *pDC, int color);//�е㻭��Բ
};

