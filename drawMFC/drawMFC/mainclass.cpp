#include "pch.h"
#include "mainclass.h"
#include <math.h>

const double pi = atan(1) * 4;

mainclass::mainclass()
{
	mmove = false;
	pre = false;
	newpre = true;
	alpre = -1;
	choose = 0;
	color = RGB(100, 100, 100);
}

mainclass::~mainclass()
{
}

void mainclass::setDC(CDC *dc)
{
	pDC = dc;
}

void mainclass::setsize(CRect * rect)
{
	mainclass::rect = rect;
	if (newpre)
	{
		b1DC.CreateCompatibleDC(pDC);//��������
		b2DC.CreateCompatibleDC(pDC);
		bmp1.CreateCompatibleBitmap(pDC, rect->Width(), rect->Height());//������Ӧ��С��λͼ
		b1DC.SelectObject(&bmp1);
		b1DC.FillSolidRect(0, 0, rect->Width(), rect->Height(), RGB(255, 255, 255));//����ɫ
		bmp2.CreateCompatibleBitmap(pDC, rect->Width(), rect->Height());
		b2DC.SelectObject(&bmp2);
		b2DC.FillSolidRect(0, 0, rect->Width(), rect->Height(), RGB(255, 255, 255));
		newpre = false;
	}
	else
	{
		bmp2.DeleteObject();
		bmp2.CreateCompatibleBitmap(&b2DC, rect->Width(), rect->Height());
		b2DC.SelectObject(&bmp2);
		b2DC.FillSolidRect(0, 0, rect->Width(), rect->Height(), RGB(255, 255, 255));
		b2DC.BitBlt(0, 0, rect->Width(), rect->Height(), &b1DC, 0, 0, SRCCOPY);
		bmp1.DeleteObject();
		bmp1.CreateCompatibleBitmap(&b1DC, rect->Width(), rect->Height());
		b1DC.SelectObject(&bmp1);
		b1DC.BitBlt(0, 0, rect->Width(), rect->Height(), &b2DC, 0, 0, SRCCOPY);//���ƻ�������
		pDC->BitBlt(0, 0, rect->Width(), rect->Height(), &b1DC, 0, 0, SRCCOPY);
	}
}

void mainclass::setmpup(CPoint mp)
{
	if (!pre)return;
	if (alpre != -1)return;
	mmove = false;
	mpup = mp;
	draw();
	//pre = false;
}

void mainclass::setmpdown(CPoint mp)
{
	if (!pre)return;
	if (alpre!=-1)
	{
		mpdown = mp;
		mmove = false;
		draw();
		mmove = true;
		return;
	}
	mpdown = mp;
	mmove = true;
}

void mainclass::setmpmove(CPoint mp)
{
	if (!mmove)return;
	mpmove = mp;
	draw();
}

void mainclass::setmpdbc(CPoint mp)
{
	if (!pre)return;
	mpdbc = mp;
	alpre = 1;
	draw();
}

void mainclass::setcolor(COLORREF col)
{
	color = col;
}

void mainclass::setchoose(int cho)
{
	choose = cho;
	pre = true;
	mmove = false;
	if (cho == 1 || cho == 2 || cho == 3 || cho == 7)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ���꼴��ʼ���ߣ��ɿ���������"));
	}
	if (cho == 4 || cho == 5)
	{
		pDC->TextOutW(0, 0, _T("������������ȷ��Բ�ģ��ƶ���꿪ʼ��Բ���ɿ���������"));
	}
	if (cho == 6)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ���꼴��ʼ����Բ���ɿ���������"));
	}
	if (cho == 8)
	{
		pre = false;
		draw();
	}
	if (cho == 9) 
	{ 
		pDC->TextOutW(0, 0, _T("��������������˫�������������������䡣"));
		alpre = 0; 
	}
	if (cho == 10)
	{
		pDC->TextOutW(0, 0, _T("������������ȷ��һ�����㣬�����һ�����㸽���Զ��պϡ�"));
		alpre = 0;
	}
	if (cho == 11)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ���꼴��ʼ�ƶ����ɿ���������"));
	}
	if (cho == 12)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ���꼴ȷ����ת�ǣ��ɿ���������"));
	}
	if (cho == 13)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ���꼴ȷ�����ű�����������С���ҷŴ��ɿ���������"));
	}
	if (cho == 14)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ���꼴ȷ���Գ��ᣬ�ɿ������ɶԳƱ任��"));
	}
	if (cho == 15)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ������ֲü����ɿ������ɲü���"));
	}
	if (cho == 16)
	{
		pDC->TextOutW(0, 0, _T("���������²����ƶ������ֲü����ɿ������ɲü���"));
	}
	if (cho == 17)
	{
		pDC->TextOutW(0, 0, _T("�����������ʼ���Ʋü�����Σ���ɶ���λ��ƺ��Զ����ֽ����"));
		alpre = 0;
	}
}

