#include "pch.h"
#include "dphotocal.h"
#include "Matrix.h"
#include <math.h>
#include <locale>

const double pi = atan(1) * 4;


int dphotocal::SplitStringArray(CString str, char split, CStringArray & aStr)
{
	int startIdx = 0;
	int idx = str.Find(split, startIdx);
	aStr.RemoveAll();//�����

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

double dphotocal::dist(point2 a, point2 b)
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx*dx + dy * dy);
}

double dphotocal::dist(point3 a, point3 b)
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx*dx + dy * dy);
}

bool dphotocal::readfile(CString &input)
{
	CFileDialog getfile(TRUE, _T("txt"),_T("data"));
	if (getfile.DoModal() != IDOK)return false;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeRead))return false;
	CString strline, strinput;
	strinput = _T("");
	CStringArray strtem;
	bool flag = file.ReadString(strline);//��һ�У���֪������δ֪����
	strinput = strinput + strline + _T("\r\n");
	int n = SplitStringArray(strline, ',', strtem);
	npknow = _tstoi(strtem[0]);
	npuknow = _tstoi(strtem[1]);
	gcps = new GCP[npknow + npuknow];
	for (int i = 0; i < npknow; i++)
	{
		flag = file.ReadString(strline);
		strinput = strinput + strline + _T("\r\n");
		n = SplitStringArray(strline, ',', strtem);
		gcps[i].lp.x = _tstof(strtem[0]) / 1000.0;//��λͳһΪm
		gcps[i].lp.y = _tstof(strtem[1]) / 1000.0;
		gcps[i].rp.x = _tstof(strtem[2]) / 1000.0;
		gcps[i].rp.y = _tstof(strtem[3]) / 1000.0;
		gcps[i].pp.x = _tstof(strtem[4]);
		gcps[i].pp.y = _tstof(strtem[5]);
		gcps[i].pp.z = _tstof(strtem[6]);
	}
	for (int i = npknow; i < npknow + npuknow; i++)
	{
		flag = file.ReadString(strline);
		strinput = strinput + strline + _T("\r\n");
		n = SplitStringArray(strline, ',', strtem);
		gcps[i].lp.x = _tstof(strtem[0]) / 1000.0;
		gcps[i].lp.y = _tstof(strtem[1]) / 1000.0;
		gcps[i].rp.x = _tstof(strtem[2]) / 1000.0;
		gcps[i].rp.y = _tstof(strtem[3]) / 1000.0;
	}
	flag = file.ReadString(strline);
	strinput = strinput + strline + _T("\r\n");
	n = SplitStringArray(strline, ',', strtem);
	inment.x0 = _tstof(strtem[0]);
	inment.y0 = _tstof(strtem[1]);
	inment.f = _tstof(strtem[2]);
	file.Close();
	input = strinput;
	return true;
}

