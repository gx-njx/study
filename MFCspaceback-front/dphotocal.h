#pragma once
struct point2//�������
{
	double x, y;
};
struct point3//�﷽����
{
	double x, y, z;
};
struct GCP//�����ݽṹ��
{
	point2 lp;//�����
	point2 rp;//�����
	point3 pp;//�������
};
struct inelement//�ڷ�λԪ��
{
	double x0, y0, f;
};
struct outelement//�ⷽλԪ��
{
	double Xs, Ys, Zs, fa, w, k;
};
struct adjustdata
{
	outelement lpment, rpment;//�����ⷽλԪ��
	point3 *points;//δ֪������
};
class dphotocal
{
	int npknow, npuknow;//��֪��δ֪����
	GCP *gcps;//���е�����ָ��
	inelement inment;//�ڷ�λԪ��
	outelement lpment, rpment;//�����ⷽλԪ��
	adjustdata jingdu;//����
	int SplitStringArray(CString str, char split, CStringArray & aStr);
	double dist(point2 a, point2 b);//��ƽ�����
	double dist(point3 a, point3 b);//����
	void spaceback();//�󷽽������ⷽλԪ��
	void spacefront();//�������ǰ�����ᣨ��ͶӰϵ������
	void readstr(CString input);//��ȡ���������
	void outstr(CString &output);//����������
public:
	void mainfunc(CString input, CString &output);//�������
	bool readfile(CString &input);//��ȡ�ļ�����
	bool savefile();//�������ݵ��ļ�
	~dphotocal();//���������ͷ���Դ
};

