// test5.22.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<opencv.hpp>

using namespace cv;
using namespace std;

int calcGaussianBackground(std::vector<cv::Mat> srcMats, cv::Mat & meanMat, cv::Mat &varMat);
int gaussianThreshold(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float weight, cv::Mat & dstMat);
//int main()
//{
//	VideoCapture cap;
//
//	cap.open("test.mp4");
//
//	if (!cap.isOpened()) {
//		cout << "Unable to open video!" << endl;
//		return -1;
//	}
//
//	int cnt = 0;
//	Mat frame;
//	Mat bgMat, subMat, bny_subMat;
//
//	while (1)
//	{
//		cap >> frame;
//		cvtColor(frame, frame, COLOR_BGR2GRAY);
//		if (cnt == 0)
//		{
//			frame.copyTo(bgMat);
//		}
//		else
//		{
//			absdiff(frame, bgMat, subMat);
//			threshold(subMat, bny_subMat, 50, 255, CV_THRESH_BINARY);
//
//			imshow("b_subMat", bny_subMat);
//			imshow("frame", frame);
//			waitKey(30);
//		}
//		cnt++;
//	}
//	return 0;
//}

int main()
{
	VideoCapture cap;

	cap.open("test.mp4");

	if (!cap.isOpened()) {
		cout << "Unable to open video!" << endl;
		return -1;
	}

	int cnt = 0;
	int nBg = 200;
	float wVar = 1;
	Mat frame;
	Mat meanMat;
	Mat varMat;
	Mat dstMat;
	vector<Mat> srcMat;

	while (1)
	{
		cap >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		if (cnt < nBg)
		{
			srcMat.push_back(frame);
		}
		else if(cnt == nBg)
		{
			meanMat.create(frame.size(), CV_8UC1);
			varMat.create(frame.size(), CV_32FC1);
			calcGaussianBackground(srcMat, meanMat, varMat);
		}
		else {
			dstMat.create(frame.size(), CV_8UC1);
			gaussianThreshold(frame, meanMat, varMat, wVar, dstMat);
			imshow("result", dstMat);
			imshow("frame", frame);
			waitKey(30);
		}
		cnt++;
	}
	return 0;
}

int calcGaussianBackground(std::vector<cv::Mat> srcMats, cv::Mat & meanMat, cv::Mat &varMat)
{

	int rows = srcMats[0].rows;
	int cols = srcMats[0].cols;


	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{

			int sum = 0;
			float var = 0;
			//求均值
			for (int i = 0; i < srcMats.size(); i++) {
				sum += srcMats[i].at<uchar>(h, w);
			}
			meanMat.at<uchar>(h, w) = sum / srcMats.size();
			//求方差
			for (int i = 0; i < srcMats.size(); i++) {
				var += pow((srcMats[i].at<uchar>(h, w) - meanMat.at<uchar>(h, w)), 2);
			}
			varMat.at<float>(h, w) = var / srcMats.size();
		}
	}

	return 0;
}

int gaussianThreshold(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float weight, cv::Mat & dstMat)
{
	int srcI;
	int meanI;
	int dstI;
	int rows = srcMat.rows;
	int cols = srcMat.cols;

	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			srcI = srcMat.at<uchar>(h, w);
			meanI = meanMat.at<uchar>(h, w);
			int dif = abs(srcI - meanI);
			int th = weight * varMat.at<float>(h, w);

			if (dif > th) {

				dstMat.at<uchar>(h, w) = 255;
			}
			else {
				dstMat.at<uchar>(h, w) = 0;
			}
		}
	}

	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
