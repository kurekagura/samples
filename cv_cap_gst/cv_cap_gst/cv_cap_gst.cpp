#include <iostream>
#include <opencv2\opencv.hpp>

/// <summary>
/// 
/// </summary>
/// <param name="decoderElement">QSV->d3d11h264device1dec, NVCODEC->d3d11h264dec or nvh264dec</param>
/// <param name="outputPath"></param>
void play_to_validate(const cv::String decoderElement, const cv::String outputPath) {

	cv::String gst_cmd_cap = cv::format("filesrc location=\"%s\" !h264parse !%s !videoconvert !appsink", outputPath.c_str(), decoderElement.c_str());
	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);

	const cv::String windName = cv::format("Validating by %s", decoderElement.c_str());
	while (true)
	{
		cv::Mat cvmat;
		if (!capture.read(cvmat))
			break;

		cv::imshow(windName, cvmat);

		if (cv::waitKey(1) == 'q')
			break;
	}
}

/*
* I found HW decoder in "d3d11" and HW encoder in "mediafoundation".
* I found nvh264dec and nvh264encin "nvcodec".
*/
int main()
{
	std::cout << cv::getBuildInformation() << std::endl;

	//QSV->d3d11h264device1dec, NVCODEC->d3d11h264dec or nvh264dec
	const cv::String decoderElement = "d3d11h264device1dec";
	//QSV->mfh264device1enc, NVCODEC->mfh264enc or nvh264enc
	const cv::String encoderElement = "mfh264device1enc";

	cv::String gst_cmd_cap = cv::format("filesrc location=\"C:\\\\dev\\\\samplevideo\\\\input.264\" !h264parse !%s !videoconvert !appsink", decoderElement.c_str());
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

	cv::String gst_cmd_writer = cv::format("appsrc !videoconvert !%s !filesink location=\"%s\"", encoderElement.c_str(), outputPath.c_str());
	cv::VideoWriter writer(gst_cmd_writer, cv::CAP_GSTREAMER, out_fourcc, out_fps, cv::Size(width, height), true);

	if (!writer.isOpened())
		std::cerr << "Error : to open VideoWriter(CAP_GSTREAMER)" << std::endl;

	const cv::String windName = cv::format("Dec by %s Enc by %s", decoderElement.c_str(), encoderElement.c_str());
	while (true)
	{
		cv::Mat cvmat;
		if (!capture.read(cvmat))
			break;

		cv::imshow(windName, cvmat);

		if (cv::waitKey(1) == 'q')
			break;

		if (writer.isOpened())
			writer.write(cvmat);
	}

	cv::destroyWindow(windName);

	play_to_validate(decoderElement, outputPath);

	return 0;
}
