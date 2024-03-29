﻿#include <memory>
#include "libavcv.h"

AVFrame& convert_mat_to_avframe(SwsContext* swsctx, const cv::Mat& mat, AVPixelFormat dst_pix_fmt) {
	//src_pix_fmtはmatに依存
	//CV_U8C3(BGR) -> AV_PIX_FMT_BGR24, or NotImplemented.
	//Point: share databuf in srcframe and mat;
	AVFrame* srcframe = av_frame_alloc();
	int reqsize_src = av_image_fill_arrays(srcframe->data, srcframe->linesize, (uint8_t*)mat.data, AVPixelFormat::AV_PIX_FMT_BGR24, mat.cols, mat.rows, 32);

	//shared with mat.
	int buffsize = av_image_get_buffer_size(dst_pix_fmt, mat.cols, mat.rows, 32);
	uint8_t* databuf = (uint8_t*)av_malloc(buffsize);

	AVFrame* dstframe = av_frame_alloc();
	int reqsize_dst = av_image_fill_arrays(dstframe->data, dstframe->linesize, databuf, dst_pix_fmt, mat.cols, mat.rows, 32);

	//Convert coloarspace, avframe(shared with Mat) to avframe.
	int res = sws_scale(swsctx, srcframe->data, srcframe->linesize, 0, mat.rows, dstframe->data, dstframe->linesize);
	if (res == 0) {
		std::cerr << "Failed to 'sws_scale'" << std::endl;
	}
	else {
		dstframe->width = mat.cols;
		dstframe->height = mat.rows;
		dstframe->format = dst_pix_fmt;
	}

	av_frame_free(&srcframe);
	return *dstframe;
}

AVFrame& convert_mat_to_avframe(const cv::Mat& mat, AVPixelFormat dst_pix_fmt) {

	SwsContext *swsctx = sws_getContext
	(
		mat.cols,
		mat.rows,
		AVPixelFormat::AV_PIX_FMT_BGR24,
		mat.cols,
		mat.rows,
		dst_pix_fmt,
		SWS_BICUBIC,
		nullptr,
		nullptr,
		nullptr
	);
	//src_pix_fmtはmatに依存
	//CV_U8C3(BGR) -> AV_PIX_FMT_BGR24, or NotImplemented.
	//Point: share databuf in srcframe and mat;
	AVFrame* srcframe = av_frame_alloc();
	int reqsize_src = av_image_fill_arrays(srcframe->data, srcframe->linesize, (uint8_t*)mat.data, AVPixelFormat::AV_PIX_FMT_BGR24, mat.cols, mat.rows, 32);

	//shared with mat.
	int buffsize = av_image_get_buffer_size(dst_pix_fmt, mat.cols, mat.rows, 32);
	uint8_t* databuf = (uint8_t*)av_malloc(buffsize);

	AVFrame* dstframe = av_frame_alloc();
	int reqsize_dst = av_image_fill_arrays(dstframe->data, dstframe->linesize, databuf, dst_pix_fmt, mat.cols, mat.rows, 32);

	//Convert coloarspace, avframe(shared with Mat) to avframe.
	int res = sws_scale(swsctx, srcframe->data, srcframe->linesize, 0, mat.rows, dstframe->data, dstframe->linesize);
	if (res == 0) {
		std::cerr << "Failed to 'sws_scale'" << std::endl;
	}
	else {
		dstframe->width = mat.cols;
		dstframe->height = mat.rows;
		dstframe->format = dst_pix_fmt;
	}

	av_frame_free(&srcframe);
	sws_freeContext(swsctx);
	return *dstframe;
}

//TODO:dst_pix_fmtはBGR24のみしか対応できないのであれば削除
//swsctxも内部で生成すべき？
cv::Mat& convert_avframe_to_mat(SwsContext* swsctx, AVFrame* srcframe, AVPixelFormat dst_pix_fmt)
{
	int buffsize = av_image_get_buffer_size(dst_pix_fmt, srcframe->width, srcframe->height, 32);
	//Who release it?
	uint8_t* databuf = (uint8_t*)av_malloc(buffsize);

	//Point: share databuf in dstframe and mat;
	AVFrame* dstframe = av_frame_alloc();
	//3rd: const ​​​uint8_t​ *​​‌src​
	int reqsize = av_image_fill_arrays(dstframe->data, dstframe->linesize, databuf, dst_pix_fmt, srcframe->width, srcframe->height, 32);

	//Convert coloarspace, avframe to frame(shared with Mat).
	//If srcFormat is NV12, this fails.
	//https://stackoverflow.com/questions/16112063/how-can-i-use-ffmpegs-swscale-to-convert-from-nv12-to-rgb32
	int res = sws_scale(swsctx, srcframe->data, srcframe->linesize, 0, srcframe->height, dstframe->data, dstframe->linesize);
	if (res == 0) {
		std::cerr << "Failed to 'sws_scale'" << std::endl;
	}

	//void*, Mat will release databuf?
	cv::Mat* mat = new cv::Mat(srcframe->height, srcframe->width, CV_8UC3, databuf, dstframe->linesize[0]);

	//const cv::String bmpfile = "C:\\dev\\samplevideo\\out-avframe2mat\\test.bmp";
	//cv::imwrite(bmpfile, *mat);

	av_frame_free(&dstframe);

	return *mat;
}

