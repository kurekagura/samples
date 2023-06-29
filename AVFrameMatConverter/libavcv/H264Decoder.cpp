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
/// "h264_qsv"の場合，エンコード前後で結果が異なる（YUV420P→NV12）．
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
/// Seekの過程でデコードされたフレームも戻す．
/// 指定フレームは，戻り値（vector）のインデックスvector_indexに格納されている．
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

	//スタックに１つのみ確保
	AVPacket avpacket;
	//AVPacket* avpakcet = new AVPacket(); //ヒープでもよさそう

	int packet_counter = 0;
	int frame_counter = 0;

	while (av_read_frame(avfmtctx, &avpacket) == 0) {
		//0:OK, <0:EOF又はエラー（エラーの場合はpacketが空）
		if (avpacket.stream_index == avstream->index) {
			if (avcodec_send_packet(avcodecctx, &avpacket) != 0) {
				throw "Failed: avcodec_send_packet";
			}

			packet_counter++;

			//デコーダに１パケット渡しても（avcodec_send_packet）、必ず１枚のフレームが取得できるとは限らない（avcodec_receive_frame）。
			//Bフレームが使われていると、数フレーム分のパケットが揃って初めて、該当する複数のデコードフレームを受け取れる。
			do {
				//ここで確保
				AVFrame* frame = av_frame_alloc();
				//取り出せるフレームが存在する(戻り値0)の間、繰り返す。
				int ret = avcodec_receive_frame(avcodecctx, frame);
				//std::cout << "avcodec_receive_frame returned (" << ret << ")" << std::endl;
				if (ret == 0)
				{
					const cv::String logstr = cv::format("NO=%d pict_type=%c pts=%d pkt_dts=%d %s",
						frame_counter,
						av_get_picture_type_char(frame->pict_type), frame->pts, frame->pkt_dts,
						frame->pict_type == AVPictureType::AV_PICTURE_TYPE_I ? " ●" : ""
					);
					std::cout << logstr << std::endl;

					//std::unique_ptr<AVFrame, deleter_for_AVFrame> pframe(frame);
					//avframeVec.push_back(std::move(pframe));
					avframeVec.push_back(std::unique_ptr<AVFrame, deleter_for_AVFrame>(frame));
					frame_counter++;
				}
				else
				{
					av_frame_free(&frame); //フレームが取得できなかった場合はここで解放
					break;
				}
			} while (true);
		}
		av_packet_unref(&avpacket);
	}
	//av_packet_free(&avpacket);

	// nullptrを渡して、デコーダをフラッシュする。
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

void H264Decoder::dump()
{
	//先頭フレームインデックスへSeek
	//if (av_seek_frame(avfmtctx, avstream->index, 0, AVSEEK_FLAG_FRAME | AVSEEK_FLAG_BACKWARD) < 0)
	//	throw "Failed: av_seek_frame";
	//avcodec_flush_buffers(avcodecctx); //av_seek_frameの直後，バッファをフラッシュ．

	std::vector<int64_t> iframes;
	auto avframeVec = this->decode();
	for (int fidx = 0; fidx < avframeVec.size(); fidx++) {
		auto& frame = avframeVec[fidx];
		if (frame->pict_type == AVPictureType::AV_PICTURE_TYPE_I)
			iframes.push_back(fidx);
		std::cout << cv::format("frame=%d %c", fidx, av_get_picture_type_char(frame->pict_type)) << std::endl;
	}

	std::cout << "I frames : ";
	for (auto iframe : iframes) {
		std::cout << iframe << ",";
	}
	std::cout << std::endl;
}
