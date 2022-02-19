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

cv::Mat& convert_avframe_to_mat(AVFrame* srcframe, AVPixelFormat src_pix_fmt);

AVFrame& convert_mat_to_avframe(SwsContext* swsctx, const cv::Mat& mat, AVPixelFormat dst_pix_fmt);

std::vector<AVFrame*> ffmpeg_send_packet_receive_frames(AVCodecContext* avcodecctx, const AVPacket* avpacket);

std::vector<AVFrame*> ffmpeg_read_frame_send_packet_receive_frames(
	AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream, int64_t pts, int* index);

AVFrame* frame_alloc_copy_props(AVFrame* src);
