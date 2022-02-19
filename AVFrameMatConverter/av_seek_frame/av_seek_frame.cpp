#pragma warning (disable:26812)
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "../libavcv/libavcv.h"

using std::cout;
using std::cerr;
using std::endl;

AVRational ffmpeg_load_time_base(AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream) {
	//avfmtctx->pb->pos == 先頭の時のみに呼び出し可能（但し先頭は0ではない）．
	//if (avfmtctx->pb->pos != 0)
	//	throw "avfmtctx->pb->pos != 0";
	if (avstream->first_dts != avstream->cur_dts)
		throw "Must call this function when the current DTS of AVStream is the first.";

	//復帰のために保存．
	//int64_t cur_dts = avstream->cur_dts;
	int64_t cur_pos = avfmtctx->pb->pos;

	//事前に先頭フレームへseek．
	//if (av_seek_frame(avfmtctx, avstream->index, 0, AVSEEK_FLAG_FRAME) < 0)
	//	throw "Failed: av_seek_frame";

	//avcodec_flush_buffers(avcodecctx); //Seek後にフラッシュ

	AVPacket avpacket;
	bool is_one_frame_decoded = false;
	while (is_one_frame_decoded == false && av_read_frame(avfmtctx, &avpacket) == 0) {
		//if (avpacket.stream_index != avstream->index) {
		//	av_packet_unref(&avpacket);
		//	throw "Invalid AVStream"; //この関数はビデオストリーム以外では呼び出してはいけない
		//}

		if (avcodec_send_packet(avcodecctx, &avpacket) != 0)
		{
			av_packet_unref(&avpacket);
			throw "Failed: avcodec_send_packet";
		}

		// avcodec_send_packetの呼出し後に初めて正しいtime_baseが取得できるようになる．
		// このtime_baseがStream内に記録されている為と考えられる．その為，実際のパケットをSendする必要がある（nullptrではNG）．
		// ここでは，安全を取って１フレーム分が復号できるまで，パケットをRead/Sendする．
		AVFrame* frame = av_frame_alloc();
		//取り出せるフレームが存在する間，戻り値0．
		if (avcodec_receive_frame(avcodecctx, frame) == 0)
			is_one_frame_decoded = true; //１フレーム取り出せれば終了．

		av_frame_free(&frame);
		av_packet_unref(&avpacket);
	}

	if (is_one_frame_decoded == false)
		throw "Can NOT get time_base because none of the frames are present.";

	//先頭に戻しておく
	//if (av_seek_frame(avfmtctx, avstream->index, 0, AVSEEK_FLAG_FRAME) < 0)
	//	throw "Failed: av_seek_frame";
	if (av_seek_frame(avfmtctx, avstream->index, 0, AVSEEK_FLAG_FRAME) < 0)
		throw "Failed: av_seek_frame";
	avcodec_flush_buffers(avcodecctx); //Seek後にフラッシュ

	//int64_t result_pos0 = avio_seek(avfmtctx->pb, cur_pos, 0);
	//if (result_pos0 != cur_pos)
	//	throw "Failed: avio_seek";

	return avcodecctx->time_base;
}

void test_read_allframes(AVCodecContext* avcodecctx, const AVStream* avstream, AVFormatContext* avfmtctx) {
	cv::String logstr;

	AVPacket avpacket = AVPacket();
	int frame_counter = 0;
	int pkt_counter = 0;

	while (av_read_frame(avfmtctx, &avpacket) == 0) {
		if (avpacket.stream_index != avstream->index) {
			av_packet_unref(&avpacket);
			continue; //対象ビデオストリーム以外は無視
		}
		pkt_counter++;

		logstr = cv::format("AVPacket: count=%3d dts=%6d pts=%6d %s",
			pkt_counter, avpacket.dts, avpacket.pts, avpacket.dts == avpacket.pts ? "" : "(!=)");
		cout << logstr << endl;

		std::vector<AVFrame*> avframeVec = ffmpeg_send_packet_receive_frames(avcodecctx, &avpacket);

		for (auto& frame : avframeVec) {
			logstr = cv::format("Info: Frame=%6d pts=%6d dts=%6d pict_type=%c pts %s dts",
				frame_counter, frame->pts, frame->pkt_dts, av_get_picture_type_char(frame->pict_type),
				frame->pts == frame->pkt_dts ? "==" : "!="
			);
			cout << logstr << endl;
			frame_counter++;

			auto mat = convert_avframe_to_mat(frame, avcodecctx->pix_fmt);
			cv::imshow("Validating", mat);
			cv::waitKey(1);
		}

		av_packet_unref(&avpacket);
	}

	// nullptrを渡して、デコーダをフラッシュする。
	cout << "Flush encoder." << endl;
	std::vector<AVFrame*> avframeVec = ffmpeg_send_packet_receive_frames(avcodecctx, nullptr);
	for (auto& frame : avframeVec) {
		logstr = cv::format("Info: Frame=%6d pts=%6d dts=%6d pict_type=%c pts %s dts",
			frame_counter, frame->pts, frame->pkt_dts, av_get_picture_type_char(frame->pict_type),
			frame->pts == frame->pkt_dts ? "==" : "!="
		);
		cout << logstr << endl;
		frame_counter++;

		auto mat = convert_avframe_to_mat(frame, avcodecctx->pix_fmt);
		cv::imshow("Validating", mat);
		cv::waitKey(1);
	}
}

