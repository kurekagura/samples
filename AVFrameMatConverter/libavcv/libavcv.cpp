#include "libavcv.h"

AVFrame* convert_mat_to_avframe(SwsContext* swsctx, const cv::Mat& mat, AVPixelFormat dst_pix_fmt) {
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
	return dstframe;
}

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