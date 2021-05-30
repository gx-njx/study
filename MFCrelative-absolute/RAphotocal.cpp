#include "pch.h"
#include "RAphotocal.h"
#include "Matrix.h"
#include <locale>

const double pi = 4 * atan(1);

int RAphotocal::SplitStringArray(CString str, char split, CStringArray & aStr)
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

void RAphotocal::readstr(CString input)
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

void RAphotocal::outstr(CString & output)
{
	CString strout, tem;
	strout = _T("��Զ���Ԫ�ؼ����ȣ�\r\n");
	tem.Format(_T("By=%.6lf\t\t���ȣ�%.6lf\r\n"), rement.By, rejust.By);
	strout += tem;
	tem.Format(_T("Bz=%.6lf\t\t���ȣ�%.6lf\r\n"), rement.Bz, rejust.Bz);
	strout += tem;
	tem.Format(_T("fa=%.6lf\t\t���ȣ�%.6lf\r\n"), rement.fa, rejust.fa);
	strout += tem;
	tem.Format(_T("w=%.6lf\t\t���ȣ�%.6lf\r\n"), rement.w, rejust.w);
	strout += tem;
	tem.Format(_T("k=%.6lf\t\t���ȣ�%.6lf\r\n"), rement.k, rejust.k);
	strout = strout + tem + _T("���Զ���Ԫ�ؼ����ȣ�\r\n");
	tem.Format(_T("��=%.6lf\t\t���ȣ�%.6lf\r\n"), abment.rm, abjust.rm);
	strout += tem;
	tem.Format(_T("Xo=%.3lf \t\t���ȣ�%.6lf\r\n"), abment.X0, abjust.X0);
	strout += tem;
	tem.Format(_T("Yo=%.3lf \t\t���ȣ�%.6lf\r\n"), abment.Y0, abjust.Y0);
	strout += tem;
	tem.Format(_T("Zo=%.3lf \t\t���ȣ�%.6lf\r\n"), abment.Z0, abjust.Z0);
	strout += tem;
	tem.Format(_T("��=%.6lf\t\t���ȣ�%.6lf\r\n"), abment.FA, abjust.FA);
	strout += tem;
	tem.Format(_T("W=%.6lf\t\t���ȣ�%.6lf\r\n"), abment.W, abjust.W);
	strout += tem;
	tem.Format(_T("K=%.6lf\t\t���ȣ�%.6lf\r\n"), abment.K, abjust.K);
	strout = strout + tem + _T("�����������£�\r\n�����x\t�����y\t�����x\t�����y\t     X\t\t     Y\t\t     Z\r\n");
	for (int i = 0; i < npknow + npuknow; i++)
	{
		tem.Format(_T("%.3lf\t%.3lf\t%.3lf\t%.3lf\t%.3lf\t\t%.3lf\t\t%.3lf\r\n"),
			gcps[i].lp.x * 1000, gcps[i].lp.y * 1000, gcps[i].rp.x * 1000, gcps[i].rp.y * 1000, gcps[i].pp.x, gcps[i].pp.y, gcps[i].pp.z);
		strout += tem;
	}
	output = strout;
}

double RAphotocal::dist(point2 a, point2 b)
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx*dx + dy * dy);
}

double RAphotocal::dist(point3 a, point3 b)
{
	double dx = a.x - b.x;
	double dy = a.y - b.y;
	return sqrt(dx*dx + dy * dy);
}

