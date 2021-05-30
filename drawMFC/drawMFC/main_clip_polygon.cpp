#include "pch.h"
#include "mainclass.h"

struct clipPoint
{
	CPoint point;
	clipPoint *drop;//˫��ָ��
	clipPoint *next;//��һ����
	bool follow;//�Ƿ񱻸���
	bool cross;//�Ƿ�Ϊ����
};

void mainclass::ClipPolygon(GPolygon & pgin, GPolygon &pgc)
{
	CList<clipPoint, clipPoint>polyin;
	CList<clipPoint, clipPoint>polyclip;
	//��������ε�ѭ��������
	if (pgin.points.GetSize())
	{
		for (int i = 0; i < pgin.points.GetSize(); i++)
		{
			clipPoint tem;
			tem.point = pgin.points[i];
			tem.cross = false;
			tem.follow = false;
			polyin.AddTail(tem);
		}
		POSITION pos = polyin.GetHeadPosition();
		clipPoint *tem;
		while (pos!=NULL)
		{
			tem = &polyin.GetNext(pos);
			if (pos == NULL)
			{
				tem->next = &polyin.GetHead();
				break;
			}
			tem->next = &polyin.GetAt(pos);
		}
	}	
	//�������ö���ε�ѭ��������
	if (pgc.points.GetSize())
	{
		for (int i = 0; i < pgc.points.GetSize(); i++)
		{
			clipPoint tem;
			tem.point = pgc.points[i];
			tem.cross = false;
			tem.follow = false;
			polyclip.AddTail(tem);
		}
		POSITION pos = polyclip.GetHeadPosition();
		clipPoint *tem;
		while (pos != NULL)
		{
			tem = &polyclip.GetNext(pos);
			if (pos == NULL)
			{
				tem->next = &polyclip.GetHead();
				break;
			}
			tem->next = &polyclip.GetAt(pos);
		}
	}
	//Ѱ�ҽ��㣬����������˫��ָ��
	POSITION posi1, posi2;
	clipPoint *intem1, *intem2, *cltem1, *cltem2;
	posi1 = polyin.GetHeadPosition();
	bool findcross = false;
	while (posi1 != NULL)
	{
		bool end0 = false;
		intem1 = &polyin.GetNext(posi1);
		if (posi1 == NULL)
		{
			intem2 = &polyin.GetHead();
			end0 = true;
		}
		else
		{
			intem2 = &polyin.GetAt(posi1);
		}
		posi2 = polyclip.GetHeadPosition();
		while (posi2 != NULL)
		{
			bool end = false;
			cltem1 = &polyclip.GetNext(posi2);
			if (posi2 == NULL)
			{
				cltem2 = &polyclip.GetHead();
				end = true;
			}
			else
			{
				cltem2 = &polyclip.GetAt(posi2);
			}
			CPoint cptem;
			bool bcro = crossline1(intem1->point, intem2->point, cltem1->point, cltem2->point, cptem);
			if (bcro)
			{
				clipPoint tem1;
				tem1.point = cptem;
				tem1.cross = true;
				tem1.follow = false;
				clipPoint tem2;
				tem2.point = cptem;
				tem2.cross = true;
				tem2.follow = false;
				posi1 = polyin.InsertBefore(posi1, tem1);
				posi2 = polyclip.InsertBefore(posi2, tem2);
				polyin.GetAt(posi1).drop = &polyclip.GetAt(posi2);
				polyclip.GetAt(posi2).drop = &polyin.GetAt(posi1);
				intem1->next = &polyin.GetAt(posi1);
				intem1->next->next = intem2;
				cltem1->next = &polyclip.GetAt(posi2);
				cltem1->next->next = cltem2;
				findcross = true;
				polyin.GetPrev(posi1);
				end0 = false;
				break;
			}
			if (end)break;
		}
		if (end0)break;
	}
	int n = polyin.GetSize();
	for (int i = 0; i < n; i++)
	{
		if (polyin.GetAt(polyin.FindIndex(i)).follow)continue;
		//�ӽ��㿪ʼ���ҳ������
		CArray<CPoint, CPoint&>resultarr;
		intem1 = &polyin.GetHead();
		int poly = polyin.GetSize();
		int npoly = 0;
		while (npoly!=poly)
		{
			if (intem1->follow)
			{
				intem1 = intem1->next;
				npoly++;
				continue;
			}
			if (intem1->cross)break;
			intem1 = intem1->next;
		}
		intem2 = intem1->drop;
		intem2->follow = true;
		while (1)
		{
			resultarr.Add(intem2->point);
			intem2 = intem2->next;
			if (intem2->follow)break;
			intem2->follow = true;
			if (intem2->cross)
			{
				if (intem2 == intem1 || intem2->drop == intem1)break;
				intem2 = intem2->drop;
				intem2->follow = true;
			}
		}
		//������
		if (i == 0)
		{
			pgin.points.RemoveAll();
			pgin.points.Copy(resultarr);
			resultarr.RemoveAll();
		}
		else
		{
			GPolygon newpoly;
			newpoly.points.Copy(resultarr);
			resultarr.RemoveAll();
			polygonsarr.Add(newpoly);
		}
	}
}

