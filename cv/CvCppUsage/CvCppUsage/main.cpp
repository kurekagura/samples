#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
	const cv::String info = cv::getBuildInformation();
	std::cout << info << std::endl;
	return 0;
}