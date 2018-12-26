#include "stdafx.h"

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>

using namespace cv;

void detectColor(Mat frame, Mat frame_HSV, Mat frame_threshold, Scalar scalarLow, Scalar scalarHigh, Scalar scalarColorToShow) {
	Mat contour, mask;

	//Deteta a cor com os parametros indicados e põe a branco no frame_HSV, o resto fica preto
	inRange(frame_HSV, scalarLow, scalarHigh, frame_threshold);

	//Erosão seguida de dilatação para retirar pequenos ruídos da cor indicada
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


int main(int argc, char* argv[])
{
	VideoCapture cap(argc > 1 ? atoi(argv[1]) : 0);

	Mat first_frame, frame, frame_HSV, frame_threshold;

	while (true) {
		cap >> frame;
		if (frame.empty())
		{
		break;
		}
		//frame = cv::imread("pool.jpeg");

		//Fazer de espelho para ser mais fácil para testar com o objeto na camara do laptop
		flip(frame, frame, 1);

		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

		//Color red
		detectColor(frame, frame_HSV, frame_threshold, Scalar(160, 120, 90), Scalar(180, 255, 255), Scalar(0, 0, 255));
		//Color orange
		detectColor(frame, frame_HSV, frame_threshold, Scalar(0, 120, 160), Scalar(20, 255, 255), Scalar(0, 140, 255));
		//Color yellow
		detectColor(frame, frame_HSV, frame_threshold, Scalar(20, 40, 170), Scalar(40, 255, 255), Scalar(0, 255, 255));
		//Color white
		detectColor(frame, frame_HSV, frame_threshold, Scalar(80, 0, 220), Scalar(110, 255, 255), Scalar(255, 255, 255));
		//Color green
		detectColor(frame, frame_HSV, frame_threshold, Scalar(60, 110, 100), Scalar(90, 255, 255), Scalar(0, 255, 0));
		//Color blue
		detectColor(frame, frame_HSV, frame_threshold, Scalar(100, 140, 160), Scalar(140, 255, 255), Scalar(255, 0, 0));

		// Show the canvas
		imshow("ColorDetection", frame);

		char key = (char)waitKey(30);
		if (key == 'q' || key == 27)
		{
			break;
		}
	}
	return 0;
}