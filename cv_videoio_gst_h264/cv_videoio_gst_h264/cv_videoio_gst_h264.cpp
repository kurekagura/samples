#include <crtdbg.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <opencv2/opencv.hpp>

//using NewType = std::unique_ptr<std::vector< std::unique_ptr<cv::Mat> >>;

/// <summary>
/// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
/// </summary>
/// <param name="mat"></param>
std::string CvMatType2String(const cv::Mat& cvmat) {
	std::string str = "CV_";

	int  type = cvmat.type();
	uchar depth_cha = type & CV_MAT_DEPTH_MASK;
	int depth = cvmat.depth();

	uchar channels_cha = 1 + (type >> CV_CN_SHIFT);
	int channels = cvmat.channels();

	switch (depth_cha) {
	case CV_8U:  str += "8U"; break;
	case CV_8S:  str += "8S"; break;
	case CV_16U: str += "16U"; break;
	case CV_16S: str += "16S"; break;
	case CV_32S: str += "32S"; break;
	case CV_32F: str += "32F"; break;
	case CV_64F: str += "64F"; break;
	default:     str += "OTHER"; break;
	}

	str += "C";
	str += (channels_cha + '0');

	return str;
}

std::vector<std::unique_ptr<cv::Mat>> decode(const std::string decoderElement, const cv::String inputPath, bool input_mp4, int& width, int& height, double& fps) {

	std::vector<std::unique_ptr<cv::Mat>> frames;

	//openh264decの場合、.mp4の45/300、.264の85/300フレームで止まる。何かが足りない？
	cv::String gst_cmd_cap = cv::format("filesrc location=\"%s\"%s !h264parse !%s !videoconvert !appsink",
		inputPath.c_str(),
		input_mp4 ? " !qtdemux" : "",
		decoderElement.c_str());

	cv::VideoCapture capture(gst_cmd_cap, cv::CAP_GSTREAMER);
	if (!capture.isOpened())
	{
		std::cerr << "Failed : to open VideoCapture(CAP_GSTREAMER)" << std::endl;
		return frames;
	}

	width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH); //valid
	height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT); //valid
	fps = capture.get(cv::CAP_PROP_FPS);	//valid
	int count = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);	//?
	int fourcc = (int)capture.get(cv::CAP_PROP_FOURCC);	//?

	while (true)
	{
		cv::Mat cvmat;
		if (!capture.read(cvmat))
			break;

		std::unique_ptr<cv::Mat> ptr = std::make_unique<cv::Mat>(cvmat);
		frames.push_back(std::move(ptr));
	}
	return frames;
}

void encode(const std::string encoderElement, const std::vector<std::unique_ptr<cv::Mat>>& frames, const cv::String outputPath, bool output_mp4,
	int width, int height, double fps) {

	using namespace std;

	const int fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');

	cv::String gst_cmd_writer;
	if (encoderElement == "mfh264device1enc")
	{
		//capsfilterがあると失敗する。無くてもyuv420pになる。
		gst_cmd_writer = cv::format("appsrc !videoconvert !%s !h264parse%s !filesink location=\"%s\"",
			encoderElement.c_str(),
			output_mp4 ? " !mp4mux" : "",
			outputPath.c_str());
	}
	else if (encoderElement == "mfh264enc") {
		//capsfilterがあると失敗する。無くてもyuv420pになる。
		gst_cmd_writer = cv::format("appsrc !videoconvert !%s !h264parse%s !filesink location=\"%s\"",
			encoderElement.c_str(),
			output_mp4 ? " !mp4mux" : "",
			outputPath.c_str());
	}
	else if (encoderElement == "nvh264enc") {
		//capsfilterが無いと、yuvj420pになる。
		gst_cmd_writer = cv::format("appsrc !videoconvert !video/x-raw,format=I420 !%s !h264parse%s !filesink location=\"%s\"",
			encoderElement.c_str(),
			output_mp4 ? " !mp4mux" : "",
			outputPath.c_str());
	}
	else if (encoderElement == "openh264enc") {
		//※現行、以下のエラーが発生している。出力ファイルサイズが明らかにおかしい（少ない）。

		//capsfilterが有っても成功はする。無くてもyuv420pになる。
		//[OpenH264] this = 0x000001C33DF83EA0, Error:CWelsH264SVCEncoder::EncodeFrame(), cmInitParaError.
		gst_cmd_writer = cv::format("appsrc !videoconvert !%s !h264parse%s !filesink location=\"%s\"",
			encoderElement.c_str(),
			output_mp4 ? " !mp4mux" : "",
			outputPath.c_str());
	}
	else if (encoderElement == "x264enc")
	{
		//capsfilterが無いと、yuv444pになる。
		gst_cmd_writer = cv::format("appsrc !videoconvert !video/x-raw,format=I420 !%s !h264parse%s !filesink location=\"%s\"",
			encoderElement.c_str(),
			output_mp4 ? " !mp4mux" : "",
			outputPath.c_str());
	}

	cv::VideoWriter writer(gst_cmd_writer, cv::CAP_GSTREAMER, fourcc, fps, cv::Size(width, height), true);

	if (!writer.isOpened())
		std::cerr << "Error : to open VideoWriter(CAP_GSTREAMER)" << std::endl;
	else
	{
		cout << "Encoder : " << encoderElement << endl;
		for (int i = 0; i < frames.size(); i++) {
			const std::unique_ptr<cv::Mat>& f = frames[i];
			writer.write(*f);
			cout << i << " ";
		}
		cout << endl;
		writer.release();
	}
}

