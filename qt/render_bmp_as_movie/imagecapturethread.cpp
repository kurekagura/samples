//Qt非依存としたい
#include "imagecapturethread.h"
#include <iostream>
#include <vector>
#include <string>
#include <filesystem> //c++17
#include <opencv2/opencv.hpp>

ImageCaptureThread::ImageCaptureThread(QueueChannel<cv::Mat>* qch)
    :qch_(qch)
{
    //映像入力デバイスを想定，ダミーでオンメモリ（pseudo_device_）に取り込んでおく．
    // 連番画像ファイルの一覧取得・ソート
    const char* input_file_dir = "C:\\\\dev\\\\samplevideo\\\\out-avframe2mat-fhd";
    std::vector<std::string> filePathVec;
    for (const auto& entry : std::filesystem::directory_iterator(input_file_dir))
        filePathVec.push_back(entry.path().string());

    sort(filePathVec.begin(), filePathVec.end(), [](std::string& s1, std::string& s2) { return s1 < s2; });

    // 連番画像ファイルをMatに読み込む
    for (const auto& afile : filePathVec) {
        cv::Mat mat = cv::imread(afile);
        pseudo_device_.push_back(mat);
    }
}

void ImageCaptureThread::start()
{
    thread_stop_requested_ = false;
    if(!this->th_.joinable()){
        this->th_ = std::thread(&ImageCaptureThread::func_thread, this);
    }
}

void ImageCaptureThread::stop()
{
    thread_stop_requested_ = true;
}

void ImageCaptureThread::waitForFinished()
{
    if(this->th_.joinable())
        this->th_.join();
}

void ImageCaptureThread::func_thread()
{
    //pseudo_device_からQueueへの投入
    int i = 0;
    int max = pseudo_device_.size() - 1;
    while(true)
    {
        if(thread_stop_requested_)
            break;

        cv::Mat mat = pseudo_device_[i].clone();
        qch_->put(mat);

        i++;
        if(i > max)
            i = 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
