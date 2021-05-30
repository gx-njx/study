/////////////////////////////////////////////////////////////
//文件名：traversenet.cpp
//导线网平差计算类
//作者：郭鑫
/////////////////////////////////////////////////////////////

#include "pch.h"
#include "traversenet.h"
#include "Matrix.h"
#include <locale>

const double pi = atan(1) * 4;//圆周率
const double EPSILON = 1.0e-12;//极小值
const double pll = 180 * 3600 / pi;//弧度转秒系数

//字符串分割函数
//int traversenet::SplitStringArray(CString str, char split, CStringArray & aStr)
//{
//	int startIdx = 0;
//	int idx = str.Find(split, startIdx);
//	aStr.RemoveAll();//先清空
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

//根据点的名称找到点，输入点名称，返回点的地址
Point * traversenet::findpoint(CString name)
{
	for (int i = 0; i < ipknow + ipuknow; i++)
	{
		if (points[i].name == name)return &points[i];
	}
	return nullptr;//没找到
}

//根据点的指针，返回点在数组中的索引值
int traversenet::findpoint(Point * po)
{
	for (int i = 0; i < points.GetSize(); i++)
	{
		if (po == &points[i])return i;
	}
	return -1;//没找到
}

//根据起止点找到计算数据，输入两点指针，输出线段方向，返回数据指针
MainObs * traversenet::findmObs(Point * po1, Point * po2, bool &sgn)
{
	for (int i = 0; i < datas.GetSize(); i++)
	{
		if (po1 == datas[i].po1 && po2 == datas[i].po2)
		{
			sgn = true;//起始点顺序与输入一致
			return &datas[i];
		}
		if (po1 == datas[i].po2 && po2 == datas[i].po1)
		{
			sgn = false;
			return &datas[i];
		}
	}
	return nullptr;//没找到
}

//度分秒转弧度，输入度分秒，返回弧度
double traversenet::DMS2RAD(double dDms)
{
	int iDeg, iMin;
	double dSec;
	iDeg = int(dDms + EPSILON);//度//加一个很小的数，以防止取整时的出错
	iMin = int((dDms - iDeg) * 100 + EPSILON);//分
	dSec = ((dDms - iDeg) * 100 - iMin) * 100;//秒
	return (iDeg + (double)iMin / 60 + dSec / 3600)*pi / 180.0;
}

//弧度转度分秒，输入弧度，返回度分秒
double traversenet::RAD2DMS(double dRad)
{
	double dDeg = dRad * 180 / pi;
	int iDeg, iMin;
	double dSec;
	double dTmp;
	iDeg = int(dDeg + EPSILON);//整数部分度
	dTmp = (dDeg - iDeg) * 60;//小数部分转换成分
	iMin = int(dTmp + EPSILON);//取分的整数部分
	dSec = (dTmp - iMin) * 60;//截取秒
	return iDeg + (double)iMin / 100 + dSec / 10000;
}

//计算边长，输入两点，返回边长
double traversenet::Dist(Point a, Point b)
{
	double dx = b.x - a.x;
	double dy = b.y - a.y;
	return sqrt(dx*dx + dy * dy);
}

//计算方位角，输入两点结构体，返回弧度方位角
double traversenet::Azimuth2(Point a, Point b)
{
	double dx, dy;
	dx = b.x - a.x;
	dy = b.y - a.y;
	return standrad(atan2(dy, dx));
}

//标准化角度，控制在0-2pi之间
double traversenet::standrad(double rad)
{
	while (rad > 2 * pi)rad -= 2 * pi;
	while (rad < 0)rad += 2 * pi;
	return rad;
}

