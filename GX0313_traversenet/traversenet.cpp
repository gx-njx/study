/////////////////////////////////////////////////////////////
//�ļ�����traversenet.cpp
//������ƽ�������
//���ߣ�����
/////////////////////////////////////////////////////////////

#include "pch.h"
#include "traversenet.h"
#include "Matrix.h"
#include <locale>

const double pi = atan(1) * 4;//Բ����
const double EPSILON = 1.0e-12;//��Сֵ
const double pll = 180 * 3600 / pi;//����ת��ϵ��

//�ַ����ָ��
//int traversenet::SplitStringArray(CString str, char split, CStringArray & aStr)
//{
//	int startIdx = 0;
//	int idx = str.Find(split, startIdx);
//	aStr.RemoveAll();//�����
//
//	while (-1 != idx)
//	{
//		CString sTmp = str.Mid(startIdx, idx - startIdx);
//		aStr.Add(sTmp);
//		startIdx = idx + 1;
//		idx = str.Find(split, startIdx);
//	}
//	CString sTmp = str.Right(str.GetLength() - startIdx);
//	if (!sTmp.IsEmpty())
//		aStr.Add(sTmp);
//	return aStr.GetSize();
//}

int traversenet::SplitStringArray(CString str, char split, CStringArray& aStr)
{
	int StartIdx = 0;
	int Idx = str.Find(split, StartIdx);
	aStr.RemoveAll();
	while (Idx != -1) {
		CString tem = str.Mid(StartIdx, Idx - StartIdx);
		aStr.Add(tem);
		StartIdx = Idx + 1;
		Idx = str.Find(split, StartIdx);
	}
	CString tem = str.Right(str.GetLength() - Idx);
	if (!tem.IsEmpty()) {
		aStr.Add(tem);
	}
	return aStr.GetSize();
}

//���ݵ�������ҵ��㣬��������ƣ����ص�ĵ�ַ
Point * traversenet::findpoint(CString name)
{
	for (int i = 0; i < ipknow + ipuknow; i++)
	{
		if (points[i].name == name)return &points[i];
	}
	return nullptr;//û�ҵ�
}

//���ݵ��ָ�룬���ص��������е�����ֵ
int traversenet::findpoint(Point * po)
{
	for (int i = 0; i < points.GetSize(); i++)
	{
		if (po == &points[i])return i;
	}
	return -1;//û�ҵ�
}

//������ֹ���ҵ��������ݣ���������ָ�룬����߶η��򣬷�������ָ��
MainObs * traversenet::findmObs(Point * po1, Point * po2, bool &sgn)
{
	for (int i = 0; i < datas.GetSize(); i++)
	{
		if (po1 == datas[i].po1 && po2 == datas[i].po2)
		{
			sgn = true;//��ʼ��˳��������һ��
			return &datas[i];
		}
		if (po1 == datas[i].po2 && po2 == datas[i].po1)
		{
			sgn = false;
			return &datas[i];
		}
	}
	return nullptr;//û�ҵ�
}

//�ȷ���ת���ȣ�����ȷ��룬���ػ���
double traversenet::DMS2RAD(double dDms)
{
	int iDeg, iMin;
	double dSec;
	iDeg = int(dDms + EPSILON);//��//��һ����С�������Է�ֹȡ��ʱ�ĳ���
	iMin = int((dDms - iDeg) * 100 + EPSILON);//��
	dSec = ((dDms - iDeg) * 100 - iMin) * 100;//��
	return (iDeg + (double)iMin / 60 + dSec / 3600)*pi / 180.0;
}

//����ת�ȷ��룬���뻡�ȣ����ضȷ���
double traversenet::RAD2DMS(double dRad)
{
	double dDeg = dRad * 180 / pi;
	int iDeg, iMin;
	double dSec;
	double dTmp;
	iDeg = int(dDeg + EPSILON);//�������ֶ�
	dTmp = (dDeg - iDeg) * 60;//С������ת���ɷ�
	iMin = int(dTmp + EPSILON);//ȡ�ֵ���������
	dSec = (dTmp - iMin) * 60;//��ȡ��
	return iDeg + (double)iMin / 100 + dSec / 10000;
}

//����߳����������㣬���ر߳�
double traversenet::Dist(Point a, Point b)
{
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	return sqrt(dx*dx + dy * dy);
}

//���㷽λ�ǣ���������ṹ�壬���ػ��ȷ�λ��
double traversenet::Azimuth2(Point a, Point b)
{
	double dx, dy;
	dx = b.x - a.x;
	dy = b.y - a.y;
	return standrad(atan2(dy, dx));
}

//��׼���Ƕȣ�������0-2pi֮��
double traversenet::standrad(double rad)
{
	while (rad > 2 * pi)rad -= 2 * pi;
	while (rad < 0)rad += 2 * pi;
	return rad;
}

