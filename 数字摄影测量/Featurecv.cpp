#include "pch.h"
#include "Featurecv.h"

void Featurecv::Morevec(Mat img, int winsize, double vmin, vector<Point3i>& point)
{
	Mat ivcr = McalIVcr(img, winsize, vmin);//计算兴趣值矩阵
	MdelNmax(ivcr, winsize, point);//抑制局部非最大
}

void Featurecv::MatchPointX(Mat img1, Mat img2, Point2i p1, Point2i& p2, int winsize, float pow, int x1, int x2)
{
	int x = 0;//暂存同名点x
	double tempow = 0;//暂存待定同名点相关系数
	if (x1 > x2) {//范围由小到大
		int tem = x1;
		x1 = x2;
		x2 = tem;
	}
	for (int i = p1.y + x1; i < p1.y + x2; i++) {
		if (i < (winsize - 1) / 2 || i > img2.rows - (winsize - 1) / 2) {//超出图片边界
			break;
		}
		double powcal = CalQxx(img1, img2, p1, Point2i(p1.x, i), winsize);//计算相关系数
		if (powcal > pow && powcal > tempow) {//符合相关系数阈值，比待定同名点相关系数大
			x = i;
			tempow = powcal;
		}
	}
	p2 = Point2i(p1.x, x);//opencv中xy是相反的
}

double Featurecv::CalQxx(Mat img1, Mat img2, Point2i p1, Point2i p2, int winsize)
{
	double avg1 = 0, avg2 = 0;//灰度平均值
	for (int i = -(winsize - 1) / 2 ; i <  (winsize + 1) / 2; i++) {
		for (int j = -(winsize - 1) / 2 ; j <  (winsize + 1) / 2; j++) {
			avg1 += img1.ptr<uchar>(p1.x + i)[p1.y + j];
			avg2 += img2.ptr<uchar>(p2.x + i)[p2.y + j];
		}
	}
	avg1 = avg1 / winsize / winsize;
	avg2 = avg2 / winsize / winsize;
	double sum1 = 0, sum2 = 0, sum3 = 0;//公式中的3个求和
	for (int i = -(winsize - 1) / 2; i < (winsize + 1) / 2; i++) {
		for (int j = -(winsize - 1) / 2; j < (winsize + 1) / 2; j++) {
			sum1 += (img1.ptr<uchar>(p1.x + i)[p1.y + j] - avg1) * (img2.ptr<uchar>(p2.x + i)[p2.y + j] - avg2);
			sum2 += pow((img1.ptr<uchar>(p1.x + i)[p1.y + j] - avg1), 2);//pow(x,2)=x^2
			sum3 += pow((img2.ptr<uchar>(p2.x + i)[p2.y + j] - avg2), 2);
		}
	}
	return sum1 / (sqrt(sum2 * sum3));//套公式计算
}

Mat Featurecv::McalIVcr(Mat img, int winsize, double vmin)
{
	Mat outimg;
	outimg.create(img.rows, img.cols, CV_32FC1);
	outimg.zeros(img.rows, img.cols, CV_32FC1);
	int v[4] = { 0,0,0,0 };//暂存各个方向兴趣值
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
			float s = min(min(v[0], v[1]), min(v[2], v[3]));//取各个方向最小值
			if (s > vmin) {
				outimg.at<float>(i, j) = s;//兴趣值
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
				float center = img.at<float>(i, j);//中心点兴趣值
				bool flag = 1;
				for (int m = -(winsize - 1) / 2; m < (winsize - 1) / 2; m++) {
					for (int n = -(winsize - 1) / 2; n < (winsize - 1) / 2; n++) {//对窗口范围内
						float temp = img.at<float>(m + i, n + j);
						if (temp > center) {//中心的不是最大兴趣值
							flag = 0;
							break;
						}
					}
				}
				if (flag) {//中心点是最大兴趣值
					Point3i p(i, j, center);//第三维储存兴趣值
					point.push_back(p);
				}
			}

		}
	}
}