void dphotocal::spaceback()
{
	CMatrix R(3, 3);//��ת����
	//CMatrix V(npknow * 2, 6);
	CMatrix A(npknow * 2, 6);//����ϵ������
	CMatrix dx(6, 1);//Ԫ�ظ���������
	CMatrix L(npknow * 2, 1);//���������
	double m;//������
	double epsa = pi/180.0*1.0/3600;//�Ƕ��޲�
	double epsb = 0.01;//�����޲�
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//��Ƭ�ⷽλԪ�ؼ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	m = dist(gcps[0].pp, gcps[npknow - 1].pp) / dist(gcps[0].lp, gcps[npknow - 1].lp);//������
	lpment.fa = lpment.w = lpment.k = lpment.Xs = lpment.Ys = lpment.Zs = 0;//��ʼ���ⷽλԪ��

	lpment.Xs = (gcps[0].pp.x + gcps[2].pp.x) / 2.0;
	lpment.Ys = (gcps[0].pp.y + gcps[2].pp.y) / 2.0;
	lpment.Zs = (gcps[0].pp.z + gcps[2].pp.z) / 2.0 + m * inment.f;//��ʼֵʹ�ÿ�����ߵ�������
	//int ndol = 0, ndor = 0;
	do
	{
		//ndol++;
		//������ת����Y��Ϊ����
		R(0, 0) = cos(lpment.fa)*cos(lpment.k) - sin(lpment.fa)*sin(lpment.w)*sin(lpment.k);
		R(0, 1) = -cos(lpment.fa)*sin(lpment.k) - sin(lpment.fa)*sin(lpment.w)*cos(lpment.k);
		R(0, 2) = -sin(lpment.fa)*cos(lpment.w);
		R(1, 0) = cos(lpment.w)*sin(lpment.k);
		R(1, 1) = cos(lpment.w)*cos(lpment.k);
		R(1, 2) = -sin(lpment.w);
		R(2, 0) = sin(lpment.fa)*cos(lpment.k) + cos(lpment.fa)*sin(lpment.w)*sin(lpment.k);
		R(2, 1) = -sin(lpment.fa)*sin(lpment.k) + cos(lpment.fa)*sin(lpment.w)*cos(lpment.k);
		R(2, 2) = cos(lpment.fa)*cos(lpment.w);
		for (int i = 0; i < npknow; i++)
		{
			//���߷��̷��ӷ�ĸ
			double _X = R(0, 0)*(gcps[i].pp.x - lpment.Xs) + R(1, 0)*(gcps[i].pp.y - lpment.Ys) + R(2, 0)*(gcps[i].pp.z - lpment.Zs);
			double _Y = R(0, 1)*(gcps[i].pp.x - lpment.Xs) + R(1, 1)*(gcps[i].pp.y - lpment.Ys) + R(2, 1)*(gcps[i].pp.z - lpment.Zs);
			double _Z = R(0, 2)*(gcps[i].pp.x - lpment.Xs) + R(1, 2)*(gcps[i].pp.y - lpment.Ys) + R(2, 2)*(gcps[i].pp.z - lpment.Zs);
			//����ϵ������
			A(2 * i, 0) = (R(0, 0)*inment.f + R(0, 2)*(gcps[i].lp.x - inment.x0)) / _Z;
			A(2 * i, 1) = (R(1, 0)*inment.f + R(1, 2)*(gcps[i].lp.x - inment.x0)) / _Z;
			A(2 * i, 2) = (R(2, 0)*inment.f + R(2, 2)*(gcps[i].lp.x - inment.x0)) / _Z;
			A(2 * i, 3) = (gcps[i].lp.y - inment.y0)*sin(lpment.w) - ((gcps[i].lp.x - inment.x0)*((gcps[i].lp.x - inment.x0)*cos(lpment.k) - (gcps[i].lp.y - inment.y0)*sin(lpment.k)) / inment.f + inment.f * cos(lpment.k))*cos(lpment.w);
			A(2 * i, 4) = -inment.f * sin(lpment.k) - (gcps[i].lp.x - inment.x0)*((gcps[i].lp.x - inment.x0)*sin(lpment.k) + (gcps[i].lp.y - inment.y0)*cos(lpment.k)) / inment.f;
			A(2 * i, 5) = (gcps[i].lp.y - inment.y0);
			A(2 * i + 1, 0) = (R(0, 1)*inment.f + R(0, 2)*(gcps[i].lp.y - inment.y0)) / _Z;
			A(2 * i + 1, 1) = (R(1, 1)*inment.f + R(1, 2)*(gcps[i].lp.y - inment.y0)) / _Z;
			A(2 * i + 1, 2) = (R(2, 1)*inment.f + R(2, 2)*(gcps[i].lp.y - inment.y0)) / _Z;
			A(2 * i + 1, 3) = -(gcps[i].lp.x - inment.x0)*sin(lpment.w) - ((gcps[i].lp.y - inment.y0)*((gcps[i].lp.x - inment.x0)*cos(lpment.k) - (gcps[i].lp.y - inment.y0)*sin(lpment.k)) / inment.f - inment.f * sin(lpment.k))*cos(lpment.w);
			A(2 * i + 1, 4) = -inment.f * cos(lpment.k) - (gcps[i].lp.y - inment.y0)*((gcps[i].lp.x - inment.x0)*sin(lpment.k) + (gcps[i].lp.y - inment.y0)*cos(lpment.k)) / inment.f;
			A(2 * i + 1, 5) = -(gcps[i].lp.x - inment.x0);
			//���߷��̼���������ֵ����ó�������
			L(2 * i, 0) = gcps[i].lp.x + (inment.f*_X) / _Z;
			L(2 * i + 1, 0) = gcps[i].lp.y + (inment.f*_Y) / _Z;
		}
		//���ƽ�����
		dx = (~A*A).Inv()*~A*L;
		//����������
		lpment.Xs += dx(0, 0);
		lpment.Ys += dx(1, 0);
		lpment.Zs += dx(2, 0);
		lpment.fa += dx(3, 0);
		lpment.w += dx(4, 0);
		lpment.k += dx(5, 0);
	} while (fabs(dx(0, 0)) > epsb || fabs(dx(1, 0)) > epsb || fabs(dx(2, 0)) > epsb || fabs(dx(3, 0)) > epsa || fabs(dx(4, 0)) > epsa || fabs(dx(5, 0)) > epsa);
	//��������
	CMatrix V;
	V = A * dx - L;
	double the = sqrt((~V*V)(0, 0) / (2 * npknow - 6));//�����
	CMatrix Qxx;
	Qxx = (~A*A).Inv();//Э��������
	jingdu.lpment.Xs = the * sqrt(Qxx(0, 0));
	jingdu.lpment.Ys = the * sqrt(Qxx(1, 1));
	jingdu.lpment.Zs = the * sqrt(Qxx(2, 2));
	jingdu.lpment.fa = the * sqrt(Qxx(3, 3));
	jingdu.lpment.w = the * sqrt(Qxx(4, 4));
	jingdu.lpment.k = the * sqrt(Qxx(5, 5));
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//��Ƭ�ⷽλԪ�ؼ���
	/////////////////////////////////////////////////////////////////////////////////////////////////
	m = dist(gcps[0].pp, gcps[npknow - 1].pp) / dist(gcps[0].rp, gcps[npknow - 1].rp);//������
	rpment.fa = rpment.w = rpment.k = rpment.Xs = rpment.Ys = rpment.Zs = 0;//��ʼ���ⷽλԪ��
	rpment.Xs = (gcps[1].pp.x + gcps[3].pp.x) / 2.0;
	rpment.Ys = (gcps[1].pp.y + gcps[3].pp.y) / 2.0;
	rpment.Zs = (gcps[1].pp.z + gcps[3].pp.z) / 2.0 + m * inment.f;//��ʼֵʹ�ÿ����ұߵĵ�
	do
	{
		//ndor++;
		//������ת����Y��Ϊ����
		R(0, 0) = cos(rpment.fa)*cos(rpment.k) - sin(rpment.fa)*sin(rpment.w)*sin(rpment.k);
		R(0, 1) = -cos(rpment.fa)*sin(rpment.k) - sin(rpment.fa)*sin(rpment.w)*cos(rpment.k);
		R(0, 2) = -sin(rpment.fa)*cos(rpment.w);
		R(1, 0) = cos(rpment.w)*sin(rpment.k);
		R(1, 1) = cos(rpment.w)*cos(rpment.k);
		R(1, 2) = -sin(rpment.w);
		R(2, 0) = sin(rpment.fa)*cos(rpment.k) + cos(rpment.fa)*sin(rpment.w)*sin(rpment.k);
		R(2, 1) = -sin(rpment.fa)*sin(rpment.k) + cos(rpment.fa)*sin(rpment.w)*cos(rpment.k);
		R(2, 2) = cos(rpment.fa)*cos(rpment.w);
		for (int i = 0; i < npknow; i++)
		{
			//���߷��̷��ӷ�ĸ
			double _X = R(0, 0)*((gcps[i].pp.x - inment.x0) - rpment.Xs) + R(1, 0)*((gcps[i].pp.y - inment.y0) - rpment.Ys) + R(2, 0)*(gcps[i].pp.z - rpment.Zs);
			double _Y = R(0, 1)*((gcps[i].pp.x - inment.x0) - rpment.Xs) + R(1, 1)*((gcps[i].pp.y - inment.y0) - rpment.Ys) + R(2, 1)*(gcps[i].pp.z - rpment.Zs);
			double _Z = R(0, 2)*((gcps[i].pp.x - inment.x0) - rpment.Xs) + R(1, 2)*((gcps[i].pp.y - inment.y0) - rpment.Ys) + R(2, 2)*(gcps[i].pp.z - rpment.Zs);
			//����ϵ������
			A(2 * i, 0) = (R(0, 0)*inment.f + R(0, 2)*(gcps[i].rp.x - inment.x0)) / _Z;
			A(2 * i, 1) = (R(1, 0)*inment.f + R(1, 2)*(gcps[i].rp.x - inment.x0)) / _Z;
			A(2 * i, 2) = (R(2, 0)*inment.f + R(2, 2)*(gcps[i].rp.x - inment.x0)) / _Z;
			A(2 * i, 3) = (gcps[i].rp.y - inment.y0)*sin(rpment.w) - ((gcps[i].rp.x - inment.x0)*((gcps[i].rp.x - inment.x0)*cos(rpment.k) - (gcps[i].rp.y - inment.y0)*sin(rpment.k)) / inment.f + inment.f * cos(rpment.k))*cos(rpment.w);
			A(2 * i, 4) = -inment.f * sin(rpment.k) - (gcps[i].rp.x - inment.x0)*((gcps[i].rp.x - inment.x0)*sin(rpment.k) + (gcps[i].rp.y - inment.y0)*cos(rpment.k)) / inment.f;
			A(2 * i, 5) = (gcps[i].rp.y - inment.y0);
			A(2 * i + 1, 0) = (R(0, 1)*inment.f + R(0, 2)*(gcps[i].rp.y - inment.y0)) / _Z;
			A(2 * i + 1, 1) = (R(1, 1)*inment.f + R(1, 2)*(gcps[i].rp.y - inment.y0)) / _Z;
			A(2 * i + 1, 2) = (R(2, 1)*inment.f + R(2, 2)*(gcps[i].rp.y - inment.y0)) / _Z;
			A(2 * i + 1, 3) = -(gcps[i].rp.x - inment.x0)*sin(rpment.w) - ((gcps[i].rp.y - inment.y0)*((gcps[i].rp.x - inment.x0)*cos(rpment.k) + (gcps[i].rp.y - inment.y0)*sin(rpment.k)) / inment.f - inment.f * sin(rpment.k))*cos(rpment.w);
			A(2 * i + 1, 4) = -inment.f * cos(rpment.k) - (gcps[i].rp.y - inment.y0)*((gcps[i].rp.x - inment.x0)*sin(rpment.k) + (gcps[i].rp.y - inment.y0)*cos(rpment.k)) / inment.f;
			A(2 * i + 1, 5) = -(gcps[i].rp.x - inment.x0);
			//���߷��̼���������ֵ����ó�������
			L(2 * i, 0) = gcps[i].rp.x + (inment.f*_X) / _Z;
			L(2 * i + 1, 0) = gcps[i].rp.y + (inment.f*_Y) / _Z;
		}
		//���ƽ�����
		dx = (~A*A).Inv()*~A*L;
		//����������
		rpment.Xs += dx(0, 0);
		rpment.Ys += dx(1, 0);
		rpment.Zs += dx(2, 0);
		rpment.fa += dx(3, 0);
		rpment.w += dx(4, 0);
		rpment.k += dx(5, 0);
	} while (fabs(dx(0, 0)) > epsb || fabs(dx(1, 0)) > epsb || fabs(dx(2, 0)) > epsb || fabs(dx(3, 0)) > epsa || fabs(dx(4, 0)) > epsa || fabs(dx(5, 0)) > epsa);
	//��������
	//CMatrix V;
	V = A * dx - L;
	the = sqrt((~V*V)(0, 0) / (2 * npknow - 6));//�����
	//CMatrix Qxx;
	Qxx = (~A*A).Inv();//Э��������
	jingdu.rpment.Xs = the * sqrt(Qxx(0, 0));
	jingdu.rpment.Ys = the * sqrt(Qxx(1, 1));
	jingdu.rpment.Zs = the * sqrt(Qxx(2, 2));
	jingdu.rpment.fa = the * sqrt(Qxx(3, 3));
	jingdu.rpment.w = the * sqrt(Qxx(4, 4));
	jingdu.rpment.k = the * sqrt(Qxx(5, 5));
}