//�������ݣ�������ڼ�������ݣ��������в�ߵķ�λ�ǣ����е���������ֵ
void traversenet::datearrange()
{
	for (int i = 0; i < ipknow-1; i++)//������֪�����ݣ�2��1����
	{
		MainObs temp;
		temp.po1 = &points[i];
		temp.po2 = &points[i + 1];
		temp.dist = Dist(points[i], points[i + 1]);
		temp.rad = Azimuth2(points[i], points[i + 1]);
		datas.Add(temp);
	}
	for (int i = 0; i < idists; i++)//���Ʋ������
	{
		MainObs temp;
		temp.po1 = dists[i].po1;
		temp.po2 = dists[i].po2;
		temp.dist = dists[i].dist;
		temp.rad = -1;//��λ�ǳ�ʼ��-1
		datas.Add(temp);
	}
	int ncount = idists;//��¼δ֪��λ�ǵı���
	while (ncount > 0)//����δ֪��ѭ��
	{
		for (int i = 0; i < istations; i++)//��ÿ����վ������ִ��
		{
			for (int j = 0; j < bearings[i].num; j++)//�Բ�վ��ÿ����׼��ִ��
			{
				bool sgn;//��¼��ֹ���Ƿ�
				MainObs *temp = findmObs(bearings[i].station, bearings[i].obs[j].po, sgn);//�ҵ���Ӧ��
				if (temp == nullptr)continue;//û�ҵ�
				if (temp->rad != -1)//����֪��λ�ǵı�
				{
					double rad1 = sgn ? temp->rad : standrad(temp->rad + pi);//��֪�ߵķ�λ�ǣ�������ֹ���෴��
					for (int k = 0; k < bearings[i].num; k++)//�Ҹò�վ��������׼��
					{
						bool sgn2;
						MainObs *temp2 = findmObs(bearings[i].station, bearings[i].obs[k].po, sgn2);//�ҵ���Ӧ��
						if (temp2 == nullptr)continue;//û�ҵ�
						if (temp2->rad == -1)//��δ֪��λ�ǵı�
						{
							double drad = bearings[i].obs[k].rad - bearings[i].obs[j].rad;//����׼��ǶȲ�
							double rad2 = standrad(rad1 + drad);//δ֪�ߵķ�λ��
							temp2->rad = sgn2 ? rad2 : standrad(rad2 + pi);//����δ֪����ֹ���෴
							ncount--;//δ֪�߼�1
						}
					}
				}
			}
		}
	}
	ncount = ipuknow;//��¼δ֪��
	while (ncount > 0)//����δ֪��ѭ��
	{
		for (int i = 0; i < datas.GetSize(); i++)//��ÿ���߼���
		{
			if (datas[i].po1->flag == true && datas[i].po2->flag == false)//һ����֪һ��δ֪�����Լ���
			{
				datas[i].po2->x = datas[i].po1->x + datas[i].dist*cos(datas[i].rad);
				datas[i].po2->y = datas[i].po1->y + datas[i].dist*sin(datas[i].rad);
				datas[i].po2->flag = true;
				ncount--;
			}
			if (datas[i].po2->flag == true && datas[i].po1->flag == false)
			{
				datas[i].po1->x = datas[i].po2->x - datas[i].dist*cos(datas[i].rad);
				datas[i].po1->y = datas[i].po2->y - datas[i].dist*sin(datas[i].rad);
				datas[i].po1->flag = true;
				ncount--;
			}
		}
	}
}

