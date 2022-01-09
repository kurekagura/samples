#include <iostream>
#include <opencv2\opencv.hpp>

int main()
{
	//cv::VideoCapture capture("C:\\dev\\samplevideo\\Big_Buck_Bunny_360_10s_1MB.mp4", cv::CAP_MSMF);
	//CAP_INTEL_MFXは、mp4などのコンテナには未対応（raw(.264)のみに対応）。
	cv::VideoCapture capture("C:\\dev\\samplevideo\\Big_Buck_Bunny_360_10s_1MB.264", cv::CAP_INTEL_MFX);

	//cv::VideoCapture capture(0, cv::CAP_MSMF); //OK
	//cv::VideoCapture capture(0, cv::CAP_DSHOW); //OK
	//cv::VideoCapture capture(0); //OK 既定がおそらくCAP_MSMFかCAP_DSHOW

	//cv::VideoCapture capture("C:\\dev\\samplevideo\\Big_Buck_Bunny_360_10s_1MB.mp4", cv::CAP_INTEL_MFX);
	//->MFX: Unsupported extension: C:\dev\samplevideo\Big_Buck_Bunny_360_10s_1MB.mp4

	int width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);
	int count = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
	int fourcc = (int)capture.get(cv::CAP_PROP_FOURCC);
	double fps = capture.get(cv::CAP_PROP_FPS);

	//cv::VideoWriter writer("out.avi", cv::CAP_FFMPEG, fourcc, fps, cv::Size(width, height), false);
	//未指定はCAP_FFMPEG(->openh264-1.8.0-win64.dllが必要)

	int out_fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');
	double out_fps = 30;
	
	cv::VideoWriter writer("C:\\dev\\samplevideo\\out_CAP_MSMF.mp4", cv::CAP_MSMF, out_fourcc, out_fps, cv::Size(width, height), true);
	//cv::VideoWriter writer("C:\\dev\\samplevideo\\out_CAP_INTEL_MFX.264", cv::CAP_INTEL_MFX, out_fourcc, out_fps, cv::Size(width, height), true);

	if (!writer.isOpened())
		std::cerr << "Error : to open VideoWriter" << std::endl;

	while (true)
	{
		cv::Mat frame;
		capture >> frame;
		if (frame.empty())
			break;

		cv::imshow("cv_cap_mfx_window", frame);

		if (cv::waitKey(1) == 'q')
			break;

		if (writer.isOpened())
			writer << frame;
	}

	return 0;
}