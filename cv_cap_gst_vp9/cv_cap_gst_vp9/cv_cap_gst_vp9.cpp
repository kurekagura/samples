#include <iostream>
#include <opencv2\opencv.hpp>

/// <summary>
/// 
/// </summary>
/// <param name="decoderElement">mfvp9enc(ng), QSV->?, NVCODEC->? ,ON2=vp9enc</param>
/// <param name="outputPath"></param>
bool play_to_validate(const cv::String decoderElement, const cv::String outputPath) {

	cv::String gst_cmd_cap = cv::format("filesrc location=\"%s\" !matroskademux !%s !videoconvert !appsink", outputPath.c_str(), decoderElement.c_str());
	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);
	if (!capture.isOpened())
	{
		std::cerr << "Failed : to open VideoCapture(CAP_GSTREAMER) for WebM." << std::endl;
		return false;
	}

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
	cv::destroyWindow(windName);
	return true;
}

/*
* vp9enc was very slow because of S/W encoder?.
*/
int main()
{
	std::cout << cv::getBuildInformation() << std::endl;

	//QSV->d3d11vp9device1dec(ok), NVCODEC->d3d11vp9dec(ok) or nvvp9dec(ok), ON2=vp9dec(ok)
	const cv::String decoderElement = "vp9dec";

	//mfvp9enc(ng), QSV->?, NVCODEC->? ,ON2=vp9enc
	const cv::String encoderElement = "vp9enc";

	cv::String inputPath = "C:\\\\dev\\\\samplevideo\\\\input.webm"; //!matroskademux 
	cv::String gst_cmd_cap = cv::format("filesrc location=\"%s\" !matroskademux !%s !videoconvert !appsink", inputPath.c_str(), decoderElement.c_str());
	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);

	if (!capture.isOpened())
	{
		std::cerr << "Failed : to open VideoCapture(CAP_GSTREAMER) for WebM." << std::endl;
		return -1;
	}
	int width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH); //ok
	int height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT); //ok
	int count = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);	//ok
	int fourcc = (int)capture.get(cv::CAP_PROP_FOURCC);	//?
	double fps = capture.get(cv::CAP_PROP_FPS);	//ok

	int out_width = width;
	int out_height = height;
	int out_fourcc = cv::VideoWriter::fourcc('V', 'P', '0', '9');
	double out_fps = fps;

	const cv::String outputPath("C:\\\\dev\\\\samplevideo\\\\out-cv_gst_vp9.webm.");

	//matroskamux includes webmmux?
	cv::String gst_cmd_writer = cv::format("appsrc !videoconvert !%s !webmmux !filesink location=\"%s\"", encoderElement.c_str(), outputPath.c_str());
	cv::VideoWriter writer(gst_cmd_writer, cv::CAP_GSTREAMER, out_fourcc, out_fps, cv::Size(out_width, out_height), true);

	if (!writer.isOpened())
		std::cerr << "Error : to open VideoWriter(CAP_GSTREAMER) for WebM" << std::endl;

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

	bool isValid = play_to_validate(decoderElement, outputPath);
	if (!isValid)
	{
		std::cerr << "Failed : to validate the output." << std::endl;
	}
	return isValid ? 0 : -1;
}
