#include <iostream>
#include <opencv2\opencv.hpp>

int main()
{
	std::cout << cv::getBuildInformation() << std::endl;

	//cv::String gst_cmd_cap("videotestsrc ! appsink");
	cv::String gst_cmd_cap("filesrc location=\"C:\\\\dev\\\\samplevideo\\\\input.264\" !h264parse !d3d11h264device1dec !videoconvert !appsink");
	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);

	if (!capture.isOpened())
	{
		std::cerr << "Failed : to open VideoCapture(CAP_GSTREAMER)" << std::endl;
		return -1;
	}
	int width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH); //ok
	int height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT); //ok
	int count = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);	//?
	int fourcc = (int)capture.get(cv::CAP_PROP_FOURCC);	//?
	double fps = capture.get(cv::CAP_PROP_FPS);	//ok

	while (true)
	{
		cv::Mat cvmat;
		if (!capture.read(cvmat))
			break;

		cv::imshow("MyWindow1", cvmat);

		if (cv::waitKey(1) == 'q')
			break;
	}
	return 0;
}
