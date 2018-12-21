#include "stdafx.h"

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>


using namespace cv;

Mat3b canvas;
String appName = "ColorDetection";
Rect redButton;

const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";

int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (redButton.contains(Point(x, y)))
		{
			rectangle(canvas, redButton, Scalar(0, 0, 255), 2);
		}
	}
	if (event == EVENT_LBUTTONUP)
	{
		rectangle(canvas, redButton, Scalar(200, 200, 200), 2);
	}

	imshow(appName, canvas);
	waitKey(1);
}

static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", window_detection_name, high_V);
}
int main(int argc, char* argv[])
{
	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
	namedWindow(appName);
	namedWindow(window_detection_name);
	// Trackbars to set thresholds for HSV values
	createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
	createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
	createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
	createTrackbar("Button", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);
	Mat frame, frame_HSV, frame_threshold;
	while (true) {
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		flip(frame, frame, 1);
		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);
		
		// Your button
		redButton = Rect(0, 0, frame.cols, 50);

		// The canvas
		canvas = Mat3b(frame.rows + redButton.height, frame.cols, Vec3b(0, 0, 0));
		
		// Draw the button
		canvas(redButton) = Vec3b(200, 200, 200);
		putText(canvas(redButton), "vermelho", Point(redButton.width*0.35, redButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));

		// Draw the frames
		frame.copyTo(canvas(Rect(0, redButton.height, frame.cols, frame.rows)));

		// Setup callback function
		namedWindow(appName);
		setMouseCallback(appName, callBackFunc);

		// Show the canvas
		imshow(appName, canvas);
		imshow(window_detection_name, frame_threshold);
		char key = (char)waitKey(30);
		if (key == 'q' || key == 27)
		{
			break;
		}
	}
	return 0;
}