void dphotocal::spacefront()
{
	CMatrix Rl(3, 3);//��ת����
	CMatrix Rr(3, 3);
	CMatrix B(3, 1);//���߷�������
	//������Ƭ��ת����Y��Ϊ����
	Rl(0, 0) = cos(lpment.fa)*cos(lpment.k) - sin(lpment.fa)*sin(lpment.w)*sin(lpment.k);
	Rl(0, 1) = -cos(lpment.fa)*sin(lpment.k) - sin(lpment.fa)*sin(lpment.w)*cos(lpment.k);
	Rl(0, 2) = -sin(lpment.fa)*cos(lpment.w);
	Rl(1, 0) = cos(lpment.w)*sin(lpment.k);
	Rl(1, 1) = cos(lpment.w)*cos(lpment.k);
	Rl(1, 2) = -sin(lpment.w);
	Rl(2, 0) = sin(lpment.fa)*cos(lpment.k) + cos(lpment.fa)*sin(lpment.w)*sin(lpment.k);
	Rl(2, 1) = -sin(lpment.fa)*sin(lpment.k) + cos(lpment.fa)*sin(lpment.w)*cos(lpment.k);
	Rl(2, 2) = cos(lpment.fa)*cos(lpment.w);
	//������Ƭ��ת����Y��Ϊ����
	Rr(0, 0) = cos(rpment.fa)*cos(rpment.k) - sin(rpment.fa)*sin(rpment.w)*sin(rpment.k);
	Rr(0, 1) = -cos(rpment.fa)*sin(rpment.k) - sin(rpment.fa)*sin(rpment.w)*cos(rpment.k);
	Rr(0, 2) = -sin(rpment.fa)*cos(rpment.w);
	Rr(1, 0) = cos(rpment.w)*sin(rpment.k);
	Rr(1, 1) = cos(rpment.w)*cos(rpment.k);
	Rr(1, 2) = -sin(rpment.w);
	Rr(2, 0) = sin(rpment.fa)*cos(rpment.k) + cos(rpment.fa)*sin(rpment.w)*sin(rpment.k);
	Rr(2, 1) = -sin(rpment.fa)*sin(rpment.k) + cos(rpment.fa)*sin(rpment.w)*cos(rpment.k);
	Rr(2, 2) = cos(rpment.fa)*cos(rpment.w);
	//������߷���
	B(0, 0) = rpment.Xs - lpment.Xs;
	B(1, 0) = rpment.Ys - lpment.Ys;
	B(2, 0) = rpment.Zs - lpment.Zs;
	CMatrix lp(3, 1);//�����-f��ɾ���
	CMatrix rp(3, 1);
	CMatrix ls(3, 1);//����ڸ�������ϵ���������
	CMatrix rs(3, 1);
	for (int i = npknow; i < npknow + npuknow; i++)
	{
		//�����-f��ɾ���
		lp(0, 0) = gcps[i].lp.x;
		lp(1, 0) = gcps[i].lp.y;
		lp(2, 0) = -inment.f;
		rp(0, 0) = gcps[i].rp.x;
		rp(1, 0) = gcps[i].rp.y;
		rp(2, 0) = -inment.f;
		//����ڸ�������ϵ���������
		ls = Rl * lp;
		rs = Rr * rp;
		//ͶӰϵ��N1,N2
		double N1 = (B(0, 0)*rs(2, 0) - B(2, 0)*rs(0, 0)) / (ls(0, 0)*rs(2, 0) - ls(2, 0)*rs(0, 0));
		double N2 = (B(0, 0)*ls(2, 0) - B(2, 0)*ls(0, 0)) / (ls(0, 0)*rs(2, 0) - ls(2, 0)*rs(0, 0));
		//������ڸ�������ϵ������
		double dX = N1 * ls(0, 0);//N2 * rs(0, 0) + B(0, 0);
		double dY = (N1*ls(1, 0) + N2 * rs(1, 0) + B(1, 0)) / 2.0;
		double dZ = N1 * ls(2, 0);//N2 * rs(2, 0) + B(2, 0);
		//���������
		gcps[i].pp.x = lpment.Xs + dX;
		gcps[i].pp.y = lpment.Ys + dY;
		gcps[i].pp.z = lpment.Zs + dZ;
	}
}

