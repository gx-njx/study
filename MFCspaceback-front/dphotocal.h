#pragma once
struct point2//像点坐标
{
	double x, y;
};
struct point3//物方坐标
{
	double x, y, z;
};
struct GCP//点数据结构体
{
	point2 lp;//左像点
	point2 rp;//右像点
	point3 pp;//结果坐标
};
struct inelement//内方位元素
{
	double x0, y0, f;
};
struct outelement//外方位元素
{
	double Xs, Ys, Zs, fa, w, k;
};
struct adjustdata
{
	outelement lpment, rpment;//左右外方位元素
	point3 *points;//未知点坐标
};
class dphotocal
{
	int npknow, npuknow;//已知点未知点数
	GCP *gcps;//所有点数据指针
	inelement inment;//内方位元素
	outelement lpment, rpment;//左右外方位元素
	adjustdata jingdu;//精度
	int SplitStringArray(CString str, char split, CStringArray & aStr);
	double dist(point2 a, point2 b);//求平面距离
	double dist(point3 a, point3 b);//重载
	void spaceback();//后方交会求外方位元素
	void spacefront();//立体像对前方交会（点投影系数法）
	void readstr(CString input);//读取输入框数据
	void outstr(CString &output);//输出结果数据
public:
	void mainfunc(CString input, CString &output);//计算过程
	bool readfile(CString &input);//读取文件数据
	bool savefile();//保存数据到文件
	~dphotocal();//析构函数释放资源
};

