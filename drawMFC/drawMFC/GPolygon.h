#pragma once

struct Edge
{
	int xs, xe, ys, ye;//�����յ�����
};

class GPolygon
{
public:
	CArray<CPoint, CPoint&>points;
	bool sure;//ȷ���������
	int nsure;//�������
	int incolor, fillcolor;
	GPolygon();
	GPolygon(int n);
	GPolygon(CPoint p);
	bool add(CPoint p);//��Ӷ��㣬�Զ��պ�
	void draw(CDC *pDC, int color);//���Ʊ߽�
	void operator =(const GPolygon &gp);//���ƣ�����=��
	void scanfill(CDC *pDC, int color);//ɨ�������
	~GPolygon();
};