//整理数据，组成用于计算的数据，计算所有测边的方位角，所有点的坐标近似值
void traversenet::datearrange()
{
	for (int i = 0; i < ipknow-1; i++)//计算已知点数据，2点1条边
	{
		MainObs temp;
		temp.po1 = &points[i];
		temp.po2 = &points[i + 1];
		temp.dist = Dist(points[i], points[i + 1]);
		temp.rad = Azimuth2(points[i], points[i + 1]);
		datas.Add(temp);
	}
	for (int i = 0; i < idists; i++)//复制测边数据
	{
		MainObs temp;
		temp.po1 = dists[i].po1;
		temp.po2 = dists[i].po2;
		temp.dist = dists[i].dist;
		temp.rad = -1;//方位角初始化-1
		datas.Add(temp);
	}
	int ncount = idists;//记录未知方位角的边数
	while (ncount > 0)//存在未知边循环
	{
		for (int i = 0; i < istations; i++)//对每个测站的数据执行
		{
			for (int j = 0; j < bearings[i].num; j++)//对测站中每个照准点执行
			{
				bool sgn;//记录起止点是否反
				MainObs *temp = findmObs(bearings[i].station, bearings[i].obs[j].po, sgn);//找到对应边
				if (temp == nullptr)continue;//没找到
				if (temp->rad != -1)//是已知方位角的边
				{
					double rad1 = sgn ? temp->rad : standrad(temp->rad + pi);//已知边的方位角（纠正起止点相反）
					for (int k = 0; k < bearings[i].num; k++)//找该测站其他的照准点
					{
						bool sgn2;
						MainObs *temp2 = findmObs(bearings[i].station, bearings[i].obs[k].po, sgn2);//找到对应边
						if (temp2 == nullptr)continue;//没找到
						if (temp2->rad == -1)//是未知方位角的边
						{
							double drad = bearings[i].obs[k].rad - bearings[i].obs[j].rad;//两照准点角度差
							double rad2 = standrad(rad1 + drad);//未知边的方位角
							temp2->rad = sgn2 ? rad2 : standrad(rad2 + pi);//纠正未知边起止点相反
							ncount--;//未知边减1
						}
					}
				}
			}
		}
	}
	ncount = ipuknow;//记录未知点
	while (ncount > 0)//存在未知点循环
	{
		for (int i = 0; i < datas.GetSize(); i++)//对每条边计算
		{
			if (datas[i].po1->flag == true && datas[i].po2->flag == false)//一点已知一点未知，可以及算
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

//平差计算
void traversenet::AdjustCal()
{
	//V=B*x-L
	CMatrix V(iangles + idists, 1);//改正数矩阵
	CMatrix Vsum(iangles + idists, 1);//最终改正数（所有改正数之和）
	CMatrix B(iangles + idists, istations + 2 * ipuknow);//系数矩阵
	CMatrix dx;//参数改正数（未知点坐标改正数）
	CMatrix L(iangles + idists, 1);//常数项矩阵
	CMatrix P(iangles + idists, iangles + idists);//权阵
	//矩阵元素初始化
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
	double maxV = 0;//记录最大改正数
	double maxdx = 0;//记录最大改正数
	int ncount = 0;//迭代次数
	//迭代平差计算
	do
	{
		maxV = 0;//记录最大改正数
		maxdx = 0;//记录最大改正数
		int Brow = 0;//记录当前B矩阵位置行数
		int Lrow = 0;//记录当前L矩阵位置行数
		//计算方向测量数据的误差方程
		for (int i = 0; i < istations; i++)
		{
			double Zi0 = Azimuth2(*bearings[i].station, *bearings[i].obs[0].po);
			for (int j = 0; j < bearings[i].num; j++)
			{
				B(Brow, i) = -1;//Z
				Point *po1 = bearings[i].station;
				Point *po2 = bearings[i].obs[j].po;
				int n1 = findpoint(po1);//找到在数组中的位置，矩阵中坐标顺序与数组相同
				int n2 = findpoint(po2);
				double Tij0 = Azimuth2(*po1, *po2);
				double aij = pll * (sin(Tij0)) / (Dist(*po1, *po2) * 1000);//距离化mm
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
				L(Lrow, 0) = -(standrad(-Zi0 + Tij0) - Lij)*pll;//常数项化秒
				Lrow++;
			}
		}
		//计算边长测量数据的误差方程
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
			L(Lrow, 0) = -(Sij0 - dists[i].dist) * 1000;//常数项化mm
			Lrow++;
		}
		//定权
		double u = 0;//单位权中误差
		int Prow = 0;//权行数
		for (int i = 0; i < istations; i++)//角度观测值定权
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
		for (int i = iangles; i < iangles + idists; i++)//边长观测值定权
		{
			if (dists[i - iangles].B != 0)
			{
				double m = sqrt(dists[i - iangles].dist)*dists[i - iangles].B;
				u = u == 0 ? m : u;
				P(i, i) = u * u / (m*m);
			}
			else P(i, i) = 100.0 / dists[i - iangles].dist;
		}
		//检查矩阵test
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
		//平差计算
		dx = (~B*P*B).Inv()*(~B*P*L);
		//改正近似坐标
		for (int i = 0; i < ipuknow; i++)
		{
			points[i + ipknow].x += dx(istations + 2 * i, 0) / 1000.0;//改正未知点坐标（mm化m）
			points[i + ipknow].y += dx(istations + 2 * i + 1, 0) / 1000.0;
		}
		//改正观测值
		V = B * dx - L;
		Vsum = Vsum + V;//累加改正数
		int Vrow = 0;
		for (int i = 0; i < istations; i++)
		{
			int Vtem = 0;
			for (int j = 0; j < bearings[i].num; j++)
			{
				if (j == 0)
				{
					Vtem = Vrow;//记录起始照准角行
					Vsum(Vtem, 0) = 0;//起始照准角为0
					Vrow++;
					continue;
				}
				bearings[i].obs[j].rad += ((V(Vrow, 0) - V(Vtem, 0)) / pll);//起始照准角改正在此体现
				Vsum(Vrow, 0) -= V(Vtem, 0);//修正累加改正数
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
	//精度评定
	V = Vsum;
	m0 = sqrt((~V*P*V)(0, 0) / (iangles + idists - 2 * ipuknow - istations));//中误差
	CMatrix Qx = (~B*P*B).Inv();//协因数矩阵
	//观测值精度评定

	//误差椭圆参数计算
	for (int i = 0; i < ipuknow; i++)
	{
		double Qxx = Qx(istations + 2 * i, istations + 2 * i);
		double Qyy = Qx(istations + 2 * i + 1, istations + 2 * i + 1);
		double Qxy = Qx(istations + 2 * i, istations + 2 * i + 1);
		double Q = standrad(0.5*atan(2 * Qxy / (Qxx - Qyy)));
		double El = m0 * sqrt(Qxx + Qxy * tan(Q));
		double Es = m0 * sqrt(Qxx + Qxy * tan(Q + pi / 2));
		double m = m0 * sqrt(Qxx + Qyy);
		points[i + ipknow].circle.Q = Q < pi ? Q : Q - pi;//限制方位角到0-pi
		points[i + ipknow].circle.El = El;
		points[i + ipknow].circle.Es = Es;
		points[i + ipknow].m = m;
	}
}

//返回输出字符串
CString traversenet::OutStr()
{
	CString outstr, strtem;
	strtem.Format(_T("%.6lf"), m0);
	outstr += _T("中误差m0：");
	outstr += strtem;
	outstr += _T("\r\n已知点坐标：\r\n点名\tX(m)        \tY(m)\r\n");
	for (int i = 0; i < ipknow; i++)
	{
		strtem.Format(_T("\t%.6lf\t%.6lf\r\n"), points[i].x, points[i].y);
		outstr = outstr + points[i].name + strtem;
	}
	outstr += _T("未知点坐标：\r\n点名\tX(m)        \tY(m)\r\n");
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		strtem.Format(_T("\t%.6lf\t%.6lf\t\r\n"), points[i].x, points[i].y);
		outstr = outstr + points[i].name + strtem;
	}
	outstr += _T("未知点误差参数：\r\n点名\t长轴m\t短轴m\t长轴方位dms\t点位中误差m\r\n");
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		strtem.Format(_T("\t%.4lf\t%.4lf\t%.6lf  \t%.4lf\r\n"),
			points[i].circle.El / 1000.0, points[i].circle.Es / 1000.0, RAD2DMS(points[i].circle.Q), points[i].m / 1000.0);
		outstr = outstr + points[i].name + strtem;
	}
	outstr += _T("边长观测成果：\r\n测站\t照准\t距离m        \t改正数m\t平差后值m\t方位角dms\r\n");
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
	outstr += _T("角度观测成果：\r\n测站\t照准\t方向值dms\t改正数s\t平差后值dms\r\n");
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

//计算比例尺，输出绘图界限，返回比例尺
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
	double m = min((rect.Height() - 125) / dx, (rect.Width() - 100) / dy);//缩放比例，留出空间
	return m;
}

//绘图坐标转换，输入坐标，输出客户区对应点
CPoint traversenet::drawPoint(double x, double y)
{
	double xmax, xmin, ymax, ymin;
	double m = drawM(xmax, xmin, ymax, ymin);
	return CPoint(m*(y - ymin) + 80, m*(xmax - x) + 80);//大地坐标系转像平面坐标，留出空间
}

//绘图坐标转换，输入坐标，输出客户区对应点
CPoint traversenet::drawPoint(Point po)
{
	double xmax, xmin, ymax, ymin;
	double m = drawM(xmax, xmin, ymax, ymin);
	return CPoint(m*(po.y - ymin) + 80, m*(xmax - po.x) + 80);
}

//椭圆辅助，输入椭圆点坐标，椭圆中心点，DC指针
void traversenet::EllipsePoints(int x, int y, Point center, CDC * pDC)
{
	//旋转结果
	double dx, dy;
	//四个象限对称
	rotate(x, y, dx, dy, center.circle.Q);//旋转方位角
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
	rotate(-x, y, dx, dy, center.circle.Q);
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
	rotate(-x, -y, dx, dy, center.circle.Q);
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
	rotate(x, -y, dx, dy, center.circle.Q);
	pDC->SetPixel(drawPoint(center.x + dx, center.y + dy), RGB(255, 0, 0));
}

//绘制误差椭圆，输入DC指针，点，比例
void traversenet::Midpointellispe(CDC * pDC, Point center, double m)
{
	int x, y;
	double d1, d2;
	int a = center.circle.El * m;
	int b = center.circle.Es * m;
	CPen penLine(PS_SOLID, 1, RGB(255, 0, 0));
	CPen *oldpen = pDC->SelectObject(&penLine);
	//绘制长短轴
	double dx, dy;//旋转结果
	rotate(-a, 0, dx, dy, center.circle.Q);//旋转方位角
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
	d1 = b * b + a * a*(-b + 0.25);//初始值
	EllipsePoints(x, y, center, pDC);
	while (b*b*(x + 1) < a*a*(y - 0.5))//弧上斜率为–1 的点处，如果在下一个点，不等号改变方向，则说明椭圆弧从上部分转入下部分
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
	//下部分椭圆
	d2 = sqrt(b*(x + 0.5)) + a * (y - 1) - a * b;//初始值
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

//旋转函数，输入坐标，输出旋转后坐标，输入角度（弧度顺时针）
void traversenet::rotate(double x0, double y0, double & x, double & y, double the)
{
	//逆时针旋转算法，使用负角顺时针
	x = int(x0 * cos(-the) + y0 * sin(-the) + 0.5);
	y = int(y0 * cos(-the) - x0 * sin(-the) + 0.5);
}

//构造函数
traversenet::traversenet()
{
}

//析构函数
traversenet::~traversenet()
{
	deletedata();
}

//清空数据，释放内存
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

//读取文件，输出文件内的内容，返回是否成功读取
int traversenet::ReadFile(CString &strRead)
{
	if (points.GetSize())
	{
		deletedata();
	}
	CFileDialog getfile(TRUE, _T("txt"), _T("一级导线测试2.txt"));
	if (getfile.DoModal() != IDOK)return -1;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeRead))return 0;
	CString strline;
	CStringArray strtem;
	strRead.Empty();//清空
	if(!file.ReadString(strline))return 2;//第1行，无则为空
	strRead += strline;
	if(!file.ReadString(strline))return false;//第2行：已知点数
	strRead = strRead + _T("\r\n") + strline;
	ipknow = _tstoi(strline);
	for (int i = 0; i < ipknow; i++)//读取已知点数据
	{
		if(!file.ReadString(strline))return false;//下一行：已知点数据
		strRead = strRead + _T("\r\n") + strline;
		int n = SplitStringArray(strline, ',', strtem);
		if (n != 4)return false;
		Point temp;//点对象
		temp.name = strtem[0];
		temp.know = true;
		temp.flag = true;
		temp.x = _tstof(strtem[2]);
		temp.y = _tstof(strtem[3]);
		points.Add(temp);//加入点数组中
	}
	if(!file.ReadString(strline))return false;//未知点标头
	strRead = strRead + _T("\r\n") + strline;
	if(!file.ReadString(strline))return false;//未知点个数
	strRead = strRead + _T("\r\n") + strline;
	ipuknow = _tstoi(strline);
	if(!file.ReadString(strline))return false;//未知点名称
	strRead = strRead + _T("\r\n") + strline;
	int n = SplitStringArray(strline, ',', strtem);
	if (n != ipuknow)return false;
	for (int i = 0; i < n; i++)//读取未知点名称
	{
		Point temp;
		temp.name = strtem[i];
		temp.know = false;
		temp.flag = false;
		points.Add(temp);
	}
	if(!file.ReadString(strline))return false;//边长测量数据标头
	strRead = strRead + _T("\r\n") + strline;
	if(!file.ReadString(strline))return false;//边长测量数据个数
	strRead = strRead + _T("\r\n") + strline;
	idists = _tstoi(strline);
	dists = new DistObs[idists];
	for (int i = 0; i < idists; i++)//读取边长测量数据
	{
		if(!file.ReadString(strline))return false;//读一行边长测量数据
		strRead = strRead + _T("\r\n") + strline;
		n = SplitStringArray(strline, ',', strtem);
		if (n < 3)return false;
		dists[i].po1 = findpoint(strtem[0]);
		dists[i].po2 = findpoint(strtem[1]);
		if (dists[i].po1 == nullptr || dists[i].po2 == nullptr)return false;//没找到点
		dists[i].dist = _tstof(strtem[2]);
		dists[i].A = dists[i].B = 0;
		if (n == 4)dists[i].B = _tstof(strtem[3]);
		if (n == 5)dists[i].A = _tstof(strtem[4]);
	}
	if(!file.ReadString(strline))return false;//方位数据标头
	strRead = strRead + _T("\r\n") + strline;
	if(!file.ReadString(strline))return false;//方位测站数和角度个数
	strRead = strRead + _T("\r\n") + strline;
	n = SplitStringArray(strline, ',', strtem);
	if (n != 2)return false;
	istations = _tstoi(strtem[0]);
	iangles = _tstoi(strtem[1]);
	bearings = new BearingObs[istations];
	for (int i = 0; i < istations; i++)//读取方向测量数据
	{
		if(!file.ReadString(strline))return false;//测站名，照准数
		strRead = strRead + _T("\r\n") + strline;
		n = SplitStringArray(strline, ',', strtem);
		if (n != 2)return false;
		bearings[i].station = findpoint(strtem[0]);
		if (bearings[i].station == nullptr)return false;
		int item = _tstoi(strtem[1]);//照准数
		bearings[i].num = item;
		bearings[i].obs = new DirectObs[item];
		for (int j = 0; j < item; j++)//读取照准点数据
		{
			if(!file.ReadString(strline))return false;//照准点名，角度（度分秒）
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
	datearrange();//概算
	AdjustCal();//平差
	return 1;
}

//生成结果字符串
CString traversenet::ResultStr()
{
	return OutStr();
}

//保存文件，返回是否成功保存
int traversenet::SaveFile()
{
	if (!points.GetSize())return 0;
	CString strout = ResultStr();//调用函数
	strout.Remove('\r');
	CFileDialog getfile(FALSE, _T("txt"), _T("导线网平差结果.txt"));
	if (getfile.DoModal() != IDOK)return 2;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))return -1;
	setlocale(LC_ALL, "chs");//没有这句将无法输出中文
	file.WriteString(strout);
	file.Close();
	return 1;
}

//绘制导线网，输入DC指针和客户区大小，返回是否绘制
bool traversenet::Drawnet(CDC * pDC, CRect rc)
{
	if (!points.GetSize())return false;
	rect = rc;//储存客户区大小
	double xmax, xmin, ymax, ymin;
	double m = drawM(xmax, xmin, ymax, ymin);//获取比例尺和绘图边界
	CPen penNet(PS_SOLID, 1, RGB(150, 200, 100));//绘制角度观测网笔
	CPen penLine(PS_SOLID, 3, RGB(100, 200, 255));//绘制边长观测笔
	CPen penXY(PS_SOLID, 1, RGB(150, 150, 150));//绘制坐标值笔
	CPen penXYLine(PS_SOLID, 1, RGB(200, 200, 200));//绘制网格笔
	CPen penPoint(PS_SOLID, 3, RGB(0, 0, 255));//绘制已知点笔
	CPen *oldpen = pDC->SelectObject(&penXY);
	xmin -= 20/m;//坐标轴与点留白
	ymin -= 20/m;
	double dx = (xmax - xmin) / 5;//网格间隔
	double dy = (ymax - ymin) / 5;
	//绘制坐标轴
	pDC->MoveTo(drawPoint(xmax + 10 / m, ymin));
	pDC->LineTo(drawPoint(xmin, ymin));
	pDC->LineTo(drawPoint(xmin, ymax + 10 / m));
	//绘制坐标轴箭头
	pDC->MoveTo(drawPoint(xmax, ymin - 5 / m));
	pDC->LineTo(drawPoint(xmax + 10 / m, ymin));
	pDC->LineTo(drawPoint(xmax, ymin + 5 / m));
	pDC->MoveTo(drawPoint(xmin - 5 / m, ymax));
	pDC->LineTo(drawPoint(xmin, ymax + 10 / m));
	pDC->LineTo(drawPoint(xmin + 5 / m, ymax));
	//绘制网格
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
	CFont xyFont;//定义字体
	xyFont.CreateFont(12, 0, 0, 0, 0, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("宋体"));
	pDC->SelectObject(xyFont);
	pDC->SetBkMode(0);//透明背景
	pDC->SetTextColor(RGB(10, 10, 10));
	CString strtem;
	strtem = _T("Y");
	CPoint temp = drawPoint(xmin + 5 / m, ymax + 20 / m);
	pDC->TextOutW(temp.x, temp.y, strtem);
	strtem = _T("X");
	temp = drawPoint(xmax + 20 / m, ymin - 5 / m);
	pDC->TextOutW(temp.x, temp.y, strtem);
	//绘制网格对应坐标
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
	//绘制导线网
	pDC->SelectObject(&penNet);
	for (int i = 0; i < istations; i++)
	{
		for (int j = 0; j < bearings[i].num; j++)
		{
			pDC->MoveTo(drawPoint(*bearings[i].station));
			pDC->LineTo(drawPoint(*bearings[i].obs[j].po));
			if (j > 0)
			{
				//照准点之间连线
				pDC->MoveTo(drawPoint(*bearings[i].obs[j - 1].po));
				pDC->LineTo(drawPoint(*bearings[i].obs[j].po));
			}
		}
	}
	//绘制导线（覆盖）
	pDC->SelectObject(&penLine);
	for (int i = 0; i < idists; i++)
	{
		pDC->MoveTo(drawPoint(*dists[i].po1));
		pDC->LineTo(drawPoint(*dists[i].po2));
	}
	//绘制已知点标志
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
	//绘制控制点连线
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
	//释放资源
	pDC->SelectObject(oldpen);
	penNet.DeleteObject();
	penLine.DeleteObject();
	penXY.DeleteObject();
	penXYLine.DeleteObject();
	xyFont.DeleteObject();
	return true;
}

//绘制误差椭圆，会先绘制导线网
bool traversenet::Drawcircle(CDC * pDC, CRect rc, int size)
{
	if (!Drawnet(pDC, rc))return false;//先绘制导线网
	double maxE = 0;//最大误差椭圆长轴
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		maxE = max(maxE, points[i].circle.El);
		maxE = max(maxE, points[i].circle.Es);
	}
	CPoint a = drawPoint(0, 0);
	CPoint b = drawPoint(0, 100);
	//比例尺，使最大长轴绘制为size像素长度
	double m = fabs(100.0 / (b.x - a.x)*size / maxE);
	//绘制误差椭圆
	for (int i = ipknow; i < ipknow + ipuknow; i++)
	{
		Midpointellispe(pDC, points[i], m);
	}
	return true;
}

//标注点名，会先绘制导线网
bool traversenet::Drawname(CDC * pDC, CRect rc)
{
	if (!Drawnet(pDC, rc))return false;//绘制导线网
	CFont nameFont;//定义字体
	nameFont.CreateFont(12, 0, 0, 0, 0, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("宋体"));
	CFont *oldpen = pDC->SelectObject(&nameFont);
	pDC->SetBkMode(2);//白色背景
	pDC->SetTextColor(RGB(0, 0, 0));
	//标出点名
	for (int i = 0; i < points.GetSize(); i++)
	{
		CPoint temp = drawPoint(points[i].x, points[i].y);
		pDC->TextOutW(temp.x, temp.y, points[i].name);
	}
	//释放资源
	pDC->SelectObject(oldpen);
	nameFont.DeleteObject();
	return true;
}

