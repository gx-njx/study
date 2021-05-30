/////////////////////////////////////////////////////////////
//文件名：traversenet.h
//导线网平差计算类
//作者：郭鑫
/////////////////////////////////////////////////////////////

#pragma once
struct AdjustCircle//误差椭圆结构体
{
	double Q;//长轴方位角
	double El, Es;//长半轴，短半轴
};

struct Point//测点结构体
{
	CString name;//名称
	bool know;//已知点
	bool flag;//坐标标识
	double x, y;//坐标
	AdjustCircle circle;//误差椭圆
	double m;//点位中误差
};

struct DistObs//边长测量数据结构体
{
	Point *po1, *po2;//起点、终点指针
	double dist, v, B, A;//距离，改正数，先验精度
};

struct DirectObs//照准数据结构体
{
	Point *po;//照准点
	double rad, v, m;//方向观测值（弧度），改正数（秒），先验精度
};

struct BearingObs//方向测量数据结构体
{
	Point *station;//测站点
	int num;//照准点数
	DirectObs *obs;//方向观测数据
};

struct MainObs//计算数据结构体
{
	Point *po1, *po2;//起点、终点指针
	double dist, rad;//距离，方位角（弧度）
};

class traversenet//导线网平差类
{
	int ipknow, ipuknow, idists, istations, iangles;//已知点数，未知点数，距离数，测站数，照准角度数
	double m0;//中误差
	CArray<Point, Point&>points;//点数据
	DistObs *dists;//边长测量数据
	BearingObs *bearings;//方向测量数据
	CArray<MainObs, MainObs&>datas;//主要数据
	CRect rect;//客户区
	int SplitStringArray(CString str, char split, CStringArray& aStr);//分割字符串
	Point *findpoint(CString name);//由名称找点
	int findpoint(Point *po);//由点指针返回索引
	MainObs *findmObs(Point *po1, Point *po2, bool &sgn);//由点找计算数据
	double DMS2RAD(double dDms);//度分秒转弧度
	double RAD2DMS(double dRad);//弧度转度分秒
	double Dist(Point a, Point b);//求距离
	double Azimuth2(Point a, Point b);//得到方位角
	double standrad(double rad);//角度标准化
	void datearrange();//整理数据，概算
	void AdjustCal();//平差计算
	CString OutStr();//生成输出字符串
	double drawM(double &xmax, double &xmin, double &ymax, double &ymin);//比例尺计算
	CPoint drawPoint(double x, double y);//坐标转换
	CPoint drawPoint(Point po);//坐标转换（重载）
	void EllipsePoints(int x, int y, Point center, CDC * pDC);//椭圆辅助画点
	void Midpointellispe(CDC * pDC, Point center, double m);//绘制误差椭圆
	void rotate(double x0, double y0, double &x, double &y, double the);//旋转函数
public:
	traversenet();
	~traversenet();//析构函数释放资源
	void deletedata();//清空数据
	int ReadFile(CString &strRead);//打开文件并读取
	CString ResultStr();//生成结果字符串
	int SaveFile();//保存为文件
	bool Drawnet(CDC *pDC, CRect rc);//绘制导线网
	bool Drawcircle(CDC *pDC, CRect rc, int size);//绘制误差椭圆
	bool Drawname(CDC *pDC, CRect rc);//绘制点名
};