void test_read_specifidframe(AVCodecContext* avcodecctx, AVFormatContext* avfmtctx, const AVStream* avstream,
	const std::vector<int64_t>& pts_list)
{
	cv::String logstr;

	for (int64_t pts : pts_list) {

		int target_idx;
		std::vector<AVFrame*> avframeVec = ffmpeg_seek_frame_read_frame_send_packet_receive_frames(avcodecctx, avfmtctx, avstream, pts, &target_idx);

		auto& frame = avframeVec[target_idx];
		logstr = cv::format("Info: pts=%6d dts=%6d pict_type=%c %d/%d", 
			frame->pts, frame->pkt_dts, av_get_picture_type_char(frame->pict_type), target_idx, avframeVec.size());
		cout << logstr << endl;

		auto mat = convert_avframe_to_mat(frame, avcodecctx->pix_fmt);
		cv::imshow("Validating", mat);
		cv::waitKey(1);
	}
}

int main(int argc, char* argv[])
{
	cv::String logstr;
	if (argc < 2) {
		cerr << "Error: 第一引数にMP4(H.264)ファイルを指定して下さい。" << endl;
		return -1;
	}
	const char* movie_file_path = argv[1];

	//if set nullptr, search a proper one.
	const char* decoder_name = "h264"; //"h264" "h264_qsv" ""

	AVFormatContext* avfmtctx = nullptr;
	if (avformat_open_input(&avfmtctx, movie_file_path, nullptr, nullptr) != 0) {
		std::cerr << "Failed: avformat_open_input" << std::endl;
		return -1;
	}

	if (avformat_find_stream_info(avfmtctx, nullptr) < 0) {
		std::cerr << "Failed: avformat_find_stream_info" << std::endl;
		return -1;
	}
	logstr = cv::format("Info: avfmtctx->nb_streams=%d", avfmtctx->nb_streams);
	std::cout << logstr << std::endl;

	// ビデオストリームを特定
	AVStream* avstream = nullptr;
	for (int i = 0; i < (int)avfmtctx->nb_streams; ++i) {
		if (avfmtctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
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
	AVRational time_base_of_avstream = avstream->time_base;
	AVRational avg_frame_rate_of_avstream = avstream->avg_frame_rate; //avformat_find_stream_infoで取得済み．
	AVRational r_frame_rate_of_avstream = avstream->r_frame_rate;

	AVCodec* avcodec = nullptr;
	if (decoder_name == nullptr)
	{
		avcodec = avcodec_find_decoder(avstream->codecpar->codec_id);
		if (avcodec == nullptr) {
			std::cerr << "Failed: Can NOT find any decoders." << std::endl;
			return -1;
		}
	}
	else
	{
		//Intel QuickSyncVideo(QSV), etc
		avcodec = avcodec_find_decoder_by_name(decoder_name);
		if (avcodec == nullptr) {
			std::cerr << "Failed: Not found the decoder - " << decoder_name << std::endl;
			return -1;
		}
	}

	AVCodecContext* avcodecctx = avcodec_alloc_context3(avcodec);
	if (avcodecctx == nullptr) {
		std::cerr << "Failed: avcodec_alloc_context3" << std::endl;
		return -1;
	}

	//AVStreamのコーデックパラメータをAVCodecContextへセット
	if (avcodec_parameters_to_context(avcodecctx, avstream->codecpar) < 0) {
		std::cerr << "Failed: avcodec_parameters_to_context" << std::endl;
		return -1;
	}

	//AVCodecをオープン
	if (avcodec_open2(avcodecctx, avcodec, nullptr) != 0) {
		std::cerr << "Failed: avcodec_open2" << std::endl;
		return -1;
	}

	AVRational time_base_avcodecctx = ffmpeg_load_time_base(avcodecctx, avfmtctx, avstream);
	auto fps_avcodecctx = avcodecctx->framerate; //[注]アクセスタイミングによって不正

	//テスト：最初から最後までシーケンシャルにデコードする．
	//test_read_allframes(avcodecctx, avstream, avfmtctx);

	AVRational time_base_from_fps_of_avstream = av_inv_q(r_frame_rate_of_avstream);
	std::vector<int64_t> pts_list;
	for (int64_t nb_frame = 0; nb_frame < avstream->nb_frames; nb_frame++) {
		pts_list.push_back(av_rescale_q(nb_frame, time_base_from_fps_of_avstream, time_base_of_avstream));
	}

	//テスト：任意フレームへのシークする．	
	test_read_specifidframe(avcodecctx, avfmtctx, avstream, pts_list);

	avcodec_free_context(&avcodecctx);
	avformat_close_input(&avfmtctx);

	return 0;
}

