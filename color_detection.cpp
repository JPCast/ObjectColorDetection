//#include "stdafx.h"
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <list>

using namespace cv;
using namespace std;

Mat3b canvas;
Mat frame;

String appName = "ColorDetection";

Rect redButton;
Rect orangeButton;
Rect yellowButton;
Rect whiteButton;
Rect greenButton;
Rect blueButton;
Rect unselectButton;

bool redSelect = false;
bool orangeSelect = false;
bool yellowSelect = false;
bool whiteSelect = false;
bool greenSelect = false;
bool blueSelect = false;

bool clicked = false;

const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_detection_name = "Object Detection";

int B, R, G = 0;
int H, S, V = 0;
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;
Mat contour, mask;

void cleanAllButtonsSelections() {
	rectangle(canvas, redButton, Scalar(0, 0, 255), 2);
	rectangle(canvas, orangeButton, Scalar(0, 140, 255), 2);
	rectangle(canvas, yellowButton, Scalar(0, 255, 255), 2);
	rectangle(canvas, whiteButton, Scalar(255, 255, 255), 2);
	rectangle(canvas, greenButton, Scalar(0, 255, 0), 2);
	rectangle(canvas, blueButton, Scalar(255, 0, 0), 2);
}

void cleanUnselectButtonSelection() {
	rectangle(canvas, unselectButton, Scalar(128, 128, 128), 2);
}

void detectColor(Mat frame, Mat frame_HSV, Mat frame_threshold, Scalar scalarLow, Scalar scalarHigh, Scalar scalarColorToShow) {


	//Deteta a cor com os parametros indicados e põe a branco no frame_HSV, o resto fica preto
	inRange(frame_HSV, scalarLow, scalarHigh, frame_threshold);

	//Erosão seguida de dilatação para retirar pequenos ruídos
	erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
	dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);

	//Dilatação e subtração para obter os contornos
	dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
	contour = contour - frame_threshold;

	//Alterar a cor dos contornos para a cor desejada
	cvtColor(contour, contour, COLOR_GRAY2BGR);
	inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
	contour.setTo(scalarColorToShow, mask);

	//Adicionar à imagem original(frame) os contornos com transparência
	addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);
}


void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		if (redButton.contains(Point(x, y)))
		{
			clicked = false;
			cleanUnselectButtonSelection();
			rectangle(canvas, redButton, Scalar(0, 0, 0), 2);
			if (redSelect) {
				redSelect = false;
				rectangle(canvas, redButton, Scalar(0, 0, 255), 2);
			}
			else {
				redSelect = true;
			}

		}
		else if (orangeButton.contains(Point(x, y)))
		{
			clicked = false;
			cleanUnselectButtonSelection();
			rectangle(canvas, orangeButton, Scalar(0, 0, 0), 2);
			if (orangeSelect) {
				orangeSelect = false;
				rectangle(canvas, orangeButton, Scalar(0, 140, 255), 2);
			}
			else {
				orangeSelect = true;
			}

		}
		else if (yellowButton.contains(Point(x, y)))
		{
			cleanUnselectButtonSelection();
			clicked = false;
			rectangle(canvas, yellowButton, Scalar(0, 0, 0), 2);
			if (yellowSelect) {
				yellowSelect = false;
				rectangle(canvas, yellowButton, Scalar(0, 255, 255), 2);
			}
			else {
				yellowSelect = true;
			}

		}
		else if (whiteButton.contains(Point(x, y)))
		{
			cleanUnselectButtonSelection();
			clicked = false;
			rectangle(canvas, whiteButton, Scalar(0, 0, 0), 2);
			if (whiteSelect) {
				whiteSelect = false;
				rectangle(canvas, whiteButton, Scalar(255, 255, 255), 2);
			}
			else {
				whiteSelect = true;
			}

		}
		else if (greenButton.contains(Point(x, y)))
		{
			cleanUnselectButtonSelection();
			clicked = false;
			rectangle(canvas, greenButton, Scalar(0, 0, 0), 2);
			if (greenSelect) {
				greenSelect = false;
				rectangle(canvas, greenButton, Scalar(0, 255, 0), 2);
			}
			else {
				greenSelect = true;
			}

		}
		else if (blueButton.contains(Point(x, y)))
		{
			cleanUnselectButtonSelection();
			clicked = false;
			rectangle(canvas, blueButton, Scalar(0, 0, 0), 2);
			low_H = 100, high_H = 140;
			low_S = 140, high_S = 255;
			low_V = 160, high_V = 255;
			if (blueSelect) {
				blueSelect = false;
				rectangle(canvas, blueButton, Scalar(255, 0, 0), 2);
			}
			else {
				blueSelect = true;
			}

		}
		else if (unselectButton.contains(Point(x, y)))
		{
			clicked = false;
			cleanAllButtonsSelections();
			
			redSelect = false;
			orangeSelect = false;
			yellowSelect = false;
			whiteSelect = false;
			greenSelect = false;
			blueSelect = false;

			//Colocar retangulo preto à volta do botão unselect
			rectangle(canvas, unselectButton, Scalar(0, 0, 0), 2);

		}
		else {
			clicked = true;
			cleanAllButtonsSelections();
			cleanUnselectButtonSelection();

			redSelect = false;
			orangeSelect = false;
			yellowSelect = false;
			whiteSelect = false;
			greenSelect = false;
			blueSelect = false;

			//Colocar retangulo preto à volta do botão unselect
			rectangle(canvas, unselectButton, Scalar(0, 0, 0), 2);

			setTrackbarPos("Low S", appName, 0);
			setTrackbarPos("High S", appName, 255);
			setTrackbarPos("Low V", appName, 0);
			setTrackbarPos("High V", appName, 255);

			// Adapted from http://answers.opencv.org/question/30547/need-to-know-the-hsv-value/
			Mat HSV;

			Mat RGB = canvas(Rect(x, y, 5, 5));
			RGB = mean(RGB);
			cvtColor(RGB, HSV, CV_BGR2HSV);
			Vec3b hsv = HSV.at<Vec3b>(0, 0);
			H = hsv.val[0];
			S = hsv.val[1];
			V = hsv.val[2];

			Mat rgb_version;
			cvtColor(HSV, rgb_version, CV_HSV2BGR);
			Vec3b bgr = rgb_version.at<Vec3b>(0, 0);
			B = bgr.val[0];
			G = bgr.val[1];
			R = bgr.val[2];
		}

	}

	imshow(appName, canvas);
	waitKey(1);
}


