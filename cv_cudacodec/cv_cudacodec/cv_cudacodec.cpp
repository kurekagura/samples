#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/cudacodec.hpp>

int main()
{
	std::cout << cv::getBuildInformation() << std::endl;
	cv::cuda::printShortCudaDeviceInfo(cv::cuda::getDevice());

	cv::Ptr<cv::cudacodec::VideoReader> cuReader = cv::cudacodec::createVideoReader(cv::String("C:\\dev\\samplevideo\\input.mp4"));

	//int out_width = 640;
	//int out_height = 360;
	//int out_fourcc = cv::VideoWriter::fourcc('H', '2', '6', '4');
	//double out_fps = 30;
	//cv::Ptr<cv::cudacodec::VideoWriter> cuWriter = cv::cudacodec::createVideoWriter(cv::String("C:\\dev\\samplevideo\\out.mp4"), cv::Size(out_width, out_height), out_fps);
	//if (cuWriter.empty())
	//	std::cerr << "Failed : to open VideoWriter" << std::endl;

	while (true) {
		cv::cuda::GpuMat gframe;
		if (!cuReader->nextFrame(gframe))
			break;

		//cv::Mat frame;
		//gframe.download(frame);
		//if (!cuWriter.empty())
		//	cuWriter->write(frame);

		cv::namedWindow("MyWindow1", cv::WINDOW_OPENGL | cv::WINDOW_AUTOSIZE);
		cv::imshow("MyWindow1", gframe);

		if (cv::waitKey(1) == 'q')
			break;

	}
	return 0;
}