void mainclass::draw()
{
	switch (choose)
	{
	case 0:
		pre = false;
		mmove = false;
		alpre = -1;
		return;
		break;
	case 1://ddaֱ��
		if (!mmove)
		{
			Line tem(mpdown, mpup);
			linesarr.Add(tem);
			tem.DDA(&b1DC, color);
		}
		else
		{
			Line tem(mpdown, mpmove);
			tem.DDA(&b1DC, color);
			tem.~Line();
		}
		break;
	case 2://�е�ֱ��
		if (!mmove)
		{
			Line tem(mpdown, mpup);
			linesarr.Add(tem);
			tem.midpoint(&b1DC, color);
		}
		else
		{
			Line tem(mpdown, mpmove);
			tem.midpoint(&b1DC, color);
			tem.~Line();
		}
		break;
	case 3://Bresenhamֱ��
		if (!mmove)
		{
			Line tem(mpdown, mpup);
			linesarr.Add(tem);
			tem.Bresenham(&b1DC, color);
		}
		else
		{
			Line tem(mpdown, mpmove);
			tem.Bresenham(&b1DC, color);
			tem.~Line();
		}
		break;
	case 4://�е㻭Բ
		if (!mmove)
		{
			Circle tem(mpdown, int(sqrt(pow(mpdown.x - mpup.x, 2) + pow(mpdown.y - mpup.y, 2))));
			circlesarr.Add(tem);
			tem.midpoint(&b1DC, color);
		}
		else
		{
			Circle tem(mpdown, int(sqrt(pow(mpdown.x - mpmove.x, 2) + pow(mpdown.y - mpmove.y, 2))));
			tem.midpoint(&b1DC, color);
			tem.~Circle();
		}
		break;
	case 5://Bresenham��Բ
		if (!mmove)
		{
			Circle tem(mpdown, int(sqrt(pow(mpdown.x - mpup.x, 2) + pow(mpdown.y - mpup.y, 2))));
			circlesarr.Add(tem);
			tem.Bresenham(&b1DC, color);
		}
		else
		{
			Circle tem(mpdown, int(sqrt(pow(mpdown.x - mpmove.x, 2) + pow(mpdown.y - mpmove.y, 2))));
			tem.Bresenham(&b1DC, color);
			tem.~Circle();
		}
		break;
	case 6://�е㻭��Բ
		if (!mmove)
		{
			Circle tem(mpdown, mpup);
			circlesarr.Add(tem);
			tem.Midpointellispe(&b1DC, color);
		}
		else
		{
			Circle tem(mpdown, mpmove);
			tem.Midpointellispe(&b1DC, color);
			tem.~Circle();
		}
		break;
	case 7://����
		if (!mmove)
		{
			Line tem(mpdown, mpup);
			linesarr.Add(tem);
			tem.dashline(&b1DC, color);
		}
		else
		{
			Line tem(mpdown, mpmove);
			tem.dashline(&b1DC, color);
			tem.~Line();
		}
		break;
	case 8://ɨ�������
		if (!polygonsarr.GetSize())
		{
			MessageBox(MB_OK, _T("û�ж���ο���䣡"), _T("����"), 0);
		}
		else
		{
			for (int i = 0; i < polygonsarr.GetSize(); i++)
			{
				polygonsarr[i].scanfill(&b1DC, color);
			}
		}
		break;
	case 9://�������
		if (alpre == 1) {
			SeedFill(mpdbc, b1DC.GetPixel(mpdbc));
			pre = false;
			mmove = false;
			alpre = -1;
		}
		break;
	case 10://�������
		if (alpre == 0)
		{
			GPolygon tem(mpdown);
			polygonsarr.Add(tem);
			alpre = 1;
			return;
		}
		if (!mmove)
		{
			b1DC.LineTo(mpdown);
			bool b = polygonsarr[polygonsarr.GetUpperBound()].add(mpdown);
			if (!b)
			{
				alpre = -1;
				choose = 0;
				pre = false;
				mmove = false;
				redraw();
			}
		}
		else
		{
			b1DC.MoveTo(mpdown);
			b1DC.LineTo(mpmove);
			b1DC.MoveTo(mpdown);
		}
		break;
	case 11://ƽ��
		if (!mmove)
		{
			ImgMove(mpdown, mpup);
			redraw();
			return;
		}
		else
		{
			ImgMove(mpdown, mpmove);
			redraw();
			ImgMove(mpmove, mpdown);
			return;
		}
		break;
	case 12://��ת
		if (!mmove)
		{
			ImgRotate(mpdown, mpup);
			redraw();
			return;
		}
		else
		{
			ImgRotate(mpdown, mpmove);
			redraw();
			mpmove.y -= 2 * (mpmove.y - mpdown.y);
			ImgRotate(mpdown, mpmove);
			return;
		}
		break;
	case 13://����
		if (!mmove)
		{
			ImgScale(mpdown, mpup);
			redraw();
			return;
		}
		else
		{
			ImgScale(mpdown, mpmove);
			redraw();
			mpmove.x = 2 * mpdown.x - mpmove.x;
			mpmove.y = 2 * mpdown.y - mpmove.y;
			ImgScale(mpdown, mpmove);
			return;
		}
		break;
	case 14://�Գ�
		if (!mmove)
		{
			ImgSymmetry(mpdown, mpup);
			redraw();
			return;
		}
		else
		{
			//ImgSymmetry(mpdown, mpmove);
			//redraw();
			b1DC.MoveTo(mpdown);
			b1DC.LineTo(mpmove);
			//ImgSymmetry(mpdown, mpmove);
			//return;
		}
		break;
	case 15://�߶βü�
		if (!mmove)
		{
			ClipLine(mpdown, mpup);
			redraw();
			return;
		}
		else
		{
			b1DC.MoveTo(mpdown);
			b1DC.LineTo(mpdown.x, mpmove.y);
			b1DC.LineTo(mpmove);
			b1DC.LineTo(mpmove.x, mpdown.y);
			b1DC.LineTo(mpdown);
		}
		break;
	case 16://���ζ���βü�
		if (!polygonsarr.GetSize())
		{
			MessageBox(MB_OK, _T("û�ж���οɼ��ã�"), _T("����"), 0);
			alpre = -1;
			choose = 0;
			pre = false;
			mmove = false;
			return;
		}
		if (!mmove)
		{
			ClipPolygon1(mpdown, mpup);
			redraw();
			return;
		}
		else
		{
			b1DC.MoveTo(mpdown);
			b1DC.LineTo(mpdown.x, mpmove.y);
			b1DC.LineTo(mpmove);
			b1DC.LineTo(mpmove.x, mpdown.y);
			b1DC.LineTo(mpdown);
		}
		break;
	case 17://�������βü�
		if (!polygonsarr.GetSize())
		{
			MessageBox(MB_OK, _T("û�ж���οɼ��ã�"), _T("����"), 0);
			alpre = -1;
			choose = 0;
			pre = false;
			mmove = false;
			return;
		}
		if (alpre == 0)
		{
			clip.add(mpdown);
			alpre = 1;
			return;
		}
		if (!mmove)
		{
			b1DC.LineTo(mpdown);
			bool b = clip.add(mpdown);
			if (!b)
			{
				alpre = -1;
				choose = 0;
				pre = false;
				mmove = false;
				int n = polygonsarr.GetSize();
				if (n)
				{
					for (int i = 0; i < n; i++)
					{
						ClipPolygon(polygonsarr[i], clip);
					}
				}
				redraw();
				clip.points.RemoveAll();
				return;
			}
		}
		else
		{
			b1DC.MoveTo(mpdown);
			b1DC.LineTo(mpmove);
			b1DC.MoveTo(mpdown);
		}
		break;
	}
	DCout();
}

