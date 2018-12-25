#include "stdafx.h"

#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>

using namespace cv;

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

		//Fazer de espelho para ser mais fácil para testar com o objeto na camara do laptop
		flip(frame, frame, 1);

		// Convert from BGR to HSV colorspace
		cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

		Mat contour, mask;

		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(160, 120, 90), Scalar(180, 255, 255), frame_threshold);
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;
		cvtColor(contour, contour, COLOR_GRAY2BGR);
		inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
		contour.setTo(Scalar(0, 0, 255), mask);
		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);

		
		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(0, 120, 160), Scalar(20, 255, 255), frame_threshold);
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;
		cvtColor(contour, contour, COLOR_GRAY2BGR);
		inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
		contour.setTo(Scalar(0, 140, 255), mask);
		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);


		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(20, 40, 170), Scalar(40, 255, 255), frame_threshold);
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;
		cvtColor(contour, contour, COLOR_GRAY2BGR);
		inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
		contour.setTo(Scalar(0, 255, 255), mask);
		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);


		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(80, 0, 220), Scalar(110, 255, 255), frame_threshold);
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;
		cvtColor(contour, contour, COLOR_GRAY2BGR);
		inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
		contour.setTo(Scalar(255, 255, 255), mask);
		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);


		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(60, 110, 100), Scalar(90, 255, 255), frame_threshold);
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;
		cvtColor(contour, contour, COLOR_GRAY2BGR);
		inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
		contour.setTo(Scalar(0, 255, 0), mask);
		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);


		// Detect the object based on HSV Range Values
		inRange(frame_HSV, Scalar(100, 140, 160), Scalar(140, 255, 255), frame_threshold);
		erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 7);
		dilate(frame_threshold, contour, Mat(), Point(-1, -1), 10);
		contour = contour - frame_threshold;
		cvtColor(contour, contour, COLOR_GRAY2BGR);
		inRange(contour, Scalar(255, 255, 255), Scalar(255, 255, 255), mask);
		contour.setTo(Scalar(255, 0, 0), mask);
		addWeighted(frame, 1.0, contour, 0.5, 0.0, frame);

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