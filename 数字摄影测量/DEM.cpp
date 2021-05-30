#include "pch.h"
#include "DEM.h"
#include "Matrix.h"

int DEM::SplitStringArray(CString str, char split, CStringArray& aStr)
{
	int startIdx = 0;
	int idx = str.Find(split, startIdx);
	aStr.RemoveAll();//先清空

	while (-1 != idx)
	{
		CString sTmp = str.Mid(startIdx, idx - startIdx);
		aStr.Add(sTmp);
		startIdx = idx + 1;
		idx = str.Find(split, startIdx);
	}
	CString sTmp = str.Right(str.GetLength() - startIdx);
	if (!sTmp.IsEmpty())
		aStr.Add(sTmp);
	return aStr.GetSize();
}

void DEM::findClose10(double x, double y, int* po, double* dist2, int num)
{
	int size = points.size();
	double maxdist2 = 0;//初始化最大距离和对应点序号
	int maxpo = 0;
	for (int i = 0; i < num; i++) {//先把待定点数组填满
		po[i] = i;
		dist2[i] = (x - points[i].x) * (x - points[i].x) + (y - points[i].y) * (y - points[i].y);
		if (dist2[i] > maxdist2) {
			maxdist2 = dist2[i];
			maxpo = i;
		}
	}
	for (int i = num; i < size; i++) {//逐步替换以找到最邻近点
		double dist2t= (x - points[i].x) * (x - points[i].x) + (y - points[i].y) * (y - points[i].y);
		if (dist2t < maxdist2) {//替换最大距离和序号
			po[maxpo] = i;
			dist2[maxpo] = dist2t;
			maxdist2 = maxpo = 0;
			for (int j = 0; j < num; j++) {//重新确定最大距离和序号
				if (dist2[j] > maxdist2) {
					maxdist2 = dist2[j];
					maxpo = j;
				}
			}
		}
	}
}

DEM::DEM()
{
	deleteData();
}

DEM::~DEM()
{
	deleteData();
}

void DEM::deleteData()
{
	nx = ny = 0;
	xlo = xhi = ylo = yhi = zlo = zhi = 0;
	points.clear();
	points.resize(0);
	if (heights) {
		delete[] heights;
	}
}

void DEM::openDatafile()
{
	CFileDialog getfile(TRUE, _T(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("txt|*.txt||"));
	if (getfile.DoModal() != IDOK)return;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	file.Open(pathname, CFile::modeRead);
	CString strline, strtem;
	CStringArray strarray;
	bool flag = file.ReadString(strline);//点数
	points.resize(_tstoi(strline));
	xlo = ylo = zlo = 10000000000.0;
	int i = 0;
	while (flag) {
		flag = file.ReadString(strline);
		int n = SplitStringArray(strline, ' ', strarray);
		if (n != 6)continue;
		points[i].x = _tstof(strarray[0]);
		points[i].y = _tstof(strarray[1]);
		points[i].z = _tstof(strarray[2]);
		xlo = min(xlo, points[i].x);
		ylo = min(ylo, points[i].y);
		zlo = min(zlo, points[i].z);
		xhi = max(xhi, points[i].x);
		yhi = max(yhi, points[i].y);
		zhi = max(zhi, points[i].z);
		i++;
	}
	file.Close();
}

void DEM::creatNet(int x, int y, double searchR)
{
	nx = x;
	ny = y;
	heights=new double[x * y];//确定高程数量
	double detx = (xhi - xlo) / x;//格网步长
	double dety = (yhi - ylo) / y;
	int m = searchR;//遗留问题，不影响
	point3d* hitem = new point3d[m];//用于保存每次用于平差的点
	double xx = 0;//当前格网点坐标
	double yy = 0;
	double _x = 0;//均值，用于重心化
	double _y = 0;
	double _z = 0;
	int* ponum = new int[m];//储存最邻近点序号
	double* podist = new double[m];//储存最邻近点距离
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			xx = xlo + i * detx;//计算当前格网点坐标
			yy = ylo + j * dety;
			_x = _y = _z = 0;
			//寻找最邻近点
			findClose10(xx, yy, ponum, podist, m);
			//获取最邻近点，并且计算坐标均值
			for (int k = 0; k < 10; k++) {
				hitem[k] = points[ponum[k]];
				_x += hitem[k].x;
				_y += hitem[k].y;
				//_z += hitem[k].z;
			}
			_x = _x / m;
			_y = _y / m;
			//_z = _z / m;
			//定义矩阵
			CMatrix B(m, 6);
			CMatrix L(m, 1);
			CMatrix P(m, m);//默认为0矩阵
			//赋值
			for (int k = 0; k < m; k++) {
				L(k, 0) = hitem[k].z;
				double x_ = hitem[k].x - _x;
				double y_ = hitem[k].y - _y;
				B(k, 0) = x_ * x_;
				B(k, 1) = x_ * y_;
				B(k, 2) = y_ * y_;
				B(k, 3) = x_;
				B(k, 4) = y_;
				B(k, 5) = 1;
				P(k, k) = 1 / podist[k];//权值定位距离平方倒数
			}
			CMatrix X = (~B * P * B).Inv() * (~B * P * L);
			heights[i * y + j] = (X(5, 0));//高程结果
		}
	}
	//释放内存
	if (hitem) {
		delete[] hitem;
	}
	if (ponum) {
		delete[] ponum;
	}
	if (podist) {
		delete[] podist;
	}
}

void DEM::saveDatafile()
{
	CFileDialog getfile(FALSE, _T(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("txt|*.txt||"));
	if (getfile.DoModal() != IDOK)return;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	file.Open(pathname, CFile::modeCreate | CFile::modeWrite);
	CString str, strtem;
	str.Format(_T("DSAA\n%d %d\n%lf %lf\n%lf %lf\n%lf %lf\n"), nx, ny, xlo, xhi, ylo, yhi, zlo, zhi);
	for (int i = 0; i < ny; i++) {//xy坐标互换位置（大地坐标系）
		for (int j = 0; j < nx; j++) {
			strtem.Format(_T("%lf "), heights[i + j * ny]);
			str += strtem;
		}
		str += _T("\n");
	}
	file.WriteString(str);
	file.Close();
}
