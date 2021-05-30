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
	void Morevec(Mat img, int winsize, double vmin, vector<Point3i>& point);//����Morevec������
	void MatchPointX(Mat img1, Mat img2, Point2i p1, Point2i& p2, int winsize, float pow, int x1, int x2);//������ƥ��
	double CalQxx(Mat img1, Mat img2, Point2i p1, Point2i p2, int winsize);//�������ϵ��
private:
	Mat McalIVcr(Mat img, int winsize, double vmin);//������Ȥֵ��ȥ����ֵ����
	void MdelNmax(Mat& img, int winsize, vector<Point3i>& point);//���ƾֲ������
};