//ƽ�����
void traversenet::AdjustCal()
{
	//V=B*x-L
	CMatrix V(iangles + idists, 1);//����������
	CMatrix Vsum(iangles + idists, 1);//���ո����������и�����֮�ͣ�
	CMatrix B(iangles + idists, istations + 2 * ipuknow);//ϵ������
	CMatrix dx;//������������δ֪�������������
	CMatrix L(iangles + idists, 1);//���������
	CMatrix P(iangles + idists, iangles + idists);//Ȩ��
	//����Ԫ�س�ʼ��
	for (int i = 0; i < B.Row(); i++)
	{
		for (int j = 0; j < B.Col(); j++)
		{
			B(i, j) = 0;
		}
	}
	for (int i = 0; i < L.Row(); i++)
	{
		L(i, 0) = 0;
		Vsum(i, 0) = 0;
	}
	for (int i = 0; i < P.Row(); i++)
	{
		for (int j = 0; j < P.Col(); j++)
		{
			P(i, j) = 0;
		}
	}
	double maxV = 0;//��¼��������
	double maxdx = 0;//��¼��������
	int ncount = 0;//��������
	//����ƽ�����
	do
	{
		maxV = 0;//��¼��������
		maxdx = 0;//��¼��������
		int Brow = 0;//��¼��ǰB����λ������
		int Lrow = 0;//��¼��ǰL����λ������
		//���㷽��������ݵ�����
		for (int i = 0; i < istations; i++)
		{
			double Zi0 = Azimuth2(*bearings[i].station, *bearings[i].obs[0].po);
			for (int j = 0; j < bearings[i].num; j++)
			{
				B(Brow, i) = -1;//Z
				Point *po1 = bearings[i].station;
				Point *po2 = bearings[i].obs[j].po;
				int n1 = findpoint(po1);//�ҵ��������е�λ�ã�����������˳����������ͬ
				int n2 = findpoint(po2);
				double Tij0 = Azimuth2(*po1, *po2);
				double aij = pll * (sin(Tij0)) / (Dist(*po1, *po2) * 1000);//���뻯mm
				double bij = -pll * (cos(Tij0)) / (Dist(*po1, *po2) * 1000);
				if (!po1->know)
				{
					B(Brow, istations + 2 * (n1 - ipknow)) = aij;
					B(Brow, istations + 2 * (n1 - ipknow) + 1) = bij;
				}
				if (!po2->know)
				{
					B(Brow, istations + 2 * (n2 - ipknow)) = -aij;
					B(Brow, istations + 2 * (n2 - ipknow) + 1) = -bij;
				}
				Brow++;
				double Lij = bearings[i].obs[j].rad;
				L(Lrow, 0) = -(standrad(-Zi0 + Tij0) - Lij)*pll;//�������
				Lrow++;
			}
		}
		//����߳��������ݵ�����
		for (int i = 0; i < idists; i++)
		{
			Point *po1 = dists[i].po1;
			Point *po2 = dists[i].po2;
			int n1 = findpoint(po1);
			int n2 = findpoint(po2);
			double Tij0 = Azimuth2(*po1, *po2);
			double Sij0 = Dist(*po1, *po2);
			if (!po1->know)
			{
				B(Brow, istations + 2 * (n1 - ipknow)) = -cos(Tij0);
				B(Brow, istations + 2 * (n1 - ipknow) + 1) = -sin(Tij0);
			}
			if (!po2->know)
			{
				B(Brow, istations + 2 * (n2 - ipknow)) = cos(Tij0);
				B(Brow, istations + 2 * (n2 - ipknow) + 1) = sin(Tij0);
			}
			Brow++;
			L(Lrow, 0) = -(Sij0 - dists[i].dist) * 1000;//�����mm
			Lrow++;
		}
		//��Ȩ
		double u = 0;//��λȨ�����
		int Prow = 0;//Ȩ����
		for (int i = 0; i < istations; i++)//�Ƕȹ۲�ֵ��Ȩ
		{
			for (int j = 0; j < bearings[i].num; j++)
			{
				if (bearings[i].obs[j].m != 0)
				{
					u = u == 0 ? bearings[i].obs[j].m : u;
					P(Prow, Prow) = u * u / (bearings[i].obs[j].m* bearings[i].obs[j].m);
				}
				else { P(Prow, Prow) = 1; }
				Prow++;
			}
		}
		for (int i = iangles; i < iangles + idists; i++)//�߳��۲�ֵ��Ȩ
		{
			if (dists[i - iangles].B != 0)
			{
				double m = sqrt(dists[i - iangles].dist)*dists[i - iangles].B;
				u = u == 0 ? m : u;
				P(i, i) = u * u / (m*m);
			}
			else P(i, i) = 100.0 / dists[i - iangles].dist;
		}
		//������test
		//CString Bstr, Lstr, Pstr, strtem;
		//for (int i = 0; i < B.Row(); i++)
		//{
		//	for (int j = 0; j < B.Col(); j++)
		//	{
		//		strtem.Format(_T("%.6lf\t"), B(i, j));
		//		Bstr += strtem;
		//	}
		//	Bstr += _T("\r\n");
		//}
		//for (int i = 0; i < L.Row(); i++)
		//{
		//	strtem.Format(_T("%.6lf\r\n"), L(i, 0));
		//	Lstr += strtem;
		//}
		//for (int i = 0; i < P.Row(); i++)
		//{
		//	for (int j = 0; j < P.Col(); j++)
		//	{
		//		strtem.Format(_T("%.6lf\t"), P(i, j));
		//		Pstr += strtem;
		//	}
		//	Pstr += _T("\r\n");
		//}
		//CString Xstr;
		//for (int i = 0; i < ipuknow; i++)
		//{
		//	strtem.Format(_T("%.6lf\t%.6lf\r\n"), points[i + ipknow].x, points[i + ipknow].y);
		//	Xstr += strtem;
		//}
		//ƽ�����
		dx = (~B*P*B).Inv()*(~B*P*L);
		//������������
		for (int i = 0; i < ipuknow; i++)
		{
			points[i + ipknow].x += dx(istations + 2 * i, 0) / 1000.0;//����δ֪�����꣨mm��m��
			points[i + ipknow].y += dx(istations + 2 * i + 1, 0) / 1000.0;
		}
		//�����۲�ֵ
		V = B * dx - L;
		Vsum = Vsum + V;//�ۼӸ�����
		int Vrow = 0;
		for (int i = 0; i < istations; i++)
		{
			int Vtem = 0;
			for (int j = 0; j < bearings[i].num; j++)
			{
				if (j == 0)
				{
					Vtem = Vrow;//��¼��ʼ��׼����
					Vsum(Vtem, 0) = 0;//��ʼ��׼��Ϊ0
					Vrow++;
					continue;
				}
				bearings[i].obs[j].rad += ((V(Vrow, 0) - V(Vtem, 0)) / pll);//��ʼ��׼�Ǹ����ڴ�����
				Vsum(Vrow, 0) -= V(Vtem, 0);//�����ۼӸ�����
				bearings[i].obs[j].v = Vsum(Vrow, 0);
				Vrow++;
			}
		}
		for (int i = 0; i < idists; i++)
		{
			dists[i].dist += V(Vrow, 0) / 1000.0;
			dists[i].v = Vsum(Vrow, 0);
			Vrow++;
		}
		for (int i = 0; i < iangles; i++)
		{
			maxV = max(maxV, V(i, 0));
		}
		for (int i = 0; i < dx.Row(); i++)
		{
			maxdx = max(maxdx, dx(i, 0));
		}
		//maxV = maxV / pll;
		ncount++;
	}while (max(maxdx, maxV) > 0.1);
	//��������
	V = Vsum;
	m0 = sqrt((~V*P*V)(0, 0) / (iangles + idists - 2 * ipuknow - istations));//�����
	CMatrix Qx = (~B*P*B).Inv();//Э��������
	//�۲�ֵ��������

	//�����Բ��������
	for (int i = 0; i < ipuknow; i++)
	{
		double Qxx = Qx(istations + 2 * i, istations + 2 * i);
		double Qyy = Qx(istations + 2 * i + 1, istations + 2 * i + 1);
		double Qxy = Qx(istations + 2 * i, istations + 2 * i + 1);
		double Q = standrad(0.5*atan(2 * Qxy / (Qxx - Qyy)));
		double El = m0 * sqrt(Qxx + Qxy * tan(Q));
		double Es = m0 * sqrt(Qxx + Qxy * tan(Q + pi / 2));
		double m = m0 * sqrt(Qxx + Qyy);
		points[i + ipknow].circle.Q = Q < pi ? Q : Q - pi;//���Ʒ�λ�ǵ�0-pi
		points[i + ipknow].circle.El = El;
		points[i + ipknow].circle.Es = Es;
		points[i + ipknow].m = m;
	}
}

