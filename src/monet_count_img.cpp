/*
* @Author: Pavlo Boidachenko
* @Date:   2018-12-06 14:48:47
* @Last Modified by:   pavlo
* @Last Modified time: 2018-12-06 17:08:12
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

#define cout std::cout;
#define endl std::endl;

int main(int argc, char const *argv[]){
	Mat src_img;
	Mat res_img;

	int dilation_size = 10;

	char img_name[] = "./images/tst1.jpg";

	src_img = imread(img_name, CV_LOAD_IMAGE_ANYCOLOR);
	if(src_img.empty()){
		std::cerr<<"Image read error."<<endl;
	}

	//convert to grayscale
	cvtColor(src_img, res_img, CV_RGB2GRAY);
	//binarize
	// threshold(res_img, res_img, 100, 255, THRESH_BINARY);
	//dilatation
	blur(res_img, res_img, Size( 10, 10 ), Point(-1, -1));
	// Mat element = getStructuringElement(MORPH_ELLIPSE,
	// 								   Size(2*dilation_size + 1, 2*dilation_size+1),
	// 								   Point(dilation_size, dilation_size));
	// dilate(res_img, res_img, element);

	std::vector<Vec3f> circles;
	HoughCircles(res_img, circles, HOUGH_GRADIENT, 1,
				 res_img.rows/16,  // change this value to detect circles with different distances to each other
				 100, 30, 1, 60 // change the last two parameters
			// (min_radius & max_radius) to detect larger circles
	);
	for( size_t i = 0; i < circles.size(); i++ )
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(src_img, center, 1, Scalar(0,100,100), 3, LINE_AA);
		// circle outline
		int radius = c[2];
		circle(src_img, center, radius, Scalar(255,0,255), 3, LINE_AA);
	}

	if (! cv::imwrite("res.jpg", src_img)) {
		std::cerr << "Image write error"<<endl;;
	}

	return 0;
}