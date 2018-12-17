#include <iostream>
#include <opencv2/opencv.hpp>

#define cout std::cout
#define endl std::endl;

using namespace cv;

void findCoin(Mat &src_img){
	Mat gray_img;


	//convert to grayscale
	cvtColor(src_img, gray_img, CV_RGB2GRAY);

	//binarize
	// threshold(gray_img, gray_img, 100, 255, THRESH_BINARY);

	//blur
	blur(gray_img, gray_img, Size( 8, 8 ), Point(-1, -1));

	//dilatation
	// int dilation_size = 10;
	// Mat element = getStructuringElement(MORPH_ELLIPSE,
	// 								   Size(2*dilation_size + 1, 2*dilation_size+1),
	// 								   Point(dilation_size, dilation_size));
	// dilate(gray_img, gray_img, element);

	//find circles
	std::vector<Vec3f> circles;
	HoughCircles(gray_img, circles, HOUGH_GRADIENT, 1,
				 gray_img.rows/16,  // distance between different cirlces
				 100, 30, 10, 60 
			// (min_radius & max_radius) 
	);

	//print text
	putText(src_img, std::to_string(circles.size()), Point(20, 150), FONT_ITALIC, 3, Scalar(0,0,0), 10);
	putText(src_img, std::to_string(circles.size()), Point(20, 150), FONT_ITALIC, 3, Scalar(255,255,255), 8);

	//draw cirle 
	for(size_t i = 0; i < circles.size(); i++){
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(src_img, center, 1, Scalar(0,100,100), 3, LINE_AA);
		// circle outline
		int radius = c[2];
		circle(src_img, center, radius, Scalar(255,0,255), 3, LINE_AA);
	}
}

int main(int argc, char * argv[])
{
    cv::VideoCapture webcam;
    cv::Mat src_img;

    // Dostępne kamery wideo są ponumerowane, zamiast nazwy (która zależy od
    // systemu operacyjnego) można więc użyć indeksu.
    if (! webcam.open(0)) {
        std::cerr << "Błąd przy otwieraniu kamery (może nie podłączona?).\n";
        return 1;
    }

    cv::namedWindow("Captured image");

    while (true) {
        if (! webcam.read(src_img) || src_img.empty()) {
            std::cerr << "Błąd przy pobieraniu obrazu z kamery.\n";
            break;
        }

        findCoin(src_img);
        cv::imshow("Captured image", src_img);


        // Funkcja waitKey wyświetla/odświeża okienka, a potem czeka podaną
        // liczbę milisekund na naciśnięcie klawisza.
        int keycode = cv::waitKey(10);
        // Kończymy program jeśli użytkownik cokolwiek nacisnął.
        if (keycode >= 0) {
            break;
        }
    }

    cv::destroyAllWindows();

    return 0;
}
