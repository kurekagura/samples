#pragma once

#include <memory>

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

/// <summary>
/// srcframe->formatが設定されていないか、信用できない場合．不要かも．
/// </summary>
/// <param name="srcframe"></param>
/// <param name="src_pix_fmt"></param>
/// <returns></returns>
cv::Mat& convert_avframe_to_mat(AVFrame* srcframe, AVPixelFormat src_pix_fmt);

/// <summary>
/// 呼び出される毎のsws_getContextのオーバーヘッドが懸念．
/// </summary>
/// <param name="srcframe"></param>
/// <returns></returns>
cv::Mat& convert_avframe_to_mat(AVFrame* srcframe);

AVFrame& convert_mat_to_avframe(SwsContext* swsctx, const cv::Mat& mat, AVPixelFormat dst_pix_fmt);

AVFrame& convert_mat_to_avframe(const cv::Mat& mat, AVPixelFormat dst_pix_fmt);

int64_t ffmpeg_frameindex_to_pts(const AVStream* avstream, int64_t frame_index);

std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> ffmpeg_send_packet_receive_frames(AVCodecContext* avcodecctx, const AVPacket* avpacket);

std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> ffmpeg_seek_read_send_receive_frames_by_frameindex(
	AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream, int64_t frame_index, int64_t* index);

std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> ffmpeg_seek_read_send_receive_frames_by_pts(
	AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream, int64_t pts, int64_t* index);

AVFrame* frame_alloc_copy_props(AVFrame* src);