//��������ַ���
CString traversenet::OutStr()
{
	CString outstr, strtem;
	strtem.Format(_T("%.6lf"), m0);
	outstr += _T("�����m0��");
	outstr += strtem;
	outstr += _T("\r\n��֪�����꣺\r\n����\tX(m)        \tY(m)\r\n");
	for (int i = 0; i < ipknow; i++)
	{
		strtem.Format(_T("\t%.6lf\t%.6lf\r\n"), points[i].x, points[i].y);
		outstr = outstr + points[i].name + strtem;
	}
	outstr += _T("δ֪�����꣺\r\n����\tX(m)        \tY(m)\r\n");
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		strtem.Format(_T("\t%.6lf\t%.6lf\t\r\n"), points[i].x, points[i].y);
		outstr = outstr + points[i].name + strtem;
	}
	outstr += _T("δ֪����������\r\n����\t����m\t����m\t���᷽λdms\t��λ�����m\r\n");
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		strtem.Format(_T("\t%.4lf\t%.4lf\t%.6lf  \t%.4lf\r\n"),
			points[i].circle.El / 1000.0, points[i].circle.Es / 1000.0, RAD2DMS(points[i].circle.Q), points[i].m / 1000.0);
		outstr = outstr + points[i].name + strtem;
	}
	outstr += _T("�߳��۲�ɹ���\r\n��վ\t��׼\t����m        \t������m\tƽ���ֵm\t��λ��dms\r\n");
	for (int i = 0; i < idists; i++)
	{
		outstr = outstr + dists[i].po1->name + _T("\t") + dists[i].po2->name + _T("\t");
		strtem.Format(_T("%.4lf    \t%.4lf\t%.4lf    \t%.6lf\r\n"),
			dists[i].dist - dists[i].v/1000.0,
			dists[i].v/1000.0,
			dists[i].dist,
			RAD2DMS(Azimuth2(*dists[i].po1, *dists[i].po2)));
		outstr += strtem;
	}
	outstr += _T("�Ƕȹ۲�ɹ���\r\n��վ\t��׼\t����ֵdms\t������s\tƽ���ֵdms\r\n");
	for (int i = 0; i < istations; i++)
	{
		for (int j = 0; j < bearings[i].num; j++)
		{
			outstr = outstr + bearings[i].station->name + _T("\t") + bearings[i].obs[j].po->name + _T("\t");
			strtem.Format(_T("%.6lf \t%.2lf\t%.6lf\r\n"),
				RAD2DMS(bearings[i].obs[j].rad - DMS2RAD(bearings[i].obs[j].v / 10000.0)),
				bearings[i].obs[j].v,
				RAD2DMS(bearings[i].obs[j].rad));
			if (j == 0)strtem.Format(_T("%.6lf\r\n"), RAD2DMS(bearings[i].obs[j].rad));
			outstr += strtem;
		}
		outstr += _T("\r\n");
	}
	return outstr;
}

