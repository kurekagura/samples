#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

#include "libavcv.h"

class H264Decoder
{
public:
	H264Decoder(const char* mp4_path, const char* decoder_name = nullptr);
	virtual ~H264Decoder();
	std::vector<cv::Mat> decode_to_mat();
	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> decode();
	AVPixelFormat pix_fmt();
	int64_t nb_frames();
	std::unique_ptr<AVFrame,deleter_for_AVFrame> seek_frame(int64_t frame_index);
	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> seek_frame(int64_t frame_index, int64_t* vector_index);

	void dump();

private:
	AVFormatContext* avfmtctx;
	AVStream* avstream;
	AVCodec* avcodec;
	AVCodecContext* avcodecctx;
};

