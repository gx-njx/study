#include "pch.h"
#include "Featurecv.h"

void Featurecv::Morevec(Mat img, int winsize, double vmin, vector<Point3i>& point)
{
	Mat ivcr = McalIVcr(img, winsize, vmin);//������Ȥֵ����
	MdelNmax(ivcr, winsize, point);//���ƾֲ������
}

void Featurecv::MatchPointX(Mat img1, Mat img2, Point2i p1, Point2i& p2, int winsize, float pow, int x1, int x2)
{
	int x = 0;//�ݴ�ͬ����x
	double tempow = 0;//�ݴ����ͬ�������ϵ��
	if (x1 > x2) {//��Χ��С����
		int tem = x1;
		x1 = x2;
		x2 = tem;
	}
	for (int i = p1.y + x1; i < p1.y + x2; i++) {
		if (i < (winsize - 1) / 2 || i > img2.rows - (winsize - 1) / 2) {//����ͼƬ�߽�
			break;
		}
		double powcal = CalQxx(img1, img2, p1, Point2i(p1.x, i), winsize);//�������ϵ��
		if (powcal > pow && powcal > tempow) {//�������ϵ����ֵ���ȴ���ͬ�������ϵ����
			x = i;
			tempow = powcal;
		}
	}
	p2 = Point2i(p1.x, x);//opencv��xy���෴��
}

double Featurecv::CalQxx(Mat img1, Mat img2, Point2i p1, Point2i p2, int winsize)
{
	double avg1 = 0, avg2 = 0;//�Ҷ�ƽ��ֵ
	for (int i = -(winsize - 1) / 2 ; i <  (winsize + 1) / 2; i++) {
		for (int j = -(winsize - 1) / 2 ; j <  (winsize + 1) / 2; j++) {
			avg1 += img1.ptr<uchar>(p1.x + i)[p1.y + j];
			avg2 += img2.ptr<uchar>(p2.x + i)[p2.y + j];
		}
	}
	avg1 = avg1 / winsize / winsize;
	avg2 = avg2 / winsize / winsize;
	double sum1 = 0, sum2 = 0, sum3 = 0;//��ʽ�е�3�����
	for (int i = -(winsize - 1) / 2; i < (winsize + 1) / 2; i++) {
		for (int j = -(winsize - 1) / 2; j < (winsize + 1) / 2; j++) {
			sum1 += (img1.ptr<uchar>(p1.x + i)[p1.y + j] - avg1) * (img2.ptr<uchar>(p2.x + i)[p2.y + j] - avg2);
			sum2 += pow((img1.ptr<uchar>(p1.x + i)[p1.y + j] - avg1), 2);//pow(x,2)=x^2
			sum3 += pow((img2.ptr<uchar>(p2.x + i)[p2.y + j] - avg2), 2);
		}
	}
	return sum1 / (sqrt(sum2 * sum3));//�׹�ʽ����
}

Mat Featurecv::McalIVcr(Mat img, int winsize, double vmin)
{
	Mat outimg;
	outimg.create(img.rows, img.cols, CV_32FC1);
	outimg.zeros(img.rows, img.cols, CV_32FC1);
	int v[4] = { 0,0,0,0 };//�ݴ����������Ȥֵ
	for (int i = (winsize - 1) / 2; i < img.rows - (winsize - 1) / 2; i++) {
		for (int j = (winsize - 1) / 2; j < img.cols - (winsize - 1) / 2; j++) {
			v[0] = v[1] = v[2] = v[3] = 0;
			for (int m = 0; m < winsize - 1; m++) {
				int a = int(img.at<uchar>(i - (winsize - 1) / 2 + m, j)) - int(img.at<uchar>(i - (winsize - 1) / 2 + m + 1, j));
				a = a * a;
				v[0] += a;
				int b = int(img.at<uchar>(i, j - (winsize - 1) / 2 + m)) - int(img.at<uchar>(i, j - (winsize - 1) / 2 + m + 1));
				b = b * b;
				v[1] += b; 
				int c = int(img.at<uchar>(i + (winsize - 1) / 2 - m, j + (winsize - 1) / 2 - m)) -
					int(img.at<uchar>(i + (winsize - 1) / 2 - m - 1, j + (winsize - 1) / 2 - m - 1));
				c = c * c;
				v[2] += c;
				int d = int(img.at<uchar>(i - (winsize - 1) / 2 + m, j + (winsize - 1) / 2 - m)) -
					int(img.at<uchar>(i - (winsize - 1) / 2 + m + 1, j + (winsize - 1) / 2 - m - 1));
				d = d * d;
				v[3] += d;
			}
			float s = min(min(v[0], v[1]), min(v[2], v[3]));//ȡ����������Сֵ
			if (s > vmin) {
				outimg.at<float>(i, j) = s;//��Ȥֵ
			}
		}
	}
	return outimg;
}

void Featurecv::MdelNmax(Mat& img, int winsize, vector<Point3i>& point)
{
	point.clear();
	int row = img.rows;
	int col = img.cols;
	for (int i = (winsize - 1) / 2; i < row - (winsize - 1) / 2; i++) {
		for (int j = (winsize - 1) / 2; j < col - (winsize - 1) / 2; j++) {
			if (img.at<float>(i, j) > 0) {
				float center = img.at<float>(i, j);//���ĵ���Ȥֵ
				bool flag = 1;
				for (int m = -(winsize - 1) / 2; m < (winsize - 1) / 2; m++) {
					for (int n = -(winsize - 1) / 2; n < (winsize - 1) / 2; n++) {//�Դ��ڷ�Χ��
						float temp = img.at<float>(m + i, n + j);
						if (temp > center) {//���ĵĲ��������Ȥֵ
							flag = 0;
							break;
						}
					}
				}
				if (flag) {//���ĵ��������Ȥֵ
					Point3i p(i, j, center);//����ά������Ȥֵ
					point.push_back(p);
				}
			}

		}
	}
}