//��������ߣ������ͼ���ޣ����ر�����
double traversenet::drawM(double &xmax, double &xmin, double &ymax, double &ymin)
{
	xmax = xmin = points[0].x;
	ymax = ymin = points[0].y;
	for (int i = 1; i < points.GetSize(); i++)
	{
		xmax = max(xmax, points[i].x);
		xmin = min(xmin, points[i].x);
		ymax = max(ymax, points[i].y);
		ymin = min(ymin, points[i].y);
	}
	double dx = xmax - xmin;
	double dy = ymax - ymin;
	double m = min((rect.Height() - 125) / dx, (rect.Width() - 100) / dy);//���ű����������ռ�
	return m;
}

//��ͼ����ת�����������꣬����ͻ�����Ӧ��
CPoint traversenet::drawPoint(double x, double y)
{
	double xmax, xmin, ymax, ymin;
	double m = drawM(xmax, xmin, ymax, ymin);
	return CPoint(m*(y - ymin) + 80, m*(xmax - x) + 80);//�������ϵת��ƽ�����꣬�����ռ�
}

//��ͼ����ת�����������꣬����ͻ�����Ӧ��
CPoint traversenet::drawPoint(Point po)
{
	double xmax, xmin, ymax, ymin;
	double m = drawM(xmax, xmin, ymax, ymin);
	return CPoint(m*(po.y - ymin) + 80, m*(xmax - po.x) + 80);
}

//��Բ������������Բ�����꣬��Բ���ĵ㣬DCָ��
void traversenet::EllipsePoints(int x, int y, Point center, CDC * pDC)
{
	//��ת���
	double dx, dy;
	//�ĸ����޶Գ�
	rotate(x, y, dx, dy, center.circle.Q);//��ת��λ��
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
	rotate(-x, y, dx, dy, center.circle.Q);
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
	rotate(-x, -y, dx, dy, center.circle.Q);
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
	rotate(x, -y, dx, dy, center.circle.Q);
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
}

//���������Բ������DCָ�룬�㣬����
void traversenet::Midpointellispe(CDC * pDC, Point center, double m)
{
	int x, y;
	double d1, d2;
	int a = center.circle.El * m;
	int b = center.circle.Es * m;
	CPen penLine(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *oldpen = pDC->SelectObject(&penLine);
	//���Ƴ�����
	double dx, dy;//��ת���
	rotate(-a, 0, dx, dy, center.circle.Q);//��ת��λ��
	pDC->MoveTo(drawPoint(center.x + dx, center.y + dy));
	rotate(a, 0, dx, dy, center.circle.Q);
	pDC->LineTo(drawPoint(center.x + dx, center.y + dy));
	rotate(0, -b, dx, dy, center.circle.Q);
	pDC->MoveTo(drawPoint(center.x + dx, center.y + dy));
	rotate(0, b, dx, dy, center.circle.Q);
	pDC->LineTo(drawPoint(center.x + dx, center.y + dy));
	pDC->SelectObject(oldpen);
	penLine.DeleteObject();
	x = 0;
	y = b;
	d1 = b * b + a * a*(-b + 0.25);//��ʼֵ
	EllipsePoints(x, y, center, pDC);
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
		EllipsePoints(x, y, center, pDC);
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
		EllipsePoints(x, y, center, pDC);
	}
}