cv::Mat& convert_avframe_to_mat(AVFrame* srcframe, AVPixelFormat src_pix_fmt)
{
	AVPixelFormat dst_pix_fmt = AVPixelFormat::AV_PIX_FMT_BGR24;

	SwsContext* swsctx = sws_getContext
	(
		srcframe->width,
		srcframe->height,
		src_pix_fmt,
		srcframe->width,
		srcframe->height,
		AVPixelFormat::AV_PIX_FMT_BGR24,
		SWS_BICUBIC,
		nullptr,
		nullptr,
		nullptr
	);

	int buffsize = av_image_get_buffer_size(dst_pix_fmt, srcframe->width, srcframe->height, 32);
	//Who release it?
	uint8_t* databuf = (uint8_t*)av_malloc(buffsize);

	//Point: share databuf in dstframe and mat;
	AVFrame* dstframe = av_frame_alloc();
	//3rd: const ​​​uint8_t​ *​​‌src​
	int reqsize = av_image_fill_arrays(dstframe->data, dstframe->linesize, databuf, dst_pix_fmt, srcframe->width, srcframe->height, 32);

	//Convert coloarspace, avframe to frame(shared with Mat).
	//If srcFormat is NV12, this fails.
	//https://stackoverflow.com/questions/16112063/how-can-i-use-ffmpegs-swscale-to-convert-from-nv12-to-rgb32
	int res = sws_scale(swsctx, srcframe->data, srcframe->linesize, 0, srcframe->height, dstframe->data, dstframe->linesize);
	if (res == 0) {
		std::cerr << "Failed to 'sws_scale'" << std::endl;
	}

	//void*, Mat will release databuf?
	cv::Mat* mat = new cv::Mat(srcframe->height, srcframe->width, CV_8UC3, databuf, dstframe->linesize[0]);

	av_frame_free(&dstframe);
	sws_freeContext(swsctx);
	return *mat;
}

cv::Mat& convert_avframe_to_mat(AVFrame* srcframe) {
	AVPixelFormat src_pix_fmt = (AVPixelFormat)srcframe->format;
	return convert_avframe_to_mat(srcframe, src_pix_fmt);
}
/// <summary>
/// AVStreamのr_frame_rateとtime_baseを用いて，フレームインデックスからPTSを算出する．
/// </summary>
/// <param name="avstream"></param>
/// <param name="frame_index"></param>
/// <returns></returns>
int64_t ffmpeg_frameindex_to_pts(const AVStream* avstream, int64_t frame_index)
{
	AVRational time_base_basedon_r_frame_rate = av_inv_q(avstream->r_frame_rate);
	int64_t pts = av_rescale_q(frame_index, time_base_basedon_r_frame_rate, avstream->time_base);
	return pts;
}

/// <summary>
/// avcodec_send_packetとavcodec_receive_frameを利用するルーティンを関数化
/// </summary>
/// <param name="avcodecctx"></param>
/// <param name="avpacket"></param>
/// <returns></returns>
std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> ffmpeg_send_packet_receive_frames(
	AVCodecContext* avcodecctx, const AVPacket* avpacket)
{

	if (avcodec_send_packet(avcodecctx, avpacket) != 0)
		throw "Failed: avcodec_send_packet";

	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> avframeVec;

	AVFrame* frame = av_frame_alloc();
	//デコーダに１パケット渡しても（avcodec_send_packet）、必ず１枚のフレームが取得できるとは限らない（avcodec_receive_frame）。
	//Bフレームが使われていると、数フレーム分のパケットが揃って初めて、該当する複数のデコードフレームを受け取れる。
	do {
		//取り出せるフレームが存在する(戻り値0)の間、繰り返す。
		if (avcodec_receive_frame(avcodecctx, frame) != 0)
			break;
		else {
			AVFrame* new_ref = av_frame_alloc();
			av_frame_ref(new_ref, frame);
			avframeVec.push_back(std::unique_ptr<AVFrame, deleter_for_AVFrame>(new_ref));
		}
	} while (true);

	av_frame_free(&frame);
	return avframeVec;
}

