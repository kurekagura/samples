/*
* べたに記述したエンコードのコード．
* BMPファイルをOpenCVのMatへ読み込み，AVFrameへ変換している．
* 最終的にmp4を出力する．
*/
#include <memory>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem> //C++17

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "../libavcv/libavcv.h"
#include "../libavcv/H264Decoder.hpp"

namespace fs = std::filesystem;
int main(int argc, char* argv[])
{
	cv::String logstr;

	if (argc < 3) {
		std::cerr << "Error: 第一引数に連番画像ファイルが格納されたディレクトリを指定して下さい。" << std::endl;
		std::cerr << "Error: 第二引数に動画の出力先ディレクトリを指定して下さい(ファイル名は自動生成されます)。" << std::endl;
		return -1;
	}
	const std::string basedir = argv[1];
	const std::string outputDir = argv[2];

	// 連番画像ファイルの一覧取得・ソート
	std::vector<std::string> bmpVec;
	for (const auto& entry : fs::directory_iterator(basedir))
		bmpVec.push_back(entry.path().string());

	sort(bmpVec.begin(), bmpVec.end(), [](std::string& s1, std::string& s2) { return s1 < s2; });

	// 連番画像ファイルをMatに読み込む
	std::vector<cv::Mat> matVec;
	for (const auto& afile : bmpVec) {
		std::cout << afile << std::endl;
		cv::Mat mat = cv::imread(afile);
		if (mat.empty())
		{
			std::cerr << "Failed: Mat is empty." << std::endl;
			return -1;
		}
		matVec.push_back(mat);
	}

	SwsContext* swsctx = nullptr;

	//エンコーダの指定
	//const char* encoder_name = "libx264";		//Requre NV12 or YUV420P
	//const char* encoder_name = "h264_qsv";	//Require NV12
	const char* encoder_name = "h264_nvenc";	//Require NV12

	//Mat（BGRA）からエンコーダが対応している色空間への変換
	// libx264 -> AV_PIX_FMT_YUV420P
	// libx264, h264_qsv, h264_nvenc -> AV_PIX_FMT_NV12
	const AVPixelFormat dst_pix_fmt = AVPixelFormat::AV_PIX_FMT_NV12;

	//未設定の時の値はエンコーダによって異なるよう．libx264(-1),h264_qsv(250),h264_nvenc(250).
	//TODO: h264_nvencだと奇妙な数列になる(0,9,18,...)．fpsも奇妙．
	const int gop = 30;

	//出力するMP4ファイルパスの文字列生成
	std::string outputFileName = cv::format("%s\\out-mat2avframe-%s-%s-gop%d.mp4",
		outputDir.c_str(),
		av_get_pix_fmt_name(dst_pix_fmt),
		encoder_name, gop);

	// 色空間変換を行い、Mat→AVFrameにする。
	// AVFrameは独自のdeletorを定義
	std::vector<std::unique_ptr<AVFrame, deleter_for_AVFrame>> avframeVec;
	for (const auto& amat : matVec) {
		//フレーム毎にサイズがことなったらどうなる？
		swsctx = sws_getCachedContext
		(
			swsctx,
			amat.cols,
			amat.rows,
			AVPixelFormat::AV_PIX_FMT_BGR24, //ソースのMatはRGBではなくBGR
			amat.cols,
			amat.rows,
			dst_pix_fmt,
			SWS_BICUBIC,
			nullptr,
			nullptr,
			nullptr
		);
		// 色空間変換と型変換を行う
		std::unique_ptr<AVFrame, deleter_for_AVFrame> dstframe(&convert_mat_to_avframe(swsctx, amat, dst_pix_fmt));
		avframeVec.push_back(std::move(dstframe));

		//確認用の表示
		cv::imshow("Press 'q' to stop", amat);

		if (cv::waitKey(1) == 'q') {
			std::cout << "Info: Program aborted." << std::endl;
		}
	}

	sws_freeContext(swsctx);

	//以降がエンコード用のコード

	AVIOContext* avioctx = nullptr;
	if (avio_open(&avioctx, outputFileName.c_str(), AVIO_FLAG_WRITE) < 0) {
		std::cerr << "Failed : avio_open" << std::endl;
		return -1;
	}

	AVFormatContext* avfmtctx = nullptr;
	if (avformat_alloc_output_context2(&avfmtctx, nullptr, "mp4", nullptr) < 0) {
		std::cerr << "Failed : avformat_alloc_output_context2" << std::endl;
		return -1;
	}
	avfmtctx->pb = avioctx;

	AVCodec* avcodec = avcodec_find_encoder_by_name(encoder_name);
	if (avcodec == nullptr) {
		std::cerr << "Failed : avcodec_find_encoder" << std::endl;
		return -1;
	}

	AVCodecContext* avcodecctx = avcodec_alloc_context3(avcodec);
	if (avcodecctx == nullptr) {
		std::cerr << "Failed : avcodec_alloc_context3" << std::endl;
		return -1;
	}

	avcodecctx->gop_size = gop;

	avcodecctx->pix_fmt = dst_pix_fmt;
	avcodecctx->width = avframeVec[0]->width;
	avcodecctx->height = avframeVec[0]->height;
	avcodecctx->field_order = AV_FIELD_PROGRESSIVE;
	avcodecctx->color_range = avframeVec[0]->color_range;
	avcodecctx->color_primaries = avframeVec[0]->color_primaries;
	avcodecctx->color_trc = avframeVec[0]->color_trc;
	avcodecctx->colorspace = avframeVec[0]->colorspace;
	avcodecctx->chroma_sample_location = avframeVec[0]->chroma_location;
	avcodecctx->sample_aspect_ratio = avframeVec[0]->sample_aspect_ratio;
	//avcodecctx->profile = FF_PROFILE_H264_BASELINE;
	//avcodecctx->profile = FF_PROFILE_H264_HIGH_444;

	// 30fps 512/15360=0.0333
	avcodecctx->framerate = { 15360 ,512 }; //=30
	avcodecctx->time_base = { 512, 15360 };

	// 60fps 512/30720
	//avcodecctx->framerate = { 30720 ,512 }; //=60
	//avcodecctx->time_base = { 512, 30720 };

	// 「グローバルヘッダ」が存在している場合は、付与する。
	if (avfmtctx->oformat->flags & AVFMT_GLOBALHEADER) {
		avcodecctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
		std::cout << "Info : Set AV_CODEC_FLAG_GLOBAL_HEADER" << std::endl;
	}

	// オプションを指定
	AVDictionary* enc_options = nullptr;
	if (av_dict_set(&enc_options, "preset", "medium", 0) < 0)
		return -1;
	if (av_dict_set(&enc_options, "crf", "22", 0) < 0)
		return -1;
	if (av_dict_set(&enc_options, "profile", "baseline", 0) < 0)
		return -1;
	if (av_dict_set(&enc_options, "level", "4.0", 0) < 0)
		return -1;

	int ret = avcodec_open2(avcodecctx, avcodecctx->codec, &enc_options);
	if (ret != 0) {
		std::cerr << "Failed : avcodec_open2 returned " << ret << std::endl;
		return -1;
	}

	AVStream* avstream = avformat_new_stream(avfmtctx, avcodec);
	if (avstream == nullptr) {
		std::cerr << "Failed : avformat_new_stream" << std::endl;
		return -1;
	}

	//avcodecctx から avstream->codecpar へのコピー
	if (avcodec_parameters_from_context(avstream->codecpar, avcodecctx) < 0) {
		std::cerr << "Failed : avcodec_parameters_from_context" << std::endl;
		return -1;
	}

	// avstream->time_baseの初期値は{0, 0}なので，avcodecctx->time_baseの値を設定．
	// この値は，avformat_write_headerの呼出し後に，num=>1に初期化されるが、avformat_write_headerの後で設定してはダメ．
	// avformat_write_headerの前に設定が必須．
	avstream->sample_aspect_ratio = avcodecctx->sample_aspect_ratio;

	//avformat_write_headerの前に設定が必須．
	avstream->time_base = { 1, 15360 };		//30fps
	//avstream->time_base = { 1,30720 };	//60fps

	logstr = cv::format("avcodecctx->time_base = {%d, %d}\navstream->time_base = {%d, %d}",
		avcodecctx->time_base.num, avcodecctx->time_base.den,
		avstream->time_base.num, avstream->time_base.den);
	std::cout << logstr << std::endl;

	// avformat_write_headerの呼び出しにより，avstream->time_baseが {1,設定値} に書き変わる.numに1以外を設定しても意味がない？
	// 未設定{0,0}の場合は{1, 90000}が設定される.
	if (avformat_write_header(avfmtctx, nullptr) < 0) {
		std::cerr << "Failed : avformat_write_header" << std::endl;
		return -1;
	}

	logstr = cv::format("avcodecctx->time_base = {%d, %d}\navstream->time_base = {%d, %d}",
		avcodecctx->time_base.num, avcodecctx->time_base.den,
		avstream->time_base.num, avstream->time_base.den);
	std::cout << logstr << std::endl;

	//エンコード
	int encoded_count = 0;
	for (int idx = 0; idx < avframeVec.size(); idx++) {
		auto& enc_frame = avframeVec[idx];

		//if (idx % 30 == 0) {
		//	enc_frame->pict_type = AVPictureType::AV_PICTURE_TYPE_I;
		//}

		//ptsは自動では設定されないよう.pkt_dtsは自動で設定されるよう．
		//enc_frame->pts = idx * 512;
		enc_frame->pts = av_rescale_q(idx, avcodecctx->time_base, avstream->time_base); // av_rescale_q(a,bq,cq): a*(bq/cq)

		//avstream->codec->time_base(={0,1})もあるがdeprecated.

		std::cout << "enc_frame->pts=" << enc_frame->pts << " enc_frame->pkt_dts=" << enc_frame->pkt_dts << std::endl;
		if (avcodec_send_frame(avcodecctx, enc_frame.get()) != 0) {
			std::cerr << "Failed: avcodec_send_frame" << std::endl;
			return -1;
		}

		AVPacket pkt;
		if (av_new_packet(&pkt, 0) != 0)
		{
			std::cerr << "Failed: av_new_packet" << std::endl;
			return -1;
		}
		pkt.data = nullptr;
		pkt.size = 0;
		while (avcodec_receive_packet(avcodecctx, &pkt) == 0) {
			pkt.stream_index = 0;
			//↓呼び出すと動画が壊れる
			//av_packet_rescale_ts(&pkt, avcodecctx->time_base, avstream->time_base);
			//エンコード書き込み
			std::cout << "pkt pts=" << pkt.pts << " dts(自動設定)=" << pkt.dts << " duration=" << pkt.duration << std::endl;
			int ret = av_interleaved_write_frame(avfmtctx, &pkt);
			if (ret != 0) {
				std::cerr << "Failed: av_interleaved_write_frame" << std::endl;
				//return -1;
				continue;
			}
			encoded_count++;
			std::cout << "Encoded: " << encoded_count << std::endl;
		}
	}

	// エンコーダをフラッシュ
	if (avcodec_send_frame(avcodecctx, nullptr) != 0) {
		std::cerr << "Failed: avcodec_send_frame(flush)" << std::endl;
		return -1;
	}

	AVPacket pkt;
	if (av_new_packet(&pkt, 0) != 0)
	{
		std::cerr << "Failed: av_new_packet" << std::endl;
		return -1;
	}
	pkt.data = nullptr;
	pkt.size = 0;
	while (avcodec_receive_packet(avcodecctx, &pkt) == 0) {
		pkt.stream_index = 0;
		//↓呼び出すと動画が壊れる
		//av_packet_rescale_ts(&pkt, avcodecctx->time_base, avstream->time_base);
		//エンコード書き込み
		std::cout << "pkt pts=" << pkt.pts << " dts(自動設定)=" << pkt.dts << " duration=" << pkt.duration << std::endl;
		if (av_interleaved_write_frame(avfmtctx, &pkt) != 0) {
			std::cerr << "Failed: av_interleaved_write_frame" << std::endl;
			//return -1;
			continue;
		}
		encoded_count++;
		std::cout << "Encoded: " << encoded_count << std::endl;
	}

	if (av_write_trailer(avfmtctx) != 0) {
		std::cerr << "Failed: av_write_trailer" << std::endl;
		return -1;
	}

	avcodec_free_context(&avcodecctx);
	avformat_free_context(avfmtctx);
	avio_closep(&avioctx);

	// エンコード出力ファイルの検証・再生
	// 利用可能なデコーダ: "h264" or "h264_qsv" or "h264_cuvid"
	auto mydecoder = std::make_unique<H264Decoder>(outputFileName.c_str(), "h264");
	AVPixelFormat src_pix_fmt = mydecoder->pix_fmt(); //デコーダによって値が異なる.YUV420PやNV12．
	auto decodedAVFrameVec = mydecoder->decode();
	AVPixelFormat src_pix_fmt2 = mydecoder->pix_fmt(); //呼び出しタイミングによって結果が異なる．

	SwsContext* swsctx2 = nullptr;

	// To print the list of I frames.
	std::ostringstream oss;
	oss << "Output file : " << outputFileName << std::endl;
	oss << "I Frames : ";

	for (auto it = decodedAVFrameVec.begin(); it != decodedAVFrameVec.end(); ++it) {
		int idx = std::distance(decodedAVFrameVec.begin(), it);

		auto& aframe = *it;
		swsctx2 = sws_getCachedContext
		(
			swsctx2,
			aframe->width,
			aframe->height,
			src_pix_fmt2,
			aframe->width,
			aframe->height,
			AVPixelFormat::AV_PIX_FMT_BGR24,
			SWS_BICUBIC,
			nullptr,
			nullptr,
			nullptr
		);

		auto amat = convert_avframe_to_mat(swsctx2, aframe.get(), AVPixelFormat::AV_PIX_FMT_BGR24);

		if (aframe->pict_type == AVPictureType::AV_PICTURE_TYPE_I)
			oss << idx << ",";

		//確認用の表示
		cv::imshow("Validating: Press 'q' to stop", amat);

		if (cv::waitKey(1) == 'q') {
			std::cout << "Info: Program aborted." << std::endl;
			break;
		}
	}

	oss << std::flush;
	std::cout << oss.str() << std::endl;

	sws_freeContext(swsctx2);

	return 0;
}

