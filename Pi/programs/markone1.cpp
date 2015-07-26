#include "SerialAPI.cpp"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <signal.h>

#define HEIGHT_PIXELS 192
#define WIDTH_PIXELS 256
#define TOLERANCE_PIXELS 20

#define TIMEDELAY 1000000

#define BYTE unsigned char

using namespace cv;
using namespace std;

Mat hsv, binary;
vector<int> hsv_vector(3,0) ;
int clicks;

BYTE FORWARD[8] = {0, 150, 0, 150, 0, 150, 0, 150};
BYTE BACKWARD[8] = {1, 150, 1, 150, 1, 150, 1, 150};
BYTE LEFTWARD[8] = {1, 200, 0, 200, 1, 200, 0, 200};
BYTE RIGHTWARD[8] = {0, 200, 1, 200, 0, 200, 1, 200};
BYTE STOP[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void exit_function(int sig) {
	setMotors(STOP);
	serialClose(fd);
	cout << "serial port closed";
	exit(0);
}

void delay(int del) {
	while(del--);
}

void mouseClickListener(int event, int x, int y, int, void *) {
	if (event == EVENT_LBUTTONDOWN) {
		for (int i=0;i<3;i++) {
			hsv_vector[i]=(int)hsv.at<Vec3b>(y,x)[i];
			cout<<hsv_vector[i]<<" ";
		}
		cout<<endl;
	clicks ++;
	}
}

Point2f getBlobCentroid(Mat& frame, vector<int> hsv_vector) {

	resize(frame, frame, Size(), 0.4, 0.4);
	cout<<frame.rows<<" "<<frame.cols<<endl;
	//convert from rgb to hsv, keep a backup for mousehandler
	cvtColor(frame, hsv, COLOR_BGR2HSV);

	//cout<<"hsv_vector:"<<hsv_vector[0]<<" "<<hsv_vector[1]<<endl;
	//use morphologocal opening and closing on the hsv to get the corresponding binary image
	int threshLo = 15, threshHi = 15;
	inRange(hsv,Scalar(hsv_vector[0]-0.5*threshLo,hsv_vector[1]-threshLo,hsv_vector[2]-threshLo),
		Scalar(hsv_vector[0]+0.5*threshHi,255,255),binary);
	erode(binary,binary,getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
	dilate(binary,binary,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	dilate(binary,binary,getStructuringElement(MORPH_ELLIPSE,Size(5,5)));
	erode(binary,binary,getStructuringElement(MORPH_ELLIPSE, Size(5,5)));

	//use image moments to calculate centroids
	Moments m = moments(binary);
	double dM01 = m.m01;
	double dM10 = m.m10;
	double dArea = m.m00;

	cout<<dArea<<endl;
	int posX, posY;

	if (dArea>100) {
		posX = dM10/dArea;
		posY = dM01/dArea;
	}

	Point2f temp;
	temp.x = posX;
	temp.y = posY;
	return temp;
}

int main(int argc, char **argv) {
	signal(SIGINT, exit_function);
	clicks = 0;
	int height = HEIGHT_PIXELS;
	int width = WIDTH_PIXELS;
	int tolerance = TOLERANCE_PIXELS;
	int leader_x, leader_y;
	if(initializeSerialAPI() < 0)
		return 1;

	VideoCapture cap(0);
	Mat frame;

	while(true) {
		cap>>frame;
		cout<<clicks<<endl;
		namedWindow("rgb",CV_WINDOW_AUTOSIZE);
		setMouseCallback("rgb", mouseClickListener);
		namedWindow("blob",CV_WINDOW_AUTOSIZE);
		Point2f p = getBlobCentroid(frame, hsv_vector);
		imshow("rgb",frame);
		imshow("blob",binary);
		//imshow("hsv", hsv);
		if (waitKey(33)==27) break;
		leader_x = p.x;
		leader_y = p.y;
		if(leader_x > width/2 + tolerance)
			setMotors(RIGHTWARD);
		else if(leader_x < width/2 - tolerance)
			setMotors(LEFTWARD);
		else if(leader_y > height/2 + tolerance)
			setMotors(BACKWARD);
		else if(leader_y < height/2 - tolerance)
			setMotors(FORWARD);
		else
			setMotors(STOP);
		delay(TIMEDELAY);
		setMotors(STOP);
	}
	destroyAllWindows();
	return 0;
}
