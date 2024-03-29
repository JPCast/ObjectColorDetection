#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>

using namespace cv;

int main()
{
	Mat originalImage;
	Mat hsv;
	originalImage = imread( "pool.jpeg", IMREAD_UNCHANGED );
	cvtColor(originalImage, hsv, CV_BGR2HSV);
	if( originalImage.empty() )
	{
		// NOT SUCCESSFUL : the data attribute is empty
		std::cout << "Image file could not be open !!" << std::endl;
	    return -1;
	}

	// Create window
    namedWindow( "Imagem Original", WINDOW_AUTOSIZE );
	// Display image
	imshow( "Imagem Original", originalImage );
	
	// Quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    calcHist( &hsv, 1, channels, Mat(), // do not use mask
             hist, 2, histSize, ranges,
             true, // the histogram is uniform
             false );
    double maxVal=0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
            rectangle( histImg, Point(h*scale, s*scale),
                        Point( (h+1)*scale - 1, (s+1)*scale - 1),
                        Scalar::all(intensity),
                        CV_FILLED );
        }

    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );
	
	
	
	
	

    // Waiting
    cv::waitKey( 0 );
	// Destroy the windows
	cv::destroyAllWindows();
	return 0;
}