static void on_low_H_thresh_trackbar(int, void *)
{
	low_H = min(high_H - 1, low_H);
	setTrackbarPos("Low H", appName, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
	high_H = max(high_H, low_H + 1);
	setTrackbarPos("High H", appName, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
	low_S = min(high_S - 1, low_S);
	setTrackbarPos("Low S", appName, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
	high_S = max(high_S, low_S + 1);
	setTrackbarPos("High S", appName, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
	low_V = min(high_V - 1, low_V);
	setTrackbarPos("Low V", appName, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
	high_V = max(high_V, low_V + 1);
	setTrackbarPos("High V", appName, high_V);
}

int main(int argc, char* argv[])
{
	int top_H, bottom_H = 0;
	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
	namedWindow(appName);
	namedWindow(window_detection_name);

	// Trackbars to set thresholds for HSV values
	createTrackbar("Low S", appName, &low_S, max_value, on_low_S_thresh_trackbar);
	createTrackbar("High S", appName, &high_S, max_value, on_high_S_thresh_trackbar);
	createTrackbar("Low V", appName, &low_V, max_value, on_low_V_thresh_trackbar);
	createTrackbar("High V", appName, &high_V, max_value, on_high_V_thresh_trackbar);

	Mat first_frame, frame_HSV, frame_threshold;

	//Primeiro frame
	cap >> first_frame;
	//Imagem como espelho, mais fácil para testar com a câmara do laptop
	flip(first_frame, first_frame, 1);

	int numberOfButtons = 7;
	// Your buttons
	redButton = Rect(0 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);
	orangeButton = Rect(1 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);
	yellowButton = Rect(2 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);
	whiteButton = Rect(3 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);
	greenButton = Rect(4 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);
	blueButton = Rect(5 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);
	unselectButton = Rect(6 * first_frame.cols / numberOfButtons, 0, first_frame.cols / numberOfButtons, 50);

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
	// Draw the unselect button
	canvas(unselectButton) = Vec3b(128, 128, 128);
	putText(canvas(unselectButton), "Unselect", Point(unselectButton.width*0.15, unselectButton.height*0.7), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
	//Select the unselect button
	//rectangle(canvas, unselectButton, Scalar(0, 0, 0), 2);


	// Setup callback function
	namedWindow(appName);
	setMouseCallback(appName, callBackFunc);
	Mat mask;

	while (true) {
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		//Imagem como espelho, mais fácil para testar com a câmara do laptop
		flip(frame, frame, 1);
		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

		//Clique num pixel para seguir essa cor
		if (clicked) {
			//printf("Detecting color: %d, %d, %d\n", H, S, V);
			if (H + 30 >= 180)
				top_H = 180;
			else
				top_H = H + 30;

			if (H - 30 <= 0)
				bottom_H = 0;
			else
				bottom_H = H - 30;

			//printf("Borders are: %d, %d, %d\n", B, G, R);

			
			detectColor(frame, frame_HSV, frame_threshold, Scalar(bottom_H, low_S, low_V), Scalar(top_H, high_S, high_V), Scalar(B, G, R));
		}
		else {
			//Clique num dos botões de cor predefinidos
			if (redSelect || orangeSelect || yellowSelect || whiteSelect || greenSelect || blueSelect) {
				if (redSelect) {
					detectColor(frame, frame_HSV, frame_threshold, Scalar(160, low_S, low_V), Scalar(180, high_S, high_V), Scalar(0, 0, 255));
				}
				if (orangeSelect) {
					detectColor(frame, frame_HSV, frame_threshold, Scalar(0, low_S, low_V), Scalar(20, high_S, high_V), Scalar(0, 140, 255));
				}
				if (yellowSelect) {
					detectColor(frame, frame_HSV, frame_threshold, Scalar(20, low_S, low_V), Scalar(40, high_S, high_V), Scalar(0, 255, 255));
				}
				if (whiteSelect) {
					detectColor(frame, frame_HSV, frame_threshold, Scalar(80, low_S, low_V), Scalar(110, high_S, high_V), Scalar(255, 255, 255));
				}
				if (greenSelect) {
					detectColor(frame, frame_HSV, frame_threshold, Scalar(60, low_S, low_V), Scalar(90, high_S, high_V), Scalar(0, 255, 0));
				}
				if (blueSelect) {
					detectColor(frame, frame_HSV, frame_threshold, Scalar(100, low_S, low_V), Scalar(140, high_S, high_V), Scalar(255, 0, 0));
				}
			}
			else {
				detectColor(frame, frame_HSV, frame_threshold, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), Scalar(0, 0, 0));
			}
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