//��ת�������������꣬�����ת�����꣬����Ƕȣ�����˳ʱ�룩
void traversenet::rotate(double x0, double y0, double & x, double & y, double the)
{
	//��ʱ����ת�㷨��ʹ�ø���˳ʱ��
	x = int(x0 * cos(-the) + y0 * sin(-the) + 0.5);
	y = int(y0 * cos(-the) - x0 * sin(-the) + 0.5);
}

//���캯��
traversenet::traversenet()
{
}

//��������
traversenet::~traversenet()
{
	deletedata();
}

//������ݣ��ͷ��ڴ�
void traversenet::deletedata()
{
	if (points.GetSize())
	{
		points.RemoveAll();
	}
	if (dists)
	{
		delete[] dists;
	}
	if (bearings)
	{
		for (int i = 0; i < istations; i++)
		{
			if (bearings[i].obs)
			{
				delete[] bearings[i].obs;
			}
		}
		delete[] bearings;
	}
	if (datas.GetSize())
	{
		datas.RemoveAll();
	}
}

//��ȡ�ļ�������ļ��ڵ����ݣ������Ƿ�ɹ���ȡ
int traversenet::ReadFile(CString &strRead)
{
	if (points.GetSize())
	{
		deletedata();
	}
	CFileDialog getfile(TRUE, _T("txt"), _T("һ�����߲���2.txt"));
	if (getfile.DoModal() != IDOK)return -1;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeRead))return 0;
	CString strline;
	CStringArray strtem;
	strRead.Empty();//���
	if(!file.ReadString(strline))return 2;//��1�У�����Ϊ��
	strRead += strline;
	if(!file.ReadString(strline))return false;//��2�У���֪����
	strRead = strRead + _T("\r\n") + strline;
	ipknow = _tstoi(strline);
	for (int i = 0; i < ipknow; i++)//��ȡ��֪������
	{
		if(!file.ReadString(strline))return false;//��һ�У���֪������
		strRead = strRead + _T("\r\n") + strline;
		int n = SplitStringArray(strline, ',', strtem);
		if (n != 4)return false;
		Point temp;//�����
		temp.name = strtem[0];
		temp.know = true;
		temp.flag = true;
		temp.x = _tstof(strtem[2]);
		temp.y = _tstof(strtem[3]);
		points.Add(temp);//�����������
	}
	if(!file.ReadString(strline))return false;//δ֪���ͷ
	strRead = strRead + _T("\r\n") + strline;
	if(!file.ReadString(strline))return false;//δ֪�����
	strRead = strRead + _T("\r\n") + strline;
	ipuknow = _tstoi(strline);
	if(!file.ReadString(strline))return false;//δ֪������
	strRead = strRead + _T("\r\n") + strline;
	int n = SplitStringArray(strline, ',', strtem);
	if (n != ipuknow)return false;
	for (int i = 0; i < n; i++)//��ȡδ֪������
	{
		Point temp;
		temp.name = strtem[i];
		temp.know = false;
		temp.flag = false;
		points.Add(temp);
	}
	if(!file.ReadString(strline))return false;//�߳��������ݱ�ͷ
	strRead = strRead + _T("\r\n") + strline;
	if(!file.ReadString(strline))return false;//�߳��������ݸ���
	strRead = strRead + _T("\r\n") + strline;
	idists = _tstoi(strline);
	dists = new DistObs[idists];
	for (int i = 0; i < idists; i++)//��ȡ�߳���������
	{
		if(!file.ReadString(strline))return false;//��һ�б߳���������
		strRead = strRead + _T("\r\n") + strline;
		n = SplitStringArray(strline, ',', strtem);
		if (n < 3)return false;
		dists[i].po1 = findpoint(strtem[0]);
		dists[i].po2 = findpoint(strtem[1]);
		if (dists[i].po1 == nullptr || dists[i].po2 == nullptr)return false;//û�ҵ���
		dists[i].dist = _tstof(strtem[2]);
		dists[i].A = dists[i].B = 0;
		if (n == 4)dists[i].B = _tstof(strtem[3]);
		if (n == 5)dists[i].A = _tstof(strtem[4]);
	}
	if(!file.ReadString(strline))return false;//��λ���ݱ�ͷ
	strRead = strRead + _T("\r\n") + strline;
	if(!file.ReadString(strline))return false;//��λ��վ���ͽǶȸ���
	strRead = strRead + _T("\r\n") + strline;
	n = SplitStringArray(strline, ',', strtem);
	if (n != 2)return false;
	istations = _tstoi(strtem[0]);
	iangles = _tstoi(strtem[1]);
	bearings = new BearingObs[istations];
	for (int i = 0; i < istations; i++)//��ȡ�����������
	{
		if(!file.ReadString(strline))return false;//��վ������׼��
		strRead = strRead + _T("\r\n") + strline;
		n = SplitStringArray(strline, ',', strtem);
		if (n != 2)return false;
		bearings[i].station = findpoint(strtem[0]);
		if (bearings[i].station == nullptr)return false;
		int item = _tstoi(strtem[1]);//��׼��
		bearings[i].num = item;
		bearings[i].obs = new DirectObs[item];
		for (int j = 0; j < item; j++)//��ȡ��׼������
		{
			if(!file.ReadString(strline))return false;//��׼�������Ƕȣ��ȷ��룩
			strRead = strRead + _T("\r\n") + strline;
			n = SplitStringArray(strline, ',', strtem);
			if (n < 2)return false;
			bearings[i].obs[j].po = findpoint(strtem[0]);
			if (bearings[i].obs[j].po == nullptr)return false;
			bearings[i].obs[j].rad = DMS2RAD(_tstof(strtem[1]));
			bearings[i].obs[j].m = 0;
			if (n == 3)bearings[i].obs[j].m = _tstof(strtem[2]);
		}
	}
	file.Close();
	datearrange();//����
	AdjustCal();//ƽ��
	return 1;
}