void RAphotocal::relative()
{
	//��������Զ���
	CMatrix dx;//��Զ���Ԫ�ظ���������
	CMatrix A(npknow + npuknow, 5);//ϵ������
	CMatrix L(npknow + npuknow, 1);//��������
	CMatrix R(3, 3);//��ת����
	CMatrix LP(3, 1);//�����ռ�����
	CMatrix RP(3, 1);//�����ռ�����
	double epsa = pi / 180.0*1.0 / 3600;//�Ƕ��޲�
	double epsb = 0.01;//�����޲�
	rement.By = rement.Bz = rement.fa = rement.w = rement.k = 0;//��ʼ��
	double Bx = gcps[0].lp.x - gcps[0].rp.x;//ȷ��Bx
	double By, Bz, N1, N2, Q;
	do
	{
		//������ת����R
		R(0, 0) = cos(rement.fa)*cos(rement.k) - sin(rement.fa)*sin(rement.w)*sin(rement.k);
		R(0, 1) = -cos(rement.fa)*sin(rement.k) - sin(rement.fa)*sin(rement.w)*cos(rement.k);
		R(0, 2) = -sin(rement.fa)*cos(rement.w);
		R(1, 0) = cos(rement.w)*sin(rement.k);
		R(1, 1) = cos(rement.w)*cos(rement.k);
		R(1, 2) = -sin(rement.w);
		R(2, 0) = sin(rement.fa)*cos(rement.k) + cos(rement.fa)*sin(rement.w)*sin(rement.k);
		R(2, 1) = -sin(rement.fa)*sin(rement.k) + cos(rement.fa)*sin(rement.w)*cos(rement.k);
		R(2, 2) = cos(rement.fa)*cos(rement.w);
		//����By,Bz��ѭ����rement��Ϊuv��
		By = Bx * rement.By;
		Bz = Bx * rement.Bz;
		//��ÿ��������ϵ���ͳ�����
		for (int i = 0; i < npknow + npuknow; i++)
		{
			//ȷ��X1,Y1,Z1
			LP(0, 0) = gcps[i].lp.x;
			LP(1, 0) = gcps[i].lp.y;
			LP(2, 0) = -inment.f;
			//ȷ��X2,Y2,Z2
			RP(0, 0) = gcps[i].rp.x;
			RP(1, 0) = gcps[i].rp.y;
			RP(2, 0) = -inment.f;
			RP = R * RP;
			//����N1,N1,Q
			N1 = (Bx*RP(2, 0) - Bz * RP(0, 0)) / (LP(0, 0)*RP(2, 0) - RP(0, 0)*LP(2, 0));
			N2 = (Bx*LP(2, 0) - Bz * LP(0, 0)) / (LP(0, 0)*RP(2, 0) - RP(0, 0)*LP(2, 0));
			Q = N1 * LP(1, 0) - N2 * RP(1, 0) - By;
			//����ϵ������A�ͳ�����L
			A(i, 0) = Bx;
			A(i, 1) = -RP(1, 0) / RP(2, 0)*Bx;
			A(i, 2) = -RP(0, 0)*RP(1, 0) / RP(2, 0)*N2;
			A(i, 3) = -(RP(2, 0) + RP(1, 0)*RP(1, 0) / RP(2, 0))*N2;
			A(i, 4) = RP(0, 0)*N2;
			L(i, 0) = Q;
		}
		//���ƽ�����
		dx = (~A*A).Inv()*~A*L;
		//��Զ���Ԫ�ظ���
		rement.By += dx(0, 0);
		rement.Bz += dx(1, 0);
		rement.fa += dx(2, 0);
		rement.w += dx(3, 0);
		rement.k += dx(4, 0);
	} while (fabs(dx(0, 0)) > epsb || fabs(dx(1, 0)) > epsb || fabs(dx(2, 0)) > epsa || fabs(dx(3, 0)) > epsa || fabs(dx(4, 0)) > epsa);
	//uvת��
	rement.By = rement.By*Bx;
	rement.Bz = rement.Bz*Bx;
	//��������
	CMatrix V;
	V = A * dx - L;
	double the = sqrt((~V*V)(0, 0) / (npknow + npuknow - 5));//�����
	CMatrix Qxx;
	Qxx = (~A*A).Inv();//Э��������
	rejust.By = the * sqrt(Qxx(0, 0))*fabs(Bx);
	rejust.Bz = the * sqrt(Qxx(1, 1))*fabs(Bx);
	rejust.fa = the * sqrt(Qxx(2, 2));
	rejust.w = the * sqrt(Qxx(3, 3));
	rejust.k = the * sqrt(Qxx(4, 4));
	//////////////////////////////////////////////////////////////////////////
	//ģ�͵����
	//////////////////////////////////////////////////////////////////////////
	double m;//������
	m = 10000;
	//m = 2 * dist(gcps[0].pp, gcps[1].pp) / (dist(gcps[0].lp, gcps[1].lp) + dist(gcps[0].rp, gcps[1].rp));
	//������ת����R
	R(0, 0) = cos(rement.fa)*cos(rement.k) - sin(rement.fa)*sin(rement.w)*sin(rement.k);
	R(0, 1) = -cos(rement.fa)*sin(rement.k) - sin(rement.fa)*sin(rement.w)*cos(rement.k);
	R(0, 2) = -sin(rement.fa)*cos(rement.w);
	R(1, 0) = cos(rement.w)*sin(rement.k);
	R(1, 1) = cos(rement.w)*cos(rement.k);
	R(1, 2) = -sin(rement.w);
	R(2, 0) = sin(rement.fa)*cos(rement.k) + cos(rement.fa)*sin(rement.w)*sin(rement.k);
	R(2, 1) = -sin(rement.fa)*sin(rement.k) + cos(rement.fa)*sin(rement.w)*cos(rement.k);
	R(2, 2) = cos(rement.fa)*cos(rement.w);
	//��ֵBx,By,Bz
	Bx = gcps[0].lp.x - gcps[0].rp.x;
	By = rement.By;
	Bz = rement.Bz;
	//��ÿ��������ģ�͵�
	for (int i = 0; i < npknow + npuknow; i++)
	{
		//ȷ��X1,Y1,Z1
		LP(0, 0) = gcps[i].lp.x;
		LP(1, 0) = gcps[i].lp.y;
		LP(2, 0) = -inment.f;
		//ȷ��X2,Y2,Z2
		RP(0, 0) = gcps[i].rp.x;
		RP(1, 0) = gcps[i].rp.y;
		RP(2, 0) = -inment.f;
		RP = R * RP;
		//����N1,N1
		N1 = (Bx*RP(2, 0) - Bz * RP(0, 0)) / (LP(0, 0)*RP(2, 0) - RP(0, 0)*LP(2, 0));
		N2 = (Bx*LP(2, 0) - Bz * LP(0, 0)) / (LP(0, 0)*RP(2, 0) - RP(0, 0)*LP(2, 0));
		//����ģ�͵�����
		gcps[i].fp.x = m * N1*LP(0, 0);
		gcps[i].fp.y = 0.5*m*(N1*LP(1, 0) + N2 * RP(1, 0) + By);
		gcps[i].fp.z = m * inment.f + m * N1*LP(2, 0);
	}
	return;
}