/// <summary>
/// 内部実装はffmpeg_seek_read_send_receive_frames_by_ptsと類似．
/// </summary>
/// <param name="avcodecctx"></param>
/// <param name="avfmtctx"></param>
/// <param name="avstream"></param>
/// <param name="frame_index"></param>
/// <param name="index"></param>
/// <returns></returns>
std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> ffmpeg_seek_read_send_receive_frames_by_frameindex(
	AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream, int64_t frame_index, int64_t* index)
{
	*index = -1;

	//指定されたフレームIDXのPTS．但し，存在するとは限らない？
	//coded_picture_numberやdisplay_picture_numberが使えないので導入．
	//フレームIDXで指定しているのに，AVFrameなど，どこにも記録されていないので回避策としてPTSと比較する．
	int64_t expected_pts = ffmpeg_frameindex_to_pts(avstream, frame_index);

	return ffmpeg_seek_read_send_receive_frames_by_pts(avcodecctx, avfmtctx, avstream, expected_pts, index);

	//以下コードでは，GOPを越えてデコード取得してしまう問題がある．
	/*
	// AVSEEK_FLAG_FRAMEのみは，次のIフレームになるので注意．
	if (av_seek_frame(avfmtctx, avstream->index, frame_index, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD) < 0)
		throw "Failed: av_seek_frame";

	avcodec_flush_buffers(avcodecctx); //av_seek_frameの直後，バッファをフラッシュ．

	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> avframeVec;

	AVPacket avpacket = AVPacket();

	bool break_read_frame = false;
	while (break_read_frame == false && av_read_frame(avfmtctx, &avpacket) == 0) {
		if (avpacket.stream_index != avstream->index) {
			av_packet_unref(&avpacket);
			continue; //対象ビデオストリーム以外は無視
		}

		auto tmpVec = ffmpeg_send_packet_receive_frames(avcodecctx, &avpacket);

		for (auto& frame : tmpVec) {
			if (frame->pts == expected_pts)
				*index = avframeVec.size();	//目的のフレーム
			else if (frame->pts > expected_pts)
				break_read_frame = true;		//PTSが過ぎたので停止．

			avframeVec.push_back(std::move(frame)); //moveはここ
		}
		av_packet_unref(&avpacket);
	}

	//readが終わった，もしくは終えた後のフラッシュ
	auto residue = ffmpeg_send_packet_receive_frames(avcodecctx, nullptr);
	for (int i = 0; i < residue.size(); i++) {
		auto& frame = residue[i];
		if (frame->pts == expected_pts)
			*index = avframeVec.size();		//目的のフレーム
		avframeVec.push_back(std::move(frame));
	}

	return avframeVec;
	*/
}

/// <summary>
/// Iフレームに至る過程でデコードされた全フレームも戻す仕様．
/// 内部で最初にav_seek_frameしている。
/// その内，ターゲットのフレームをindexで示す．Iteratorを戻した方が良い？
/// </summary>
/// <param name="avcodecctx"></param>
/// <param name="avfmtctx"></param>
/// <param name="avstream"></param>
/// <param name="pts"></param>
/// <param name="index">呼び出し直後，-1に初期化されまる．</param>
/// <returns></returns>
std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> ffmpeg_seek_read_send_receive_frames_by_pts(
	AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream, int64_t pts, int64_t* index)
{
	*index = -1;
	// Seek by PTS
	if (av_seek_frame(avfmtctx, avstream->index, pts, AVSEEK_FLAG_BACKWARD) < 0)
		throw "Failed: av_seek_frame";

	avcodec_flush_buffers(avcodecctx); //av_seek_frameの直後，バッファをフラッシュ．

	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> avframeVec;

	AVPacket avpacket = AVPacket();

	bool break_read_frame = false;
	while (break_read_frame == false && av_read_frame(avfmtctx, &avpacket) == 0) {
		if (avpacket.stream_index != avstream->index) {
			av_packet_unref(&avpacket);
			continue; //対象ビデオストリーム以外は無視
		}

		auto tmpVec = ffmpeg_send_packet_receive_frames(avcodecctx, &avpacket);

		for (auto& frame : tmpVec) {
			if (frame->pts == pts)
				*index = avframeVec.size();	//目的のフレーム
			else if (frame->pts > pts)
				break_read_frame = true;		//PTSが過ぎたので停止．

			avframeVec.push_back(std::move(frame));	//moveはここ
		}
		av_packet_unref(&avpacket);
	}

	//readが終わった，もしくは終えた後のフラッシュ
	auto residue = ffmpeg_send_packet_receive_frames(avcodecctx, nullptr);
	for (int i = 0; i < residue.size(); i++) {
		auto& frame = residue[i];
		if (frame->pts == pts)
			*index = avframeVec.size();
		avframeVec.push_back(std::move(frame));
	}

	return avframeVec;
}

AVFrame* frame_alloc_copy_props(AVFrame* src) {
	AVFrame* dst = av_frame_alloc();

	av_frame_copy_props(dst, src);

	dst->format = src->format;
	dst->width = src->width;
	dst->height = src->height;

	// Alloc buffer.
	if (av_frame_get_buffer(dst, 32) != 0) {
		throw "av_frame_get_buffer";
	}

	return dst;
}

//AVPixelFormat extract_pix_fmt() {
//
//	return AVPixelFormat::AV_PIX_FMT_YUV420P;
//}