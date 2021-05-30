#pragma once
#include <vector>

using namespace std;

struct point3d
{
	double x, y, z;
};

class DEM
{
	int nx, ny;//格网大小
	double xlo, xhi, ylo, yhi, zlo, zhi;//xyz极值
	double *heights;//高程值
	vector<point3d>points;//已知点
	int SplitStringArray(CString Str, char split, CStringArray& aStr);//字符串分割
	void findClose10(double x, double y, int* po, double* dist2, int num);//找出最邻近点序号以及距离平方
public:
	DEM();
	~DEM();
	void deleteData();//清空数据
	void openDatafile();//打开文件
	void creatNet(int x, int y, double searchR);//格网计算
	void saveDatafile();//保存文件NSAA
};

