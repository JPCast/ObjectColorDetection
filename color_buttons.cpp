#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>
#include <list>

using namespace cv;
using namespace std;

Mat3b canvas;
String appName = "ColorDetection";

Rect redButton;
Rect orangeButton;
Rect yellowButton;
Rect whiteButton;
Rect greenButton;
Rect blueButton;

bool redSelect = false;
bool orangeSelect = false;
bool yellowSelect = false;
bool whiteSelect = false;
bool greenSelect = false;
bool blueSelect = false;

int count_selections = 0;


const int max_value_H = 360 / 2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
const String control_panel = "Control Panel HSV";

String color = "";

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

	imshow(control_panel, frame_threshold);
	/*if(count_selections==1)
		imshow(control_panel, frame_threshold);
	else
		destroyWindow(control_panel);*/
}


void callBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{ 	
		if (redButton.contains(Point(x, y)))
		{
			//cleanAllButtonsSelections();
			rectangle(canvas, redButton, Scalar(0, 0, 0), 2);
			color = "red";
			if(redSelect){
				redSelect = false;
				count_selections-=1;
				rectangle(canvas, redButton, Scalar(0, 0, 255), 2);	
			}
			else{
				redSelect = true;
				count_selections+=1;
			}
			
		}
		if (orangeButton.contains(Point(x, y)))
		{
			//cleanAllButtonsSelections();
			rectangle(canvas, orangeButton, Scalar(0, 0, 0), 2);
			color = "orange";
			if(orangeSelect){
				orangeSelect = false;
				count_selections-=1;
				rectangle(canvas, orangeButton, Scalar(0, 140, 255), 2);	
			}
			else{
				orangeSelect = true;
				count_selections+=1;
			}
			
		}
		if (yellowButton.contains(Point(x, y)))
		{
			//cleanAllButtonsSelections();
			rectangle(canvas, yellowButton, Scalar(0, 0, 0), 2);
			color = "yellow";
			if(yellowSelect){
				yellowSelect = false;	
				count_selections-=1;
				rectangle(canvas, yellowButton, Scalar(0, 255, 255), 2);
			}
			else{
				yellowSelect = true;
				count_selections+=1;
			}
			
		}
		if (whiteButton.contains(Point(x, y)))
		{
			//cleanAllButtonsSelections();
			rectangle(canvas, whiteButton, Scalar(0, 0, 0), 2);
			color = "white";
			if(whiteSelect){
				whiteSelect = false;	
				count_selections-=1;
				rectangle(canvas, whiteButton, Scalar(255, 255, 255), 2);
			}
			else{
				whiteSelect = true;
				count_selections+=1;
			}
			
		}
		if (greenButton.contains(Point(x, y)))
		{
			//cleanAllButtonsSelections();
			rectangle(canvas, greenButton, Scalar(0, 0, 0), 2);
			color = "green";
			if(greenSelect){
				greenSelect = false;	
				count_selections-=1;
				rectangle(canvas, greenButton, Scalar(0, 255, 0), 2);
			}
			else{
				greenSelect = true;
				count_selections+=1;
			}
			
		}
		if (blueButton.contains(Point(x, y)))
		{
			//cleanAllButtonsSelections();
			rectangle(canvas, blueButton, Scalar(0, 0, 0), 2);
			low_H = 100, high_H = 140;
			low_S = 140, high_S = 255;
			low_V = 160, high_V = 255;
			color = "blue";
			if(blueSelect){
				blueSelect = false;	
				count_selections-=1;
				rectangle(canvas, blueButton, Scalar(255, 0, 0), 2);
			}
			else{
				blueSelect = true;
				count_selections+=1;
			}
			
		}

	}
	/*if (event == EVENT_LBUTTONUP)
	{
		rectangle(canvas, redButton, Scalar(0, 0, 255), 2);
	}*/

	imshow(appName, canvas);
	waitKey(1);
}


static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", control_panel, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", control_panel, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", control_panel, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", control_panel, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", control_panel, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", control_panel, high_V);
}

int main(int argc, char* argv[])
{

	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);
	namedWindow(appName);
	namedWindow(window_detection_name);
	namedWindow(control_panel);
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low S", control_panel, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", control_panel, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", control_panel, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", control_panel, &high_V, max_value, on_high_V_thresh_trackbar);

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
	Mat mask;

	while (true) {
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		flip(frame, frame, 1);
		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

		detectColor(frame, frame_HSV, frame_threshold, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), Scalar(0, 0, 0));
		
		detectColor(frame, frame_HSV, frame_threshold, Scalar(0, 0, 0), Scalar(max_value_H, max_value, max_value), Scalar(0, 0, 0));
		if(redSelect){
			detectColor(frame, frame_HSV, frame_threshold, Scalar(160, 120, 90), Scalar(180, 255, 255), Scalar(0, 0, 255));
		}
		if(orangeSelect){
			detectColor(frame, frame_HSV, frame_threshold, Scalar(0, 120, 160), Scalar(20, 255, 255), Scalar(0, 140, 255));
		}
		if(yellowSelect){
			detectColor(frame, frame_HSV, frame_threshold, Scalar(20, 40, 170), Scalar(40, 255, 255), Scalar(0, 255, 255));
		}
		if(whiteSelect){
			detectColor(frame, frame_HSV, frame_threshold, Scalar(80, 0, 220), Scalar(110, 255, 255), Scalar(255, 255, 255));
		}
		if(greenSelect){
			detectColor(frame, frame_HSV, frame_threshold, Scalar(60, 110, 100), Scalar(90, 255, 255), Scalar(0, 255, 0));
		}
		if(blueSelect){
			detectColor(frame, frame_HSV, frame_threshold, Scalar(100, 140, 160), Scalar(140, 255, 255), Scalar(255, 0, 0));
		}

		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);

		// Draw the frames
		frame.copyTo(canvas(Rect(0, redButton.height, frame.cols, frame.rows)));

		// Show the canvas
		imshow(appName, canvas);
		imshow(window_detection_name, contour);
		//imshow(control_panel, frame_threshold);

		char key = (char)waitKey(30);
		if (key == 'q' || key == 27)
		{
			break;
		}

	}
	return 0;
}