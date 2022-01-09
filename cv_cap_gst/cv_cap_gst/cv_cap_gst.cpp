#include <iostream>
#include <opencv2\opencv.hpp>

void play_to_validate(const cv::String outputPath) {

	cv::String gst_cmd_cap("filesrc location=\"");
	gst_cmd_cap += outputPath;
	gst_cmd_cap += "\" !h264parse !d3d11h264device1dec !videoconvert !appsink";
	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);

	while (true)
	{
		cv::Mat cvmat;
		if (!capture.read(cvmat))
			break;

		cv::imshow("Validating:" + outputPath, cvmat);

		if (cv::waitKey(1) == 'q')
			break;
	}
}

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

	int out_width = width;
	int out_height = height;
	int out_fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');
	double out_fps = fps;

	const cv::String outputPath("C:\\\\dev\\\\samplevideo\\\\out-cv_cap_gst.264");
	cv::String gst_cmd_writer("appsrc !videoconvert !mfh264device1enc !filesink location=\"");
	gst_cmd_writer += outputPath;
	gst_cmd_writer += "\"";
	cv::VideoWriter writer(gst_cmd_writer, cv::CAP_GSTREAMER, out_fourcc, out_fps, cv::Size(width, height), true);
	
	if (!writer.isOpened())
		std::cerr << "Error : to open VideoWriter(CAP_GSTREAMER)" << std::endl;

	const cv::String windowName("window1 - cv_cap_gst");
	while (true)
	{
		cv::Mat cvmat;
		if (!capture.read(cvmat))
			break;

		cv::imshow(windowName, cvmat);

		if (cv::waitKey(1) == 'q')
			break;

		if (writer.isOpened())
			writer.write(cvmat);
	}

	cv::destroyWindow(windowName);

	play_to_validate(outputPath);

	return 0;
}