void mainclass::DCout()
{
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &b1DC, 0, 0, SRCCOPY);
	if (mmove)
	{
		b1DC.BitBlt(0, 0, rect.Width(), rect.Height(), &b2DC, 0, 0, SRCCOPY);
	}
	else
	{
		b2DC.BitBlt(0, 0, rect.Width(), rect.Height(), &b1DC, 0, 0, SRCCOPY);
	}
}

void mainclass::redraw()
{
	b1DC.FillSolidRect(0, 0, rect.Width(), rect.Height(), RGB(255, 255, 255));
	if (int n = linesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			linesarr[i].Bresenham(&b1DC, -1);
		}
	}
	if (int n = circlesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			if(circlesarr[i].ellispe)circlesarr[i].Midpointellispe(&b1DC, -1);
			else circlesarr[i].Bresenham(&b1DC, -1);
		}
	}
	if (int n = polygonsarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			polygonsarr[i].draw(&b1DC, -1);
		}
	}
	DCout();
}

void mainclass::SeedFill(CPoint mp, int ncolor)
{
	CList<CPoint, CPoint&>seeds;
	seeds.AddTail(mp);//��ӵ�һ�����ӵ�
	while (!seeds.IsEmpty())//��������Ϊ��
	{
		CPoint tem = seeds.RemoveHead();//ɾ����ǰ������ӵ�
		if (b1DC.GetPixel(tem) == ncolor)//�õ�Ϊ������ɫ��������Ҫ���ĵ�
		{
			b1DC.SetPixel(tem, color);
			tem.x++;
			if (b1DC.GetPixel(tem) == ncolor)seeds.AddTail(tem);
			tem.x -= 2;
			if (b1DC.GetPixel(tem) == ncolor)seeds.AddTail(tem);
			tem.x++; tem.y++;
			if (b1DC.GetPixel(tem) == ncolor)seeds.AddTail(tem);
			tem.y -= 2;
			if (b1DC.GetPixel(tem) == ncolor)seeds.AddTail(tem);
		}
	}
}