void RAphotocal::absolute()
{
	CMatrix dx;//���Զ���Ԫ�ظ�����
	CMatrix A(3 * npknow, 7);//ϵ������
	CMatrix L(3 * npknow, 1);//���������
	CMatrix R(3, 3);//��ת����
	double epsa = pi / 180.0*1.0 / 3600;//�Ƕ��޲�
	double epsb = 0.01;//�����޲�
	abment.FA = abment.K = abment.W = abment.X0 = abment.Y0 = abment.Z0 = 0;
	abment.rm = 1;
	//���Ļ��������
	double _Xtpg = 0, _Ytpg = 0, _Ztpg = 0;//���Ƶ�����
	for (int i = 0; i < npknow; i++)
	{
		_Xtpg += gcps[i].pp.x;
		_Ytpg += gcps[i].pp.y;
		_Ztpg += gcps[i].pp.z;
	}
	_Xtpg = _Xtpg / npknow;//���Ƶ�����λ������
	_Ytpg = _Ytpg / npknow;
	_Ztpg = _Ztpg / npknow;
	for (int i = 0; i < npknow; i++)//���Ƶ����Ļ�
	{
		gcps[i].pp.x -= _Xtpg;
		gcps[i].pp.y -= _Ytpg;
		gcps[i].pp.z -= _Ztpg;
	}
	double _Xpg = 0, _Ypg = 0, _Zpg = 0;//ģ�͵�����
	for (int i = 0; i < npknow + npuknow; i++)
	{
		_Xpg += gcps[i].fp.x;
		_Ypg += gcps[i].fp.y;
		_Zpg += gcps[i].fp.z;
	}
	_Xpg = _Xpg / (npknow + npuknow);//ģ�͵�����λ������
	_Ypg = _Ypg / (npknow + npuknow);
	_Zpg = _Zpg / (npknow + npuknow);
	for (int i = 0; i < npknow + npuknow; i++)//ģ�͵����Ļ�
	{
		gcps[i].fp.x -= _Xpg;
		gcps[i].fp.y -= _Ypg;
		gcps[i].fp.z -= _Zpg;
	}
	do
	{
		for (int i = 0; i < npknow; i++)
		{
			//������ת����R
			R(0, 0) = cos(abment.FA)*cos(abment.K) - sin(abment.FA)*sin(abment.W)*sin(abment.K);
			R(0, 1) = -cos(abment.FA)*sin(abment.K) - sin(abment.FA)*sin(abment.W)*cos(abment.K);
			R(0, 2) = -sin(abment.FA)*cos(abment.W);
			R(1, 0) = cos(abment.W)*sin(abment.K);
			R(1, 1) = cos(abment.W)*cos(abment.K);
			R(1, 2) = -sin(abment.W);
			R(2, 0) = sin(abment.FA)*cos(abment.K) + cos(abment.FA)*sin(abment.W)*sin(abment.K);
			R(2, 1) = -sin(abment.FA)*sin(abment.K) + cos(abment.FA)*sin(abment.W)*cos(abment.K);
			R(2, 2) = cos(abment.FA)*cos(abment.W);
			CMatrix temp(3, 1);//Xp,Yp,Zp
			temp(0, 0) = gcps[i].fp.x;
			temp(1, 0) = gcps[i].fp.y;
			temp(2, 0) = gcps[i].fp.z;
			CMatrix temfp(3, 1);//X',Y',Z'
			temfp = R * temp;
			//ϵ������A��ֵ
			//��һ��
			A(3 * i, 0) = 1;
			A(3 * i, 1) = 0;
			A(3 * i, 2) = 0;
			A(3 * i, 3) = temfp(0, 0);
			A(3 * i, 4) = -temfp(2, 0);
			A(3 * i, 5) = 0;
			A(3 * i, 6) = -temfp(1, 0);
			//�ڶ���
			A(3 * i + 1, 0) = 0;
			A(3 * i + 1, 1) = 1;
			A(3 * i + 1, 2) = 0;
			A(3 * i + 1, 3) = temfp(1, 0);
			A(3 * i + 1, 4) = 0;
			A(3 * i + 1, 5) = -temfp(2, 0);
			A(3 * i + 1, 6) = temfp(0, 0);
			//������
			A(3 * i + 2, 0) = 0;
			A(3 * i + 2, 1) = 0;
			A(3 * i + 2, 2) = 1;
			A(3 * i + 2, 3) = temfp(2, 0);
			A(3 * i + 2, 4) = temfp(0, 0);
			A(3 * i + 2, 5) = temfp(1, 0);
			A(3 * i + 2, 6) = 0;
			//���㳣�������L
			CMatrix temtp(3, 1);//Xtp,Ytp,Ztp
			temtp(0, 0) = gcps[i].pp.x;
			temtp(1, 0) = gcps[i].pp.y;
			temtp(2, 0) = gcps[i].pp.z;
			CMatrix tem0(3, 1);//X0,Y0,Z0
			tem0(0, 0) = abment.X0;
			tem0(1, 0) = abment.Y0;
			tem0(2, 0) = abment.Z0;
			CMatrix teml(3, 1);//Lx,Ly,Lz
			teml = temtp - abment.rm*R*temp - tem0;
			L(3 * i, 0) = teml(0, 0);
			L(3 * i + 1, 0) = teml(1, 0);
			L(3 * i + 2, 0) = teml(2, 0);
		}
		//���ƽ�����
		dx = (~A*A).Inv()*~A*L;
		//���Զ���Ԫ�ظ���
		abment.X0 += dx(0, 0);
		abment.Y0 += dx(1, 0);
		abment.Z0 += dx(2, 0);
		abment.rm += dx(3, 0);
		abment.FA += dx(4, 0);
		abment.W += dx(5, 0);
		abment.K += dx(6, 0);
	} while (fabs(dx(0, 0)) > epsb || fabs(dx(1, 0)) > epsb || fabs(dx(2, 0)) > epsb || fabs(dx(4, 0)) > epsa || fabs(dx(5, 0)) > epsa || fabs(dx(5, 0)) > epsa);
	//��������
	CMatrix V;
	V = A * dx - L;
	double the = sqrt((~V*V)(0, 0) / (3 * npknow - 7));//�����
	CMatrix Qxx;
	Qxx = (~A*A).Inv();//Э��������
	abjust.X0 = the * sqrt(Qxx(0, 0));
	abjust.Y0 = the * sqrt(Qxx(1, 1));
	abjust.Z0 = the * sqrt(Qxx(2, 2));
	abjust.rm = the * sqrt(Qxx(3, 3));
	abjust.FA = the * sqrt(Qxx(4, 4));
	abjust.W = the * sqrt(Qxx(5, 5));
	abjust.K = the * sqrt(Qxx(6, 6));
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//������������
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//������ת����R
	R(0, 0) = cos(abment.FA)*cos(abment.K) - sin(abment.FA)*sin(abment.W)*sin(abment.K);
	R(0, 1) = -cos(abment.FA)*sin(abment.K) - sin(abment.FA)*sin(abment.W)*cos(abment.K);
	R(0, 2) = -sin(abment.FA)*cos(abment.W);
	R(1, 0) = cos(abment.W)*sin(abment.K);
	R(1, 1) = cos(abment.W)*cos(abment.K);
	R(1, 2) = -sin(abment.W);
	R(2, 0) = sin(abment.FA)*cos(abment.K) + cos(abment.FA)*sin(abment.W)*sin(abment.K);
	R(2, 1) = -sin(abment.FA)*sin(abment.K) + cos(abment.FA)*sin(abment.W)*cos(abment.K);
	R(2, 2) = cos(abment.FA)*cos(abment.W);
	//������������
	for (int i = 0; i < npknow; i++)//��ԭ���Ļ�����
	{
		gcps[i].pp.x += _Xtpg;
		gcps[i].pp.y += _Ytpg;
		gcps[i].pp.z += _Ztpg;
	}
	for (int i = npknow; i < npknow + npuknow; i++)//����δ֪������
	{
		CMatrix temtp(3, 1);//Xtp,Ytp,Ztp
		CMatrix temp(3, 1);//Xp,Yp,Zp
		temp(0, 0) = gcps[i].fp.x;
		temp(1, 0) = gcps[i].fp.y;
		temp(2, 0) = gcps[i].fp.z;
		CMatrix tem0(3, 1);//X0,Y0,Z0
		tem0(0, 0) = abment.X0;
		tem0(1, 0) = abment.Y0;
		tem0(2, 0) = abment.Z0;
		//��������
		temtp = abment.rm*R*temp + tem0;
		gcps[i].pp.x = temtp(0, 0) + _Xtpg;
		gcps[i].pp.y = temtp(1, 0) + _Ytpg;
		gcps[i].pp.z = temtp(2, 0) + _Ztpg;
	}
}

void RAphotocal::mainfunc(CString input, CString & output)
{
	readstr(input);
	relative();
	absolute();
	outstr(output);
}

bool RAphotocal::readfile(CString & input)
{
	CFileDialog getfile(TRUE, _T("txt"), _T("data"));
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

bool RAphotocal::savefile()
{
	CFileDialog getfile(FALSE, _T("txt"), _T("dataresult"));
	if (getfile.DoModal() != IDOK)return false;
	CString pathname = getfile.GetPathName();
	CStdioFile file;
	if (!file.Open(pathname, CFile::modeCreate | CFile::modeWrite))return false;
	setlocale(LC_ALL, "chs");//û����佫�޷��������
	CString strout;
	outstr(strout);
	strout.Remove('\r');
	file.WriteString(strout);
	file.Close();
	return true;
}

RAphotocal::~RAphotocal()
{
	if (gcps)delete[] gcps;
}
