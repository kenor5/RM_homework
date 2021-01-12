//
// Created by kenor on 2021/1/12.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void OnMouseAction(int event, int x, int y, int flags, void *ustc);
void doThreshold(Mat &src);

int num = 0;
vector<Point2f> point;  // 鼠标点击的四个角点
vector<Point2f> target_point; //
const Point2f target_size = Point2f(200, 60);
Mat image = imread("/home/kenor/CLionProjects/untitled/wps14.jpg", 1);
Mat transformation_mat;
Mat output_image;

int main() {

    target_point.emplace_back(0, 0);
    target_point.emplace_back(target_size.x, 0);
    target_point.emplace_back(0, target_size.y);
    target_point.emplace_back(target_size.x, target_size.y);

    imshow("image", image);
    setMouseCallback("image", OnMouseAction);


    waitKey(0);

    return 0;
}

void OnMouseAction(int event, int x, int y, int flags, void *ustc) {

        if (event == EVENT_LBUTTONUP) {
            num++;
            Point2f temp;
            temp.x = x;
            temp.y = y;
            point.push_back(temp);

//            circle(image, temp, 5, Scalar(255, 255));
            imshow("image", image);


        }
//        如果用鼠标点了四个点，就做透视变换
    if(num == 4) {
        transformation_mat = getPerspectiveTransform(point, target_point);
        warpPerspective(image,
                        output_image,
                        transformation_mat,
                        Size(target_size.x, target_size.y)
        );
        imshow("00", output_image);
        imwrite("/home/kenor/CLionProjects/untitled/car_dst1.jpg", output_image);
        doThreshold(output_image);
    }
}

void doThreshold(Mat &src) {

    if(src.empty())return;

    inRange(src, Scalar(70, 0, 0), Scalar(200, 255, 90),  src);
    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
//    erode(src, src, element);
    element = getStructuringElement(MORPH_RECT, Size(2, 2));
    dilate(src, src, element);
    imwrite("/home/kenor/CLionProjects/untitled/car_dst.jpg", src);
    imshow("src", src);
}