int main(int argc, char* argv[])
{
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);

	using namespace std;

	const vector<string> decoderElements({
		"d3d11h264device1dec",
		"d3d11h264dec",
		"nvh264dec",
		"openh264dec", //300fameの動画が45frameで停止している。
		});

	map<string, vector<unique_ptr<cv::Mat>>> decoder_frames;
	int input_width = -1;
	int input_height = -1;
	double input_fps = -1;

	bool input_mp4 = true;
	{
		cv::String inputPath = cv::format("C:\\\\dev\\\\samplevideo\\\\test-cv_videoio_gst_h264\\\\Big_Buck_Bunny_360_10s_1MB.%s", input_mp4 ? "mp4" : "264");
		for (int decIdx = 0; decIdx < decoderElements.size(); decIdx++) {
			const auto& decoderElement = decoderElements[decIdx];
			int a_width, a_height;
			double a_fps;
			decoder_frames[decoderElement] = decode(decoderElement, inputPath, input_mp4, a_width, a_height, a_fps);
			if (input_width == -1)
				input_width = a_width;
			else
				assert(input_width == a_width);

			if (input_height == -1)
				input_height = a_height;
			else
				assert(input_height == a_height);

			if (input_fps == -1.0)
				input_fps = a_fps;
			else
				assert(input_fps == a_fps);
		}
	}

	for (auto itr = decoder_frames.begin(); itr != decoder_frames.end(); ++itr) {
		cout << "encoder : " << itr->first << endl;
		const auto& vec = itr->second;
		cout << CvMatType2String(*vec[0]) << endl;
		cout << "frame count : " << vec.size()  << endl;
	}

	const vector<string> encoderElements({
		"mfh264device1enc", //QSV
		"mfh264enc",	//NVCODEC
		"nvh264enc",	//NVCODEC
		"openh264enc",
		"x264enc"		//264出力はNG?
		});

	bool output_mp4 = true;
	//全デコーダからの結果を全エンコーダに対して渡してエンコード。
	for (int decIdx = 0; decIdx < decoderElements.size(); decIdx++) {
		const std::string target_decoder = decoderElements[decIdx];
		const vector<unique_ptr<cv::Mat>>& targetFrames = decoder_frames[target_decoder];
		for (int encIdx = 0; encIdx < encoderElements.size(); encIdx++) {
			const string& encoderElement = encoderElements[encIdx];
			const cv::String outputPath = cv::format("C:\\\\dev\\\\samplevideo\\\\test-cv_videoio_gst_h264\\\\DecBy-%s-EncBy-%s.%s",
				target_decoder.c_str(), encoderElement.c_str(), output_mp4 ? "mp4" : "264");

			encode(encoderElement, targetFrames, outputPath, input_mp4, input_width, input_height, input_fps);
		}
	}
	return 0;
}