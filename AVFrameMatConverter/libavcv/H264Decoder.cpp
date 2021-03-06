#include "libavcv.h"
#include "H264Decoder.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <opencv2/opencv.hpp>
#include <iostream>

/// <summary>
/// 
/// </summary>
/// <param name="mp4_path"></param>
/// <param name="decoder_name">"h264" | "h264_qsv" | "h264_cuvid"</param>
H264Decoder::H264Decoder(const char* mp4_path, const char* decoder_name) {

	//AVFormatContext* avfmtctx = nullptr;
	if (avformat_open_input(&avfmtctx, mp4_path, nullptr, nullptr) != 0)
		throw "Failed: avformat_open_input";

	if (avformat_find_stream_info(avfmtctx, nullptr) < 0)
		throw "Failed: avformat_find_stream_info";

	//AVStream* avstream = nullptr;
	for (int i = 0; i < (int)avfmtctx->nb_streams; ++i) {
		if (avfmtctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			std::cout << "Info: Found AVMEDIA_TYPE_VIDEO." << std::endl;
			avstream = avfmtctx->streams[i];
			break;
		}
	}
	if (avstream == nullptr)
		throw "Error: Not found any video stream.";

	if (avstream->codecpar->codec_id == AVCodecID::AV_CODEC_ID_H264) {
		std::cout << "Info: codec_id = " << avstream->codecpar->codec_id
			<< "(" << avcodec_get_name(avstream->codecpar->codec_id) << ")" << std::endl; //->codec_id = 27(h264)
	}
	else
		throw "Failed: No implementation other than h264.";

	//AVCodec* avcodec = nullptr;
	if (decoder_name == nullptr)
		avcodec = avcodec_find_decoder(avstream->codecpar->codec_id);
	else
		avcodec = avcodec_find_decoder_by_name(decoder_name);

	if (avcodec == nullptr)
		throw "Failed: Can NOT find any decoders.";

	//AVCodecContext* avcodecctx = avcodec_alloc_context3(avcodec);
	avcodecctx = avcodec_alloc_context3(avcodec);
	if (avcodecctx == nullptr)
		throw "Failed: avcodec_alloc_context3";

	if (avcodec_parameters_to_context(avcodecctx, avstream->codecpar) < 0)
		throw "Failed: avcodec_parameters_to_context";

	if (avcodec_open2(avcodecctx, avcodec, nullptr) != 0)
		throw "Failed: avcodec_open2";
}

H264Decoder::~H264Decoder() {
	if (avcodecctx != nullptr)
		avcodec_free_context(&avcodecctx);
	if (avfmtctx != nullptr)
		avformat_close_input(&avfmtctx);
}

/// <summary>
/// "h264_qsv"???????C?G???R?[?h?O?????????????????iYUV420P??NV12?j?D
/// </summary>
/// <returns></returns>
AVPixelFormat H264Decoder::pix_fmt() {
	if (avcodecctx == nullptr)
		throw "avcodecctx is null.";

	return avcodecctx->pix_fmt;
}

int64_t H264Decoder::nb_frames() {
	return avstream->nb_frames;
}

/// <summary>
/// Seek?????????f?R?[?h???????t???[?????????D
/// ?w???t???[?????C?????l?ivector?j???C???f?b?N?Xvector_index???i?[???????????D
/// </summary>
/// <param name="frame_index"></param>
/// <param name="vector_index"></param>
/// <returns></returns>
std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> H264Decoder::seek_frame(int64_t frame_index, int64_t* vector_index) {
	return ffmpeg_seek_read_send_receive_frames_by_frameindex(avcodecctx, avfmtctx, avstream, frame_index, vector_index);
}

std::unique_ptr<AVFrame, deleter_for_AVFrame> H264Decoder::seek_frame(int64_t frame_index) {
	int64_t target_idx;
	auto avframeVec = ffmpeg_seek_read_send_receive_frames_by_frameindex(avcodecctx, avfmtctx, avstream, frame_index, &target_idx);
	int64_t test = 1;
	if (target_idx != -1)
		return std::move(avframeVec[target_idx]);
	else
		return nullptr;
}

