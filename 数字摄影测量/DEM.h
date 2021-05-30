#pragma once
#include <vector>

using namespace std;

struct point3d
{
	double x, y, z;
};

class DEM
{
	int nx, ny;//������С
	double xlo, xhi, ylo, yhi, zlo, zhi;//xyz��ֵ
	double *heights;//�߳�ֵ
	vector<point3d>points;//��֪��
	int SplitStringArray(CString Str, char split, CStringArray& aStr);//�ַ����ָ�
	void findClose10(double x, double y, int* po, double* dist2, int num);//�ҳ����ڽ�������Լ�����ƽ��
public:
	DEM();
	~DEM();
	void deleteData();//�������
	void openDatafile();//���ļ�
	void creatNet(int x, int y, double searchR);//��������
	void saveDatafile();//�����ļ�NSAA
};