//���ɽ���ַ���
CString traversenet::ResultStr()
{
	return OutStr();
}

//�����ļ��������Ƿ�ɹ�����
int traversenet::SaveFile()
{
	if (!points.GetSize())return 0;
	CString strout = ResultStr();//���ú���
	strout.Remove('\r');
	CFileDialog getfile(FALSE, _T("txt"), _T("������ƽ����.txt"));
	if (getfile.DoModal() != IDOK)return 2;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))return -1;
	setlocale(LC_ALL, "chs");//û����佫�޷��������
	file.WriteString(strout);
	file.Close();
	return 1;
}

//���Ƶ�����������DCָ��Ϳͻ�����С�������Ƿ����
bool traversenet::Drawnet(CDC * pDC, CRect rc)
{
	if (!points.GetSize())return false;
	rect = rc;//����ͻ�����С
	double xmax, xmin, ymax, ymin;
	double m = drawM(xmax, xmin, ymax, ymin);//��ȡ�����ߺͻ�ͼ�߽�
	CPen penNet(PS_SOLID, 1, RGB(150, 200, 100));//���ƽǶȹ۲�����
	CPen penLine(PS_SOLID, 3, RGB(100, 200, 255));//���Ʊ߳��۲��
	CPen penXY(PS_SOLID, 1, RGB(150, 150, 150));//��������ֵ��
	CPen penXYLine(PS_SOLID, 1, RGB(200, 200, 200));//���������
	CPen penPoint(PS_SOLID, 3, RGB(0, 0, 255));//������֪���
	CPen *oldpen = pDC->SelectObject(&penXY);
	xmin -= 20/m;//�������������
	ymin -= 20/m;
	double dx = (xmax - xmin) / 5;//������
	double dy = (ymax - ymin) / 5;
	//����������
	pDC->MoveTo(drawPoint(xmax + 10 / m, ymin));
	pDC->LineTo(drawPoint(xmin, ymin));
	pDC->LineTo(drawPoint(xmin, ymax + 10 / m));
	//�����������ͷ
	pDC->MoveTo(drawPoint(xmax, ymin - 5 / m));
	pDC->LineTo(drawPoint(xmax + 10 / m, ymin));
	pDC->LineTo(drawPoint(xmax, ymin + 5 / m));
	pDC->MoveTo(drawPoint(xmin - 5 / m, ymax));
	pDC->LineTo(drawPoint(xmin, ymax + 10 / m));
	pDC->LineTo(drawPoint(xmin + 5 / m, ymax));
	//��������
	pDC->SelectObject(&penXYLine);
	for (int x = xmin + dx; x < xmax + 1; x += dx)
	{
		pDC->MoveTo(drawPoint(x, ymin));
		pDC->LineTo(drawPoint(x, ymax));
	}
	for (int y = ymin + dy; y < ymax + 1; y += dy)
	{
		pDC->MoveTo(drawPoint(xmin, y));
		pDC->LineTo(drawPoint(xmax, y));
	}
	CFont xyFont;//��������
	xyFont.CreateFont(12, 0, 0, 0, 0, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("����"));
	pDC->SelectObject(xyFont);
	pDC->SetBkMode(0);//͸������
	pDC->SetTextColor(RGB(10, 10, 10));
	CString strtem;
	strtem = _T("Y");
	CPoint temp = drawPoint(xmin + 5 / m, ymax + 20 / m);
	pDC->TextOutW(temp.x, temp.y, strtem);
	strtem = _T("X");
	temp = drawPoint(xmax + 20 / m, ymin - 5 / m);
	pDC->TextOutW(temp.x, temp.y, strtem);
	//���������Ӧ����
	for (int x = xmin; x < xmax + 1; x += dx)
	{
		strtem.Format(_T("%d"), x);
		CPoint temp = drawPoint(x + 5 / m, ymin - 45 / m);
		pDC->TextOutW(temp.x, temp.y, strtem);
	}
	for (int y = ymin; y < ymax + 1; y += dy)
	{
		strtem.Format(_T("%d"), y);
		CPoint temp = drawPoint(xmin - 5 / m, y - 10 / m);
		pDC->TextOutW(temp.x, temp.y, strtem);
	}
	//���Ƶ�����
	pDC->SelectObject(&penNet);
	for (int i = 0; i < istations; i++)
	{
		for (int j = 0; j < bearings[i].num; j++)
		{
			pDC->MoveTo(drawPoint(*bearings[i].station));
			pDC->LineTo(drawPoint(*bearings[i].obs[j].po));
			if (j > 0)
			{
				//��׼��֮������
				pDC->MoveTo(drawPoint(*bearings[i].obs[j - 1].po));
				pDC->LineTo(drawPoint(*bearings[i].obs[j].po));
			}
		}
	}
	//���Ƶ��ߣ����ǣ�
	pDC->SelectObject(&penLine);
	for (int i = 0; i < idists; i++)
	{
		pDC->MoveTo(drawPoint(*dists[i].po1));
		pDC->LineTo(drawPoint(*dists[i].po2));
	}
	//������֪���־
	pDC->SelectObject(&penPoint);
	for (int i = 0; i < ipknow; i++)
	{
		double x[3], y[3];
		x[0] = drawPoint(points[i]).x + 10;
		y[0] = drawPoint(points[i]).y + 7;
		x[1] = drawPoint(points[i]).x - 10;
		y[1] = drawPoint(points[i]).y + 7;
		x[2] = drawPoint(points[i]).x;
		y[2] = drawPoint(points[i]).y - 11;
		pDC->MoveTo(x[0], y[0]);
		pDC->LineTo(x[1], y[1]);
		pDC->LineTo(x[2], y[2]);
		pDC->LineTo(x[0], y[0]);
	}
	//���ƿ��Ƶ�����
	for (int i = 1; i < ipknow; i++)
	{
		double x[2], y[2];
		CPoint p1 = drawPoint(points[i - 1]);
		CPoint p2 = drawPoint(points[i]);
		bool dx = (p1.x < p2.x);
		bool dy = (p1.y < p2.y);
		int d = 5;
		x[0] = dx ? p1.x + d : p1.x - d;
		x[1] = dx ? p2.x - d : p2.x + d;
		y[0] = dy ? p1.y + d : p1.y - d;
		y[1] = dy ? p2.y - d : p2.y + d;
		pDC->MoveTo(x[0], p1.y);
		pDC->LineTo(p2.x, y[1]);
		pDC->MoveTo(p1.x, y[0]);
		pDC->LineTo(x[1], p2.y);
	}
	//�ͷ���Դ
	pDC->SelectObject(oldpen);
	penNet.DeleteObject();
	penLine.DeleteObject();
	penXY.DeleteObject();
	penXYLine.DeleteObject();
	xyFont.DeleteObject();
	return true;
}

