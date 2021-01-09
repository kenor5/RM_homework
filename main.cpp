#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

int main() {
    Mat src = imread("/home/kenor/CLionProjects/untitled/apple.jpg", 1);
    if (src.empty()) {
        printf("cannot load image...\n");
        return -1;
    }


    Mat dst;
//    转换色彩空间
    cvtColor(src, dst, COLOR_BGR2HSV);
//    imshow("apple_output", dst);

    vector<Mat> split_hsv;
//    分割色彩空间
    split(dst, split_hsv);
//    imshow("split_hsv_h", split_hsv[0]);

    Mat dstTempImage1, dstTempImage2;

    inRange(split_hsv[0], 1, 12, dstTempImage1);
//    imshow("apple_output1", dstTempImage1);
    inRange(split_hsv[0], 150, 255, dstTempImage2);
//    imshow("apple_output2", dstTempImage2);
//    两个界限合起来
    bitwise_or(dstTempImage1, dstTempImage2, dst);
//    imshow("final_output", dst);
//    腐蚀膨胀
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(dst, dst, element);
    erode(dst, dst, element);
    erode(dst, dst, element);
    erode(dst, dst, element);
    erode(dst, dst, element);
    dilate(dst, dst, element);
    dilate(dst, dst, element);
    dilate(dst, dst, element);
    dilate(dst, dst, element);
    dilate(dst, dst, element);

    Mat img1, img2;
    src.copyTo(img1);  //深拷贝用来绘制最大外接矩形
    src.copyTo(img2);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(dst, contours, hierarchy, RETR_TREE,CHAIN_APPROX_SIMPLE,Point());

    //寻找轮廓的外接矩形
    for (int n = 0; n < contours.size(); n++)
    {
        // 最大外接矩形
        Rect rect = boundingRect(contours[n]);
        if(rect.area()>3800)
            rectangle(img1, rect, Scalar(0, 0, 255), 2, 8, 0);
        else;
        // 最小外接矩形
        RotatedRect rrect = minAreaRect(contours[n]);
        Point2f points[4];
        rrect.points(points);  //读取最小外接矩形的四个顶点
        Point2f cpt = rrect.center;  //最小外接矩形的中心

        // 绘制旋转矩形与中心位置

        for (int i = 0; i < 4; i++)
        {
            if (i == 3)
            {
                line(img2, points[i], points[0], Scalar(0, 255, 0), 2, 8, 0);
                break;
            }
            line(img2, points[i], points[i + 1], Scalar(0, 255, 0), 2, 8, 0);
        }
        //绘制矩形的中心
        circle(src, cpt, 2, Scalar(255, 0, 0), 2, 8, 0);
    }
    imshow("max", img1);
//    imshow("min", img2);
//    imwrite("/home/kenor/CLionProjects/untitled/out_put.jpg", img1);
    waitKey(0);
    return 0;
}