void dphotocal::readstr(CString input)
{
	CStringArray strline;
	CStringArray strtem;
	int a = SplitStringArray(input, 13, strline);
	int n = SplitStringArray(strline[0], ',', strtem);
	npknow = _tstoi(strtem[0]);
	npuknow = _tstoi(strtem[1]);
	gcps = new GCP[npknow + npuknow];
	for (int i = 0; i < npknow; i++)
	{
		n = SplitStringArray(strline[i + 1], ',', strtem);
		gcps[i].lp.x = _tstof(strtem[0]) / 1000.0;//��λͳһΪm
		gcps[i].lp.y = _tstof(strtem[1]) / 1000.0;
		gcps[i].rp.x = _tstof(strtem[2]) / 1000.0;
		gcps[i].rp.y = _tstof(strtem[3]) / 1000.0;
		gcps[i].pp.x = _tstof(strtem[4]);
		gcps[i].pp.y = _tstof(strtem[5]);
		gcps[i].pp.z = _tstof(strtem[6]);
	}
	for (int i = npknow; i < npknow + npuknow; i++)
	{
		n = SplitStringArray(strline[i + 1], ',', strtem);
		gcps[i].lp.x = _tstof(strtem[0]) / 1000.0;
		gcps[i].lp.y = _tstof(strtem[1]) / 1000.0;
		gcps[i].rp.x = _tstof(strtem[2]) / 1000.0;
		gcps[i].rp.y = _tstof(strtem[3]) / 1000.0;
	}
	n = SplitStringArray(strline[npknow + npuknow + 1], ',', strtem);
	inment.x0 = _tstof(strtem[0]);
	inment.y0 = _tstof(strtem[1]);
	inment.f = _tstof(strtem[2]);
}