void mainclass::ImgMove(CPoint mp1, CPoint mp2)
{
	int dx = mp2.x - mp1.x;
	int dy = mp2.y - mp1.y;
	if (int n = linesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			linesarr[i].stp.x += dx;
			linesarr[i].stp.y += dy;
			linesarr[i].endp.x += dx;
			linesarr[i].endp.y += dy;
		}
	}
	if (int n = circlesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			circlesarr[i].center.x += dx;
			circlesarr[i].center.y += dy;
		}
	}
	if (int n = polygonsarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			for (int ii = 0; ii < polygonsarr[i].points.GetSize(); ii++)
			{
				polygonsarr[i].points[ii].x += dx;
				polygonsarr[i].points[ii].y += dy;
			}
		}
	}
}

void mainclass::ImgRotate(CPoint mp1, CPoint mp2)
{
	int dx = mp2.x - mp1.x;
	int dy = mp2.y - mp1.y;
	double the = -atan2(dy, dx);//��ת�Ƕ�
	if (int n = linesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			rotate(linesarr[i].stp, the, mp1);
			rotate(linesarr[i].endp, the, mp1);
		}
	}
	if (int n = circlesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			rotate(circlesarr[i].center, the, mp1);
		}
	}
	if (int n = polygonsarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			for (int ii = 0; ii < polygonsarr[i].points.GetSize(); ii++)
			{
				rotate(polygonsarr[i].points[ii], the, mp1);
			}
		}
	}
}

void mainclass::ImgScale(CPoint mp1, CPoint mp2)
{
	double m = (fabs(mp2.x - mp1.x) + 100) / 100.0;//���ű���
	if (mp2.x - mp1.x < 0)m = 1.0 / m;
	if (int n = linesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			scale(linesarr[i].stp, m, mp1);
			scale(linesarr[i].endp, m, mp1);
		}
	}
	if (int n = circlesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			scale(circlesarr[i].center, m, mp1);
			circlesarr[i].R = int(circlesarr[i].R*m + 0.5);
			circlesarr[i].a = int(circlesarr[i].a*m + 0.5);
			circlesarr[i].b = int(circlesarr[i].b*m + 0.5);
		}
	}
	if (int n = polygonsarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			for (int ii = 0; ii < polygonsarr[i].points.GetSize(); ii++)
			{
				scale(polygonsarr[i].points[ii], m, mp1);
			}
		}
	}
}