std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> H264Decoder::decode() {
	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> avframeVec;

	//?X?^?b?N???P???????m??
	AVPacket avpacket;
	//AVPacket* avpakcet = new AVPacket(); //?q?[?v????????????

	int packet_counter = 0;
	int frame_counter = 0;

	while (av_read_frame(avfmtctx, &avpacket) == 0) {
		//0:OK, <0:EOF?????G???[?i?G???[????????packet?????j
		if (avpacket.stream_index == avstream->index) {
			if (avcodec_send_packet(avcodecctx, &avpacket) != 0) {
				throw "Failed: avcodec_send_packet";
			}

			packet_counter++;

			//?f?R?[?_???P?p?P?b?g?n???????iavcodec_send_packet?j?A?K???P?????t???[???????????????????????????iavcodec_receive_frame?j?B
			//B?t???[?????g?????????????A???t???[???????p?P?b?g???????????????A?Y?????????????f?R?[?h?t???[???????????????B
			do {
				//???????m??
				AVFrame* frame = av_frame_alloc();
				//?????o?????t???[????????????(?????l0)?????A?J???????B
				int ret = avcodec_receive_frame(avcodecctx, frame);
				//std::cout << "avcodec_receive_frame returned (" << ret << ")" << std::endl;
				if (ret == 0)
				{
					const cv::String logstr = cv::format("NO=%d pict_type=%c pts=%d pkt_dts=%d %s",
						frame_counter,
						av_get_picture_type_char(frame->pict_type), frame->pts, frame->pkt_dts,
						frame->pict_type == AVPictureType::AV_PICTURE_TYPE_I ? " ??" : ""
					);
					std::cout << logstr << std::endl;

					//std::unique_ptr<AVFrame, deleter_for_AVFrame> pframe(frame);
					//avframeVec.push_back(std::move(pframe));
					avframeVec.push_back(std::unique_ptr<AVFrame, deleter_for_AVFrame>(frame));
					frame_counter++;
				}
				else
				{
					av_frame_free(&frame); //?t???[????????????????????????????????????
					break;
				}
			} while (true);
		}
		av_packet_unref(&avpacket);
	}
	//av_packet_free(&avpacket);

	// nullptr???n?????A?f?R?[?_???t???b?V???????B
	//std::cout << std::endl;
	//std::cout << "Call 'avcodec_send_packet' with nullptr on avpkt." << std::endl;
	if (avcodec_send_packet(avcodecctx, nullptr) != 0) {
		throw "Failed:: avcodec_send_packet";
	}

	do {
		// Store a decoded frame.
		AVFrame* frame = av_frame_alloc();
		int ret = avcodec_receive_frame(avcodecctx, frame);
		if (ret == 0)
		{
			const cv::String logstr = cv::format("NO=%d pict_type=%c pts=%d pkt_dts=%d",
				frame_counter,
				av_get_picture_type_char(frame->pict_type), frame->pts, frame->pkt_dts);
			std::cout << logstr << std::endl;

			avframeVec.push_back(std::unique_ptr<AVFrame, deleter_for_AVFrame>(frame));
			frame_counter++;
		}
		else
		{
			av_frame_free(&frame);
			break;
		}
	} while (true);

	return avframeVec;
}

std::vector<cv::Mat>  H264Decoder::decode_to_mat() {

	SwsContext* swsctx = nullptr;

	std::vector<cv::Mat> matVec;

	auto avframeVec = this->decode();

	for (auto& avframe : avframeVec) {
		swsctx = sws_getCachedContext
		(
			swsctx,
			avframe->width,
			avframe->height,
			avcodecctx->pix_fmt,
			avframe->width,
			avframe->height,
			AVPixelFormat::AV_PIX_FMT_BGR24,
			SWS_BICUBIC,
			nullptr,
			nullptr,
			nullptr
		);

		auto mat = convert_avframe_to_mat(swsctx, avframe.get(), AVPixelFormat::AV_PIX_FMT_BGR24);
		matVec.push_back(mat);
	}

	sws_freeContext(swsctx);
	return matVec;
}