void dphotocal::outstr(CString & output)
{
	CString strout, tem;
	strout = _T("��Ƭ�ⷽλԪ�ؼ����ȣ�\r\n");
	tem.Format(_T("Xs=%.3lf\t\t���ȣ�%.6lf\r\n"), lpment.Xs, jingdu.lpment.Xs);
	strout += tem;
	tem.Format(_T("Ys=%.3lf\t\t���ȣ�%.6lf\r\n"), lpment.Ys, jingdu.lpment.Ys);
	strout += tem;
	tem.Format(_T("Zs=%.3lf\t\t���ȣ�%.6lf\r\n"), lpment.Zs, jingdu.lpment.Zs);
	strout += tem;
	tem.Format(_T("fa=%.6lf\t\t���ȣ�%.6lf\r\n"), lpment.fa, jingdu.lpment.fa);
	strout += tem;
	tem.Format(_T("w=%.6lf\t\t���ȣ�%.6lf\r\n"), lpment.w, jingdu.lpment.w);
	strout += tem;
	tem.Format(_T("k=%.6lf\t\t���ȣ�%.6lf\r\n"), lpment.k, jingdu.lpment.k);
	strout = strout + tem + _T("��Ƭ�ⷽλԪ�ؼ����ȣ�\r\n");
	tem.Format(_T("Xs=%.3lf\t\t���ȣ�%.6lf\r\n"), rpment.Xs, jingdu.rpment.Xs);
	strout += tem;
	tem.Format(_T("Ys=%.3lf\t\t���ȣ�%.6lf\r\n"), rpment.Ys, jingdu.rpment.Ys);
	strout += tem;
	tem.Format(_T("Zs=%.3lf\t\t���ȣ�%.6lf\r\n"), rpment.Zs, jingdu.rpment.Zs);
	strout += tem;
	tem.Format(_T("fa=%.6lf\t\t���ȣ�%.6lf\r\n"), rpment.fa, jingdu.rpment.fa);
	strout += tem;
	tem.Format(_T("w=%.6lf\t\t���ȣ�%.6lf\r\n"), rpment.w, jingdu.rpment.w);
	strout += tem;
	tem.Format(_T("k=%.6lf\t\t���ȣ�%.6lf\r\n"), rpment.k, jingdu.rpment.k);
	strout = strout + tem + _T("�����������£�\r\n�����x\t�����y\t�����x\t�����y\t     X\t\t     Y\t\t     Z\r\n");
	for (int i = 0; i < npknow + npuknow; i++)
	{
		tem.Format(_T("%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t\t%.3lf\t\t%.3lf\r\n"),
			gcps[i].lp.x * 1000, gcps[i].lp.y * 1000, gcps[i].rp.x * 1000, gcps[i].rp.y * 1000, gcps[i].pp.x, gcps[i].pp.y, gcps[i].pp.z);
		strout += tem;
	}
	output = strout;
}