void mainclass::ImgSymmetry(CPoint mp1, CPoint mp2)
{
	if (int n = linesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			symmetry(linesarr[i].stp, mp1, mp2);
			symmetry(linesarr[i].endp, mp1, mp2);
		}
	}
	if (int n = circlesarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			symmetry(circlesarr[i].center, mp1, mp2);
		}
	}
	if (int n = polygonsarr.GetSize())
	{
		for (int i = 0; i < n; i++)
		{
			for (int ii = 0; ii < polygonsarr[i].points.GetSize(); ii++)
			{
				symmetry(polygonsarr[i].points[ii], mp1, mp2);
			}
		}
	}
}

void mainclass::rotate(CPoint &mp, double the, CPoint cen)
{
	int x0 = mp.x - cen.x;
	int y0 = mp.y - cen.y;
	mp.x = int(x0 * cos(the) + y0 * sin(the) + cen.x + 0.5);
	mp.y = int(y0 * cos(the) - x0 * sin(the) + cen.y + 0.5);
}

void mainclass::scale(CPoint & mp, double the, CPoint cen)
{
	int x0 = mp.x - cen.x;
	int y0 = mp.y - cen.y;
	mp.x = int(x0*the + cen.x + 0.5);
	mp.y = int(y0*the + cen.y + 0.5);
}

void mainclass::symmetry(CPoint & mp, CPoint mp1, CPoint mp2)
{
	double the1 = atan2(mp2.y - mp1.y, mp2.x - mp1.x);
	double the2 = atan2(mp.y - mp1.y, mp.x - mp1.x);
	double the = 2 * (the2 - the1);
	rotate(mp, the, mp1);
}

void mainclass::ClipLine(CPoint mp1, CPoint mp2)
{
	int xmax = max(mp1.x, mp2.x);
	int xmin = min(mp1.x, mp2.x);
	int ymax = max(mp1.y, mp2.y);
	int ymin = min(mp1.y, mp2.y);
	if (!linesarr.GetSize())return;
	CArray<Line, Line&>newlines;
	for (int i = 0; i < linesarr.GetSize(); i++)
	{
		int x1 = linesarr[i].stp.x;
		int y1 = linesarr[i].stp.y;
		int x2 = linesarr[i].endp.x;
		int y2 = linesarr[i].endp.y;
		//��ȫ�ڲü�������
		if (min(x1, x2) >= xmax)continue;
		if (min(y1, y2) >= ymax)continue;
		if (max(x1, x2) <= xmin)continue;
		if (max(y1, y2) <= ymin)continue;
		//��ȫ�ڲü�������
		if (min(x1, x2) >= xmin && max(x1, x2) <= xmax && min(y1, y2) >= ymin && max(y1, y2) <= ymax)
		{
			newlines.Add(linesarr[i]);
			continue;
		}
		//��ʼ�󽻵�
		bool in = false;
		CPoint sp1, ep1, cop;
		sp1.SetPoint(xmax, ymax);
		ep1.SetPoint(xmax, ymin);//�ұ߽�
		if (crossline2(sp1, ep1, linesarr[i].stp, linesarr[i].endp, cop))
		{
			if (linesarr[i].stp.x > linesarr[i].endp.x)
			{
				linesarr[i].stp = cop;
			}
			else
			{
				linesarr[i].endp = cop;
			}
			in = true;
		}
		ep1.SetPoint(xmin, ymax);//�ϱ߽�
		if (crossline2(sp1, ep1, linesarr[i].stp, linesarr[i].endp, cop))
		{
			if (linesarr[i].stp.y > linesarr[i].endp.y)
			{
				linesarr[i].stp = cop;
			}
			else
			{
				linesarr[i].endp = cop;
			}
			in = true;
		}
		sp1.SetPoint(xmin, ymax);
		ep1.SetPoint(xmin, ymin);//��߽�
		if (crossline2(sp1, ep1, linesarr[i].stp, linesarr[i].endp, cop))
		{
			if (linesarr[i].stp.x < linesarr[i].endp.x)
			{
				linesarr[i].stp = cop;
			}
			else
			{
				linesarr[i].endp = cop;
			}
			in = true;
		}
		sp1.SetPoint(xmax, ymin);//�±߽�
		if (crossline2(sp1, ep1, linesarr[i].stp, linesarr[i].endp, cop))
		{
			if (linesarr[i].stp.y < linesarr[i].endp.y)
			{
				linesarr[i].stp = cop;
			}
			else
			{
				linesarr[i].endp = cop;
			}
			in = true;
		}
		if (in)newlines.Add(linesarr[i]);
	}
	linesarr.RemoveAll();
	linesarr.Copy(newlines);
}

