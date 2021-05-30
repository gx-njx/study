#pragma once
struct point2//�������
{
	double x, y;
};
struct point3//�ռ�����
{
	double x, y, z;
};
struct GCP//�����ݽṹ��
{
	point2 lp;//�����
	point2 rp;//�����
	point3 pp;//�������
	point3 fp;//��������
};
struct inelement//�ڷ�λԪ��
{
	double x0, y0, f;
};
struct relativement//��Զ���Ԫ��
{
	double By, Bz, fa, w, k;
};
struct absolutement//���Զ���Ԫ��
{
	double rm, X0, Y0, Z0, FA, W, K;
};
class RAphotocal
{
	int npknow, npuknow;//��֪��δ֪����
	GCP *gcps;//���е�����ָ��
	inelement inment;//�ڷ�λԪ��
	relativement rement;//��Զ���Ԫ��
	relativement rejust;//��Զ���Ԫ�ؾ���
	absolutement abment;//���Զ���Ԫ��
	absolutement abjust;//���Զ���Ԫ�ؾ���
	int SplitStringArray(CString str, char split, CStringArray & aStr);
	void readstr(CString input);//��ȡ���������
	void outstr(CString &output);//����������
	double dist(point2 a, point2 b);//���������
	double dist(point3 a, point3 b);//������ƽ�����
	void relative();//��Զ������
	void absolute();//���Զ������
public:
	void mainfunc(CString input, CString &output);//�������
	bool readfile(CString &input);//��ȡ�ļ�����
	bool savefile();//�������ݵ��ļ�
	~RAphotocal();//���������ͷ���Դ
};