//���������Բ�����Ȼ��Ƶ�����
bool traversenet::Drawcircle(CDC * pDC, CRect rc, int size)
{
	if (!Drawnet(pDC, rc))return false;//�Ȼ��Ƶ�����
	double maxE = 0;//��������Բ����
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		maxE = max(maxE, points[i].circle.El);
		maxE = max(maxE, points[i].circle.Es);
	}
	CPoint a = drawPoint(0, 0);
	CPoint b = drawPoint(0, 100);
	//�����ߣ�ʹ��������Ϊsize���س���
	double m = fabs(100.0 / (b.x - a.x)*size / maxE);
	//���������Բ
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		Midpointellispe(pDC, points[i], m);
	}
	return true;
}

//��ע���������Ȼ��Ƶ�����
bool traversenet::Drawname(CDC * pDC, CRect rc)
{
	if (!Drawnet(pDC, rc))return false;//���Ƶ�����
	CFont nameFont;//��������
	nameFont.CreateFont(12, 0, 0, 0, 0, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("����"));
	CFont *oldpen = pDC->SelectObject(&nameFont);
	pDC->SetBkMode(2);//��ɫ����
	pDC->SetTextColor(RGB(0, 0, 0));
	//�������
	for (int i = 0; i < points.GetSize(); i++)
	{
		CPoint temp = drawPoint(points[i].x, points[i].y);
		pDC->TextOutW(temp.x, temp.y, points[i].name);
	}
	//�ͷ���Դ
	pDC->SelectObject(oldpen);
	nameFont.DeleteObject();
	return true;
}

