#pragma once
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Featurecv
{
public:
	void Morevec(Mat img, int winsize, double vmin, vector<Point3i>& point);//计算Morevec特征点
	void MatchPointX(Mat img1, Mat img2, Point2i p1, Point2i& p2, int winsize, float pow, int x1, int x2);//特征点匹配
	double CalQxx(Mat img1, Mat img2, Point2i p1, Point2i p2, int winsize);//计算相关系数
private:
	Mat McalIVcr(Mat img, int winsize, double vmin);//计算兴趣值，去除阈值以下
	void MdelNmax(Mat& img, int winsize, vector<Point3i>& point);//抑制局部非最大
};