void mainclass::ClipPolygon1(CPoint mp1, CPoint mp2)
{
	//ȷ�����ο�Χ
	int xmax = max(mp1.x, mp2.x);
	int xmin = min(mp1.x, mp2.x);
	int ymax = max(mp1.y, mp2.y);
	int ymin = min(mp1.y, mp2.y);
	if (!polygonsarr.GetSize())return;
	CArray<CPoint, CPoint&>*points;//����ζ���
	CArray<CPoint, CPoint&>newpoints;//�ü�����
	CPoint temp, *po1, *po2, tem1, tem2;
	for (int ipoly = 0; ipoly < polygonsarr.GetSize(); ipoly++)
	{
		points = &polygonsarr[ipoly].points;
		//�ϱ߽�ü�
		for (int i = 0; i < points->GetSize(); i++)
		{
			po1 = &points->GetAt(i);//��ǰ����
			if (i == points->GetSize() - 1)po2 = &points->GetAt(0);
			else po2 = &points->GetAt(i + 1);//��һ������
			if (po1->y > ymax && po2->y < ymax)
			{
				tem1.SetPoint(1, ymax);
				tem2.SetPoint(rect.right, ymax);//�ϱ߽��߶ζ˵�
				crossline1(tem1, tem2, *po1, *po2, temp);//�󽻵�
				newpoints.Add(temp);//��ӽ���
				continue;
			}
			if (po1->y > ymax && po2->y > ymax)continue;
			if (po1->y < ymax && po2->y < ymax)
			{
				newpoints.Add(*po1);
				continue;
			}
			if (po1->y < ymax && po2->y > ymax)
			{
				tem1.SetPoint(1, ymax);
				tem2.SetPoint(rect.right, ymax);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(*po1);//��ӵ�ǰ����ͽ���
				newpoints.Add(temp);
				continue;
			}
		}
		points->RemoveAll();
		points->Copy(newpoints);
		newpoints.RemoveAll();
		//�±߽�ü�
		for (int i = 0; i < points->GetSize(); i++)
		{
			po1 = &points->GetAt(i);
			if (i == points->GetSize() - 1)po2 = &points->GetAt(0);
			else po2 = &points->GetAt(i + 1);
			if (po1->y < ymin && po2->y > ymin)
			{
				tem1.SetPoint(1, ymin);
				tem2.SetPoint(rect.right, ymin);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(temp);
				continue;
			}
			if (po1->y < ymin && po2->y < ymin)continue;
			if (po1->y > ymin && po2->y > ymin)
			{
				newpoints.Add(*po1);
				continue;
			}
			if (po1->y > ymin && po2->y < ymin)
			{
				tem1.SetPoint(1, ymin);
				tem2.SetPoint(rect.right, ymin);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(*po1);
				newpoints.Add(temp);
				continue;
			}
		}
		points->RemoveAll();
		points->Copy(newpoints);
		newpoints.RemoveAll();
		//�ұ߽�ü�
		for (int i = 0; i < points->GetSize(); i++)
		{
			po1 = &points->GetAt(i);
			if (i == points->GetSize() - 1)po2 = &points->GetAt(0);
			else po2 = &points->GetAt(i + 1);
			if (po1->x > xmax && po2->x < xmax)
			{
				tem1.SetPoint(xmax, 1);
				tem2.SetPoint(xmax, rect.bottom);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(temp);
				continue;
			}
			if (po1->x > xmax && po2->x > xmax)continue;
			if (po1->x < xmax && po2->x < xmax)
			{
				newpoints.Add(*po1);
				continue;
			}
			if (po1->x < xmax && po2->x > xmax)
			{
				tem1.SetPoint(xmax, 1);
				tem2.SetPoint(xmax, rect.bottom);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(*po1);
				newpoints.Add(temp);
				continue;
			}
		}
		points->RemoveAll();
		points->Copy(newpoints);
		newpoints.RemoveAll();
		//��߽�ü�
		for (int i = 0; i < points->GetSize(); i++)
		{
			po1 = &points->GetAt(i);
			if (i == points->GetSize() - 1)po2 = &points->GetAt(0);
			else po2 = &points->GetAt(i + 1);
			if (po1->x < xmin && po2->x > xmin)
			{
				tem1.SetPoint(xmin, 1);
				tem2.SetPoint(xmin, rect.bottom);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(temp);
				continue;
			}
			if (po1->x < xmin && po2->x < xmin)continue;
			if (po1->x > xmin && po2->x > xmin)
			{
				newpoints.Add(*po1);
				continue;
			}
			if (po1->x > xmin && po2->x < xmin)
			{
				tem1.SetPoint(xmin, 1);
				tem2.SetPoint(xmin, rect.bottom);
				crossline1(tem1, tem2, *po1, *po2, temp);
				newpoints.Add(*po1);
				newpoints.Add(temp);
				continue;
			}
		}
		points->RemoveAll();
		points->Copy(newpoints);//�滻ԭ���Ķ���ζ�������
		newpoints.RemoveAll();
	}
}