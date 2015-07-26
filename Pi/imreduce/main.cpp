#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>


using namespace cv;

int main() {
	Mat img = imread("ocr_pi.png");
	resize(img,img,Size(),0.5,0.5);
	imwrite("output.jpg",img);
	return 0;
}