#pragma warning(disable : 26812)

#include <memory>
#include <iostream>
#include <opencv2/opencv.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "../libavcv/libavcv.h"

int main(int argc, char* argv[])
{
	//if set nullptr, search a proper one.
	const char* decoder_name = "h264_qsv"; // "h264_qsv";

	if (argc < 3) {
		std::cerr << "Error: Specify the path to movie file on 1st." << std::endl;
		std::cerr << "Error: Specify directory(no trailing slash) to output bitmap files." << std::endl;
		return -1;
	}
	const char* movie_file_path = argv[1];
	const char* output_dir = argv[2];

	AVFormatContext* avfmtctx = nullptr;
	if (avformat_open_input(&avfmtctx, movie_file_path, nullptr, nullptr) != 0) {
		std::cerr << "Failed: avformat_open_input" << std::endl;
		return -1;
	}

	if (avformat_find_stream_info(avfmtctx, nullptr) < 0) {
		std::cerr << "Failed: avformat_find_stream_info" << std::endl;
		return -1;
	}

	AVStream* avstream = nullptr;
	for (int i = 0; i < (int)avfmtctx->nb_streams; ++i) {
		if (avfmtctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
			std::cout << "Info: Found AVMEDIA_TYPE_VIDEO." << std::endl;
			avstream = avfmtctx->streams[i];
			break;
		}
	}
	if (avstream == nullptr) {
		std::cerr << "Error: Not found any video stream." << std::endl;
		return -1;
	}

	if (avstream->codecpar->codec_id == AVCodecID::AV_CODEC_ID_H264) {
		std::cout << "Info: codec_id = " << avstream->codecpar->codec_id
			<< "(" << avcodec_get_name(avstream->codecpar->codec_id) << ")" << std::endl; //->codec_id = 27(h264)
	}
	else
	{
		std::cout << "Failed: No implementation other than h264." << std::endl;
		return -1;
	}

	AVCodec* avcodec = nullptr;
	if (decoder_name == nullptr)
		avcodec = avcodec_find_decoder(avstream->codecpar->codec_id);
	else
		avcodec = avcodec_find_decoder_by_name(decoder_name);

	if (avcodec == nullptr) {
		std::cerr << "Failed: Can NOT find any decoders." << std::endl;
		return -1;
	}

	AVCodecContext* avcodecctx = avcodec_alloc_context3(avcodec);
	if (avcodecctx == nullptr) {
		std::cerr << "Failed: avcodec_alloc_context3" << std::endl;
		return -1;
	}

	if (avcodec_parameters_to_context(avcodecctx, avstream->codecpar) < 0) {
		std::cerr << "Failed: avcodec_parameters_to_context" << std::endl;
		return -1;
	}

	if (avcodec_open2(avcodecctx, avcodec, nullptr) != 0) {
		std::cerr << "Failed: avcodec_open2" << std::endl;
		return -1;
	}

	//Because Mat requires BGR.
	const AVPixelFormat dst_pix_fmt = AV_PIX_FMT_BGR24;

	// If srcFormat is NV12, fail.
	// https://stackoverflow.com/questions/56409122/swscale-image-patch-nv12-color-conversion-invalid-border
	// SWS_POINT | SWS_BITEXACT
	// h264_qsvでは、avcodecctx->pix_fmtはavcodec_send_packet呼出し後にNV12に設定される（この段階ではYUV420pになっている）。
	// その為、ここでsws_getContextするのではなく、後でフレーム復元の度にsws_getCachedContextをする。
	SwsContext* swsctx = nullptr;
	//SwsContext* swsctx = sws_getContext
	//(
	//	avstream->codecpar->width,
	//	avstream->codecpar->height,
	//	AV_PIX_FMT_NV12,
	//	avstream->codecpar->width,
	//	avstream->codecpar->height,
	//	dst_pix_fmt,
	//	SWS_BICUBIC,
	//	nullptr,
	//	nullptr,
	//	nullptr
	//);

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
				std::cerr << "Failed: avcodec_send_packet" << std::endl;
				return -1;
			}

			packet_counter++;
			std::cout << "'av_read_frame' read one pakect, then 'avcodec_send_packet':" << packet_counter << std::endl;

			swsctx = sws_getCachedContext
			(
				swsctx,
				avstream->codecpar->width,
				avstream->codecpar->height,
				avcodecctx->pix_fmt,
				avstream->codecpar->width,
				avstream->codecpar->height,
				dst_pix_fmt,
				SWS_BICUBIC,
				nullptr,
				nullptr,
				nullptr
			);

			//デコーダに１パケット渡しても（avcodec_send_packet）、必ず１枚のフレームが取得できるとは限らない（avcodec_receive_frame）。
			//Bフレームが使われていると、数フレーム分のパケットが揃って初めて、該当する複数のデコードフレームを受け取れる。
			do {
				//取り出せるフレームが存在する(戻り値0)の間、繰り返す。
				int ret = avcodec_receive_frame(avcodecctx, frame);
				std::cout << "avcodec_receive_frame returned (" << ret << ")" << std::endl;
				if (ret != 0)
					break;

				std::cout << "frame->pict_type=" << av_get_picture_type_char(frame->pict_type) << std::endl;
				std::cout << "Info: Decoded frame(" << frame_counter << ") PTS:" << frame->pts << std::endl;

				auto mat = convert_avframe_to_mat(swsctx, frame, dst_pix_fmt);
				const cv::String bmpfile = cv::format("%s\\%03d.bmp", output_dir, frame_counter);
				frame_counter++;

				if (!cv::imwrite(bmpfile, mat))
				{
					std::cerr << "failed to 'imwrite'" << std::endl;
				}
			} while (true);
			std::cout << std::endl;
		}
		av_packet_unref(&avpacket);
	}

	// nullptrを渡して、デコーダをフラッシュする。
	std::cout << std::endl;
	std::cout << "Call 'avcodec_send_packet' with nullptr on avpkt." << std::endl;
	if (avcodec_send_packet(avcodecctx, nullptr) != 0) {
		std::cerr << "Failed:: avcodec_send_packet" << std::endl;
		return -1;
	}

	do {
		int ret = avcodec_receive_frame(avcodecctx, frame);
		std::cout << "avcodec_receive_frame returned (" << ret << ")" << std::endl;
		if (ret != 0)
			break;

		std::cout << "frame->pict_type=" << av_get_picture_type_char(frame->pict_type) << std::endl;
		std::cout << "Info: Decoded frame(" << frame_counter << ") PTS:" << frame->pts << std::endl;

		auto mat = convert_avframe_to_mat(swsctx, frame, dst_pix_fmt);
		const cv::String bmpfile = cv::format("%s\\%03d.bmp", output_dir, frame_counter);
		frame_counter++;

		if (!cv::imwrite(bmpfile, mat))
		{
			std::cerr << "failed to 'imwrite'" << std::endl;
		}
	} while (true);

	sws_freeContext(swsctx);
	av_frame_free(&frame);
	avcodec_free_context(&avcodecctx);
	avformat_close_input(&avfmtctx);

	return 0;
}



