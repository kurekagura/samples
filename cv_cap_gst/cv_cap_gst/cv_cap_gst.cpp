#include <iostream>
#include <opencv2\opencv.hpp>

/// <summary>
/// 
/// </summary>
/// <param name="decoderElement">
/// QSV->d3d11h264device1dec, NVCODEC->d3d11h264dec or nvh264dec.
/// openh264dec
/// </param>
/// <param name="outputPath"></param>
/// <returns></returns>
bool play_to_validate(const cv::String decoderElement, const cv::String outputPath, bool input_mp4) {

	cv::String gst_cmd_cap = cv::format("filesrc location=\"%s\" %s !h264parse !%s !videoconvert !appsink",
		outputPath.c_str(),
		input_mp4 ? "!qtdemux" : "",
		decoderElement.c_str());
	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);
	if (!capture.isOpened())
	{
		std::cerr << "Failed : to open VideoCapture(CAP_GSTREAMER)" << std::endl;
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
* I found HW decoder in "d3d11" and HW encoder in "mediafoundation".
* I found nvh264dec and nvh264encin "nvcodec".
* openh264dec can NOT play .264.
*/
int main(int argc, char* argv[])
{
	std::cout << cv::getBuildInformation() << std::endl;

	// QSV->d3d11h264device1dec, NVCODEC->d3d11h264dec or nvh264dec
	// openh264dec
	const cv::String decoderElement = "d3d11h264device1dec";
	bool input_mp4 = true;

	// QSV->mfh264device1enc, NVCODEC->mfh264enc or nvh264enc, openh264enc
	// x264enc:.264出力はNG?
	// x264encのみがqtmuxの入力に対応している（？）
	// x264enc以外では「could not link nvh264enc0 to qtmux0」エラーが発生。
	const cv::String encoderElement = "x264enc";
	bool output_mp4 = true;

	//To extract h264 in .mp4, need qtdemux.
	cv::String gst_cmd_cap = cv::format("filesrc location=\"C:\\\\dev\\\\samplevideo\\\\input.%s\" %s !h264parse !%s !videoconvert !appsink",
		input_mp4 ? "mp4" : "264",	//ファイル拡張子.
		input_mp4 ? "!qtdemux" : "",
		decoderElement.c_str());
	//cv::String gst_cmd_cap = cv::format("filesrc location=\"C:\\\\dev\\\\samplevideo\\\\input.mp4\" !qtdemux !queue !h264parse !videoconvert !appsink",
	//	decoderElement.c_str());
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

	const cv::String outputPath = cv::format("C:\\\\dev\\\\samplevideo\\\\out-cv_cap_gst-by-%s.%s",
		encoderElement.c_str(),
		output_mp4 ? "mp4" : "264");
	//mp4mux vs qtmux
	//x264は既定でyuv444pとなり、デコードできない場合がある為、yuv420p（I420）へ明示的に変換する。
	//!videoconvert !video/x-raw,format=I420
	cv::String conv_to_encoder = "";
	if (encoderElement.compare("x264enc")==1) {
		conv_to_encoder = "!video/x-raw,format=I420";
	}
	cv::String gst_cmd_writer = cv::format("appsrc !videoconvert %s !%s %s !filesink location=\"%s\"",
		//cv::String gst_cmd_writer = cv::format("appsrc !%s %s !filesink location=\"%s\"",	
		conv_to_encoder.c_str(),
		encoderElement.c_str(),
		output_mp4 ? "!qtmux" : "",
		outputPath.c_str());
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

	if (writer.isOpened())
		writer.release();

	cv::destroyWindow(windName);

	bool isValid = play_to_validate(decoderElement, outputPath, output_mp4);

	std::cout << "Info : isValid=" << isValid << std::endl;

	return 0;
}