bool mainclass::crossline1(CPoint sp1, CPoint ep1, CPoint sp2, CPoint ep2, CPoint & cp)
{
	//y=kx+b
	double dx1 = ep1.x - sp1.x;
	double dy1 = ep1.y - sp1.y;
	double dx2 = ep2.x - sp2.x;
	double dy2 = ep2.y - sp2.y;
	if (dx1 == 0)dx1 = 1e-10;
	if (dx2 == 0)dx2 = 1e-10;
	double k1 = dy1 / dx1;
	double k2 = dy2 / dx2;
	if (k1 == k2)return false;
	double b1 = sp1.y - k1 * sp1.x;
	double b2 = sp2.y - k2 * sp2.x;
	//k1x+b1=k2x+b2
	cp.x = int((b2 - b1) / (k1 - k2) + 0.5);
	cp.y = int(k2 * cp.x + b2 + 0.5);
	//if (cp.y >= max(sp1.y, ep1.y) || cp.y <= min(sp1.y, ep1.y))return false;
	//if (cp.x >= max(sp1.x, ep1.x) || cp.x <= min(sp1.x, ep1.x))return false;
	//if (cp.y >= max(sp2.y, ep2.y) || cp.y <= min(sp2.y, ep2.y))return false;
	//if (cp.x >= max(sp2.x, ep2.x) || cp.x <= min(sp2.x, ep2.x))return false;
	if (cp.y > max(sp1.y, ep1.y) || cp.y < min(sp1.y, ep1.y))return false;
	if (cp.x > max(sp1.x, ep1.x) || cp.x < min(sp1.x, ep1.x))return false;
	if (cp.y > max(sp2.y, ep2.y) || cp.y < min(sp2.y, ep2.y))return false;
	if (cp.x > max(sp2.x, ep2.x) || cp.x < min(sp2.x, ep2.x))return false;
	if (cp.x == sp1.x && cp.y == sp1.y)return false;
	if (cp.x == sp2.x && cp.y == sp2.y)return false;
	if (cp.x == ep1.x && cp.y == ep1.y)return false;
	if (cp.x == ep2.x && cp.y == ep2.y)return false;
	return true;
}

bool mainclass::crossline2(CPoint sp1, CPoint ep1, CPoint sp2, CPoint ep2, CPoint & cp)
{
	//y=kx+b
	double dx1 = ep1.x - sp1.x;
	double dy1 = ep1.y - sp1.y;
	double dx2 = ep2.x - sp2.x;
	double dy2 = ep2.y - sp2.y;
	if (dx1 == 0)dx1 = 1e-10;
	if (dx2 == 0)dx2 = 1e-10;
	double k1 = dy1 / dx1;
	double k2 = dy2 / dx2;
	if (k1 == k2)return false;
	double b1 = sp1.y - k1 * sp1.x;
	double b2 = sp2.y - k2 * sp2.x;
	//k1x+b1=k2x+b2
	cp.x = int((b2 - b1) / (k1 - k2) + 0.5);
	cp.y = int(k2 * cp.x + b2 + 0.5);
	if (cp.y > max(sp1.y, ep1.y) || cp.y < min(sp1.y, ep1.y))return false;
	if (cp.x > max(sp1.x, ep1.x) || cp.x < min(sp1.x, ep1.x))return false;
	if (cp.y > max(sp2.y, ep2.y) || cp.y < min(sp2.y, ep2.y))return false;
	if (cp.x > max(sp2.x, ep2.x) || cp.x < min(sp2.x, ep2.x))return false;
	return true;
}
