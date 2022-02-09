#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <opencv2/opencv.hpp>

struct deleter_for_AVFrame {
	void operator()(AVFrame* ptr_) {
		av_frame_free(&ptr_);
	}
};

cv::Mat& convert_avframe_to_mat(SwsContext* swsctx, AVFrame* srcframe, AVPixelFormat dst_pix_fmt);

AVFrame* convert_mat_to_avframe(SwsContext* swsctx, const cv::Mat& mat, AVPixelFormat dst_pix_fmt);
