/////////////////////////////////////////////////////////////
//�ļ�����traversenet.h
//������ƽ�������
//���ߣ�����
/////////////////////////////////////////////////////////////

#pragma once
struct AdjustCircle//�����Բ�ṹ��
{
	double Q;//���᷽λ��
	double El, Es;//�����ᣬ�̰���
};

struct Point//���ṹ��
{
	CString name;//����
	bool know;//��֪��
	bool flag;//�����ʶ
	double x, y;//����
	AdjustCircle circle;//�����Բ
	double m;//��λ�����
};

struct DistObs//�߳��������ݽṹ��
{
	Point *po1, *po2;//��㡢�յ�ָ��
	double dist, v, B, A;//���룬�����������龫��
};

struct DirectObs//��׼���ݽṹ��
{
	Point *po;//��׼��
	double rad, v, m;//����۲�ֵ�����ȣ������������룩�����龫��
};

struct BearingObs//����������ݽṹ��
{
	Point *station;//��վ��
	int num;//��׼����
	DirectObs *obs;//����۲�����
};

struct MainObs//�������ݽṹ��
{
	Point *po1, *po2;//��㡢�յ�ָ��
	double dist, rad;//���룬��λ�ǣ����ȣ�
};

class traversenet//������ƽ����
{
	int ipknow, ipuknow, idists, istations, iangles;//��֪������δ֪����������������վ������׼�Ƕ���
	double m0;//�����
	CArray<Point, Point&>points;//������
	DistObs *dists;//�߳���������
	BearingObs *bearings;//�����������
	CArray<MainObs, MainObs&>datas;//��Ҫ����
	CRect rect;//�ͻ���
	int SplitStringArray(CString str, char split, CStringArray& aStr);//�ָ��ַ���
	Point *findpoint(CString name);//�������ҵ�
	int findpoint(Point *po);//�ɵ�ָ�뷵������
	MainObs *findmObs(Point *po1, Point *po2, bool &sgn);//�ɵ��Ҽ�������
	double DMS2RAD(double dDms);//�ȷ���ת����
	double RAD2DMS(double dRad);//����ת�ȷ���
	double Dist(Point a, Point b);//�����
	double Azimuth2(Point a, Point b);//�õ���λ��
	double standrad(double rad);//�Ƕȱ�׼��
	void datearrange();//�������ݣ�����
	void AdjustCal();//ƽ�����
	CString OutStr();//��������ַ���
	double drawM(double &xmax, double &xmin, double &ymax, double &ymin);//�����߼���
	CPoint drawPoint(double x, double y);//����ת��
	CPoint drawPoint(Point po);//����ת�������أ�
	void EllipsePoints(int x, int y, Point center, CDC * pDC);//��Բ��������
	void Midpointellispe(CDC * pDC, Point center, double m);//���������Բ
	void rotate(double x0, double y0, double &x, double &y, double the);//��ת����
public:
	traversenet();
	~traversenet();//���������ͷ���Դ
	void deletedata();//�������
	int ReadFile(CString &strRead);//���ļ�����ȡ
	CString ResultStr();//���ɽ���ַ���
	int SaveFile();//����Ϊ�ļ�
	bool Drawnet(CDC *pDC, CRect rc);//���Ƶ�����
	bool Drawcircle(CDC *pDC, CRect rc, int size);//���������Բ
	bool Drawname(CDC *pDC, CRect rc);//���Ƶ���
};

