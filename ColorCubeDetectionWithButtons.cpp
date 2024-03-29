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
Rect orangeButton;
Rect yellowButton;
Rect whiteButton;
Rect greenButton;
Rect blueButton;

const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";

String color = "";

int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;

void cleanAllButtonsSelections() {
	rectangle(canvas, redButton, Scalar(0, 0, 255), 2);
	rectangle(canvas, orangeButton, Scalar(0, 140, 255), 2);
	rectangle(canvas, yellowButton, Scalar(0, 255, 255), 2);
	rectangle(canvas, whiteButton, Scalar(255, 255, 255), 2);
	rectangle(canvas, greenButton, Scalar(0, 255, 0), 2);
	rectangle(canvas, blueButton, Scalar(255, 0, 0), 2);
}

void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (redButton.contains(Point(x, y)))
		{
			cleanAllButtonsSelections();
			rectangle(canvas, redButton, Scalar(0, 0, 0), 2);
			low_H = 160, high_H = 180;
			low_S = 120, high_S = 255;
			low_V = 90, high_V = 255;
			color = "red";
		}
		if (orangeButton.contains(Point(x, y)))
		{
			cleanAllButtonsSelections();
			rectangle(canvas, orangeButton, Scalar(0, 0, 0), 2);
			low_H = 0, high_H = 20;
			low_S = 120, high_S = 255;
			low_V = 160, high_V = 255;
			color = "orange";
		}
		if (yellowButton.contains(Point(x, y)))
		{
			cleanAllButtonsSelections();
			rectangle(canvas, yellowButton, Scalar(0, 0, 0), 2);
			low_H = 20, high_H = 40;
			low_S = 40, high_S = 255;
			low_V = 170, high_V = 255;
			color = "yellow";
		}
		if (whiteButton.contains(Point(x, y)))
		{
			cleanAllButtonsSelections();
			rectangle(canvas, whiteButton, Scalar(0, 0, 0), 2);
			low_H = 80, high_H = 110;
			low_S = 0, high_S = 255;
			low_V = 220, high_V = 255;
			color = "white";
		}
		if (greenButton.contains(Point(x, y)))
		{
			cleanAllButtonsSelections();
			rectangle(canvas, greenButton, Scalar(0, 0, 0), 2);
			low_H = 60, high_H = 90;
			low_S = 110, high_S = 255;
			low_V = 100, high_V = 255;
			color = "green";
		}
		if (blueButton.contains(Point(x, y)))
		{
			cleanAllButtonsSelections();
			rectangle(canvas, blueButton, Scalar(0, 0, 0), 2);
			low_H = 100, high_H = 140;
			low_S = 140, high_S = 255;
			low_V = 160, high_V = 255;
			color = "blue";
		}

	}
	/*if (event == EVENT_LBUTTONUP)
	{
		rectangle(canvas, redButton, Scalar(0, 0, 255), 2);
	}*/

	imshow(appName, canvas);
	waitKey(1);
}

int main(int argc, char* argv[])
{
	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
	namedWindow(appName);
	namedWindow(window_detection_name);

	Mat first_frame, frame, frame_HSV, frame_threshold;

	//Primeiro frame
	cap >> first_frame;
	flip(first_frame, first_frame, 1);
	// Your buttons
	redButton	 = Rect(0* first_frame.cols/6, 0, first_frame.cols/6, 50);
	orangeButton = Rect(1* first_frame.cols/6, 0, first_frame.cols/6, 50);
	yellowButton = Rect(2* first_frame.cols/6, 0, first_frame.cols/6, 50);
	whiteButton  = Rect(3* first_frame.cols/6, 0, first_frame.cols/6, 50);
	greenButton  = Rect(4* first_frame.cols/6, 0, first_frame.cols/6, 50);
	blueButton	 = Rect(5* first_frame.cols/6, 0, first_frame.cols/6, 50);

	// The canvas
	canvas = Mat3b(first_frame.rows + redButton.height, first_frame.cols, Vec3b(0, 0, 0));

	// Draw the red button
	canvas(redButton) = Vec3b(0, 0, 255);
	putText(canvas(redButton), "Vermelho", Point(redButton.width*0.15, redButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	// Draw the orange button
	canvas(orangeButton) = Vec3b(0, 140, 255);
	putText(canvas(orangeButton), "Laranja", Point(orangeButton.width*0.15, orangeButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	// Draw the yellow button
	canvas(yellowButton) = Vec3b(0, 255, 255);
	putText(canvas(yellowButton), "Amarelo", Point(yellowButton.width*0.15, yellowButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	// Draw the white button
	canvas(whiteButton) = Vec3b(255, 255, 255);
	putText(canvas(whiteButton), "Branco", Point(whiteButton.width*0.15, whiteButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	// Draw the green button
	canvas(greenButton) = Vec3b(0, 255, 0);
	putText(canvas(greenButton), "Verde", Point(greenButton.width*0.15, greenButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	// Draw the blue button
	canvas(blueButton) = Vec3b(255, 0, 0);
	putText(canvas(blueButton), "Azul", Point(blueButton.width*0.15, blueButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(255, 255, 255));

	

	// Setup callback function
	namedWindow(appName);
	setMouseCallback(appName, callBackFunc);

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

		Mat contour;
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;

		//contour.setTo(Scalar(0, 0, 255));

		cvtColor(contour, contour, COLOR_GRAY2BGR);

		Mat mask;
		if (!color.compare("red")) {
			inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
			contour.setTo(Scalar(0, 0, 255), mask);
		}
		if (!color.compare("orange")) {
			inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
			contour.setTo(Scalar(0, 140, 255), mask);
		}
		if (!color.compare("yellow")) {
			inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
			contour.setTo(Scalar(0, 255, 255), mask);
		}
		if (!color.compare("white")) {
			inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
			contour.setTo(Scalar(255, 255, 255), mask);
		}
		if (!color.compare("green")) {
			inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
			contour.setTo(Scalar(0, 255, 0), mask);
		}
		if (!color.compare("blue")) {
			inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
			contour.setTo(Scalar(255, 0, 0), mask);
		}


		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);

		// Draw the frames
		frame.copyTo(canvas(Rect(0, redButton.height, frame.cols, frame.rows)));

		// Show the canvas
		imshow(appName, canvas);
		imshow(window_detection_name, contour);

		char key = (char)waitKey(30);
		if (key == 'q' || key == 27)
		{
			break;
		}
	}
	return 0;
}