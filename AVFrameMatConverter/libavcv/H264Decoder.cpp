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
	if (swsctx != nullptr)
		sws_freeContext(swsctx);
	if (avcodecctx != nullptr)
		avcodec_free_context(&avcodecctx);
	if (avfmtctx != nullptr)
		avformat_close_input(&avfmtctx);
}

std::vector<cv::Mat>  H264Decoder::decode() {

	std::vector<cv::Mat> matVec;

	// Store a decoded frame.
	AVFrame* frame = av_frame_alloc();
	AVPacket avpacket = AVPacket();

	int packet_counter = 0;
	int frame_counter = 0;
	std::cout << std::endl;
	while (av_read_frame(avfmtctx, &avpacket) == 0) {
		//0:OK, <0:EOF又はエラー（エラーの場合はpacketが空）
		if (avpacket.stream_index == avstream->index) {
			if (avcodec_send_packet(avcodecctx, &avpacket) != 0) {
				throw "Failed: avcodec_send_packet";
			}

			packet_counter++;

			swsctx = sws_getCachedContext
			(
				swsctx,
				avstream->codecpar->width,
				avstream->codecpar->height,
				avcodecctx->pix_fmt,
				avstream->codecpar->width,
				avstream->codecpar->height,
				AVPixelFormat::AV_PIX_FMT_BGR24,
				SWS_BICUBIC,
				nullptr,
				nullptr,
				nullptr
			);

			//デコーダに１パケット渡しても（avcodec_send_packet）、必ず１枚のフレームが取得できるとは限らない（avcodec_receive_frame）。
			//Bフレームが使われていると、数フレーム分のパケットが揃って初めて、該当する複数のデコードフレームを受け取れる。
			int ret = 0;
			do {
				//取り出せるフレームが存在する(戻り値0)の間、繰り返す。
				ret = avcodec_receive_frame(avcodecctx, frame);
				//std::cout << "avcodec_receive_frame returned (" << ret << ")" << std::endl;
				if (ret != 0)
					break;

				const cv::String logstr = cv::format("NO=%d pict_type=%c pts=%d pkt_dts=%d",
					frame_counter,
					av_get_picture_type_char(frame->pict_type),frame->pts, frame->pkt_dts);
				std::cout << logstr << std::endl;

				auto mat = convert_avframe_to_mat(swsctx, frame, AVPixelFormat::AV_PIX_FMT_BGR24);
				matVec.push_back(mat);

				frame_counter++;

			} while (true);
		}
		av_packet_unref(&avpacket);
	}

	// nullptrを渡して、デコーダをフラッシュする。
	//std::cout << std::endl;
	//std::cout << "Call 'avcodec_send_packet' with nullptr on avpkt." << std::endl;
	if (avcodec_send_packet(avcodecctx, nullptr) != 0) {
		throw "Failed:: avcodec_send_packet";
	}

	int ret = 0;
	do {
		ret = avcodec_receive_frame(avcodecctx, frame);
		if (ret == 0)
		{
			const cv::String logstr = cv::format("NO=%d pict_type=%c pts=%d pkt_dts=%d",
				frame_counter,
				av_get_picture_type_char(frame->pict_type), frame->pts, frame->pkt_dts);
			std::cout << logstr << std::endl;
			
			frame_counter++;

			//if (!cv::imwrite(bmpfile, mat))
			//{
			//	cerr << "failed to 'imwrite'" << endl;
			//}
		}
	} while (ret == 0);

	av_frame_free(&frame);

	return matVec;
}
