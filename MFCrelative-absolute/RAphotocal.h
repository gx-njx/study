#pragma once
struct point2//像点坐标
{
	double x, y;
};
struct point3//空间坐标
{
	double x, y, z;
};
struct GCP//点数据结构体
{
	point2 lp;//左像点
	point2 rp;//右像点
	point3 pp;//结果坐标
	point3 fp;//辅助坐标
};
struct inelement//内方位元素
{
	double x0, y0, f;
};
struct relativement//相对定向元素
{
	double By, Bz, fa, w, k;
};
struct absolutement//绝对定向元素
{
	double rm, X0, Y0, Z0, FA, W, K;
};
class RAphotocal
{
	int npknow, npuknow;//已知点未知点数
	GCP *gcps;//所有点数据指针
	inelement inment;//内方位元素
	relativement rement;//相对定向元素
	relativement rejust;//相对定向元素精度
	absolutement abment;//绝对定向元素
	absolutement abjust;//绝对定向元素精度
	int SplitStringArray(CString str, char split, CStringArray & aStr);
	void readstr(CString input);//读取输入框数据
	void outstr(CString &output);//输出结果数据
	double dist(point2 a, point2 b);//求两点距离
	double dist(point3 a, point3 b);//求两点平面距离
	void relative();//相对定向计算
	void absolute();//绝对定向计算
public:
	void mainfunc(CString input, CString &output);//计算过程
	bool readfile(CString &input);//读取文件数据
	bool savefile();//保存数据到文件
	~RAphotocal();//析构函数释放资源
};

