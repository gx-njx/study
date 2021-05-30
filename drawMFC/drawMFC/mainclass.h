#pragma once
#include "Line.h"
#include "Circle.h"
#include "GPolygon.h"
class mainclass
{
	CDC *pDC, b1DC, b2DC;//������壬�ڲ�����
	CBitmap bmp1, bmp2;//�ڲ���ͼ
	CRect rect;
	CArray<Line, Line&>linesarr;//��̬������
	CArray<Circle, Circle&>circlesarr;//��̬Բ����
	CArray<GPolygon, GPolygon&>polygonsarr;//��̬���������
	GPolygon clip;//�ü������
	int color, choose, alpre;//��ɫ,ѡ���������ͼ״̬
	bool pre, newpre, mmove;//��ͼ׼������ʼ����⣬�ƶ����
	CPoint mpup, mpdown, mpmove, mpdbc;//����ɿ������µ㣬�ƶ��㣬˫����
public:
	mainclass();//��ʼ��
	~mainclass();
	void setDC(CDC *dc);//�����������
	void setsize(CRect *rect);//���ÿͻ�����С
	void setmpup(CPoint mp);//��������ɿ���
	void setmpdown(CPoint mp);//������갴�µ�
	void setmpmove(CPoint mp);//����������λ��
	void setmpdbc(CPoint mp);//�������˫����
	void setcolor(COLORREF col);//������ɫ
	void setchoose(int cho);//����ѡ��
	void draw();//��ͼ
	void DCout();//���
	void redraw();//���»�ͼ
	void SeedFill(CPoint mp, int ncolor);//����Ƕ�����
	void ImgMove(CPoint mp1, CPoint mp2);//ƽ�Ʊ任
	void ImgRotate(CPoint mp1, CPoint mp2);//��ת�任
	void ImgScale(CPoint mp1, CPoint mp2);//���ű任
	void ImgSymmetry(CPoint mp1, CPoint mp2);//�ԳƱ任
	void rotate(CPoint &mp, double the, CPoint cen);//��ת��������
	void scale(CPoint &mp, double the, CPoint cen);//���Ÿ�������
	void symmetry(CPoint &mp, CPoint mp1, CPoint mp2);//�ԳƸ�������
	void ClipLine(CPoint mp1, CPoint mp2);//�߶βü�
	void ClipPolygon1(CPoint mp1, CPoint mp2);//���ζ���βü�
	void ClipPolygon(GPolygon &pgin, GPolygon &pgc);//�������βü�
	bool crossline1(CPoint sp1, CPoint ep1, CPoint sp2, CPoint ep2, CPoint &cp);//����ֱ�߽��㣨�������˵㣩
	bool crossline2(CPoint sp1, CPoint ep1, CPoint sp2, CPoint ep2, CPoint &cp);//����ֱ�߽��㣨�����˵㣩
};