bool dphotocal::savefile()
{
	CFileDialog getfile(FALSE, _T("txt"), _T("dataresult"));
	if (getfile.DoModal() != IDOK)return false;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))return false;
	setlocale(LC_ALL, "chs");//û����佫�޷��������
	CString strout, tem;
	strout = _T("��Ƭ�ⷽλԪ�ؼ����ȣ�\n");
	tem.Format(_T("Xs=%.3lf\t\t���ȣ�%.6lf\n"), lpment.Xs, jingdu.lpment.Xs);
	strout += tem;
	tem.Format(_T("Ys=%.3lf\t\t���ȣ�%.6lf\n"), lpment.Ys, jingdu.lpment.Ys);
	strout += tem;
	tem.Format(_T("Zs=%.3lf\t\t���ȣ�%.6lf\n"), lpment.Zs, jingdu.lpment.Zs);
	strout += tem;
	tem.Format(_T("fa=%.6lf\t\t���ȣ�%.6lf\n"), lpment.fa, jingdu.lpment.fa);
	strout += tem;
	tem.Format(_T("w=%.6lf\t\t���ȣ�%.6lf\n"), lpment.w, jingdu.lpment.w);
	strout += tem;
	tem.Format(_T("k=%.6lf\t\t���ȣ�%.6lf\n"), lpment.k, jingdu.lpment.k);
	strout = strout + tem + _T("��Ƭ�ⷽλԪ�ؼ����ȣ�\n");
	tem.Format(_T("Xs=%.3lf\t\t���ȣ�%.6lf\n"), rpment.Xs, jingdu.rpment.Xs);
	strout += tem;
	tem.Format(_T("Ys=%.3lf\t\t���ȣ�%.6lf\n"), rpment.Ys, jingdu.rpment.Ys);
	strout += tem;
	tem.Format(_T("Zs=%.3lf\t\t���ȣ�%.6lf\n"), rpment.Zs, jingdu.rpment.Zs);
	strout += tem;
	tem.Format(_T("fa=%.6lf\t\t���ȣ�%.6lf\n"), rpment.fa, jingdu.rpment.fa);
	strout += tem;
	tem.Format(_T("w=%.6lf\t\t���ȣ�%.6lf\n"), rpment.w, jingdu.rpment.w);
	strout += tem;
	tem.Format(_T("k=%.6lf\t\t���ȣ�%.6lf\n"), rpment.k, jingdu.rpment.k);
	strout = strout + tem + _T("�����������£�\n�����x\t�����y\t�����x\t�����y\t     X\t\t     Y\t\t     Z\n");
	for (int i = 0; i < npknow + npuknow; i++)
	{
		tem.Format(_T("%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t\t%.3lf\t\t%.3lf\n"), 
			gcps[i].lp.x * 1000, gcps[i].lp.y * 1000, gcps[i].rp.x * 1000, gcps[i].rp.y * 1000, gcps[i].pp.x, gcps[i].pp.y, gcps[i].pp.z);
		strout += tem;
	}
	file.WriteString(strout);
	file.Close();
	return true;
}

void dphotocal::mainfunc(CString input, CString &output)
{
	//readfile();
	readstr(input);
	spaceback();
	spacefront();
	outstr(output);
	//savefile();
}

dphotocal::~dphotocal()
{
	if (gcps)delete[] gcps;
}
