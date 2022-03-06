#include "videodevicethread.h"
#include <filesystem>
#include <memory>
#include <Windows.h>

VideoDeviceThread::~VideoDeviceThread(){
    if(pseudo_device_ != nullptr)
        free(pseudo_device_);
    if(doublebuff_[0] != nullptr)
        free(doublebuff_[0]);
    if(doublebuff_[1] != nullptr)
        free(doublebuff_[1]);
}

VideoDeviceThread::VideoDeviceThread()
    :pseudo_device_(nullptr)
    ,pseudo_device_max_(0)
    ,thread_stop_requested_(false)
    ,is_dirty_(true)
    //,curr_doublebuff_(nullptr)
    ,doublebuff_{nullptr, nullptr}
    //,curr_doublebuff_(doublebuff_[0])
{
    // 連番画像ファイルの一覧取得・ソート
    const char* input_file_dir = "C:\\\\dev\\\\samplevideo\\\\out-avframe2mat-fhd";
    std::vector<std::string> filePathVec;
    for (const auto& entry : std::filesystem::directory_iterator(input_file_dir))
        filePathVec.push_back(entry.path().string());

    sort(filePathVec.begin(), filePathVec.end(), [](std::string& s1, std::string& s2) { return s1 < s2; });

    if(filePathVec.size() < 1)
        throw "最低でも１ファイルは必要です。";

    cv::Mat firstMat = cv::imread(filePathVec[0]);
    width_ = firstMat.cols;
    height_ = firstMat.rows;
    channels_ = firstMat.channels();
    step_ = static_cast<int>(firstMat.step);
    pseudo_device_max_ = filePathVec.size();
    size_t size_bitmap = width_ * height_ * channels_;
    size_t size_bitmap_buff = size_bitmap * pseudo_device_max_;

    pseudo_device_ = (uchar*)malloc(sizeof(uchar) * size_bitmap_buff);

    // 連番画像ファイルをMatに読み込み、画素データのみをuchar一次元配列にコピーしておく。
    uchar* ptr = pseudo_device_;
    for (int i=0; i < pseudo_device_max_; i++) {
        cv::Mat mat = cv::imread(filePathVec[i]);
        //pseudo_device_.push_back();
        if(mat.isContinuous()){
            memcpy(ptr, mat.data, size_bitmap);
            ptr += size_bitmap;
        }else{
            throw "連続配置でなければなりません。";
        }
    }

    //doublebuff_の先頭8バイトにはタイムスタンプを入れる
    size_t size_timestamp = sizeof(LARGE_INTEGER); //=8
    size_t size_row = size_timestamp + size_bitmap;
    doublebuff_[0] = (uchar*)malloc(sizeof(uchar) * size_row);
    doublebuff_[1] = (uchar*)malloc(sizeof(uchar) * size_row);
}

void VideoDeviceThread::start()
{
    thread_stop_requested_ = false;
    if(!this->thr_.joinable()){
        this->thr_ = std::thread(&VideoDeviceThread::func_thread, this);
    }
}

void VideoDeviceThread::stop(){
    thread_stop_requested_ = true;
}

void VideoDeviceThread::waitForFinished(){
    if(this->thr_.joinable())
        this->thr_.join();
}

void VideoDeviceThread::func_thread()
{
    size_t size_bitmap = width_ * height_ * channels_;
    size_t size_timestamp = sizeof(LARGE_INTEGER); //=8

    //120fps以上を想定
    //pseudo_device_->へ１フレーム分コピー
    int idx = -1;
    uchar* ptr = pseudo_device_;
    while(true){
        std::this_thread::sleep_for(std::chrono::microseconds(8333)); //120fps
        //std::this_thread::sleep_for(std::chrono::microseconds(66666)); //15fps

        if(thread_stop_requested_)
            break;

        idx++;
        if(idx == pseudo_device_max_)
        {
            idx = 0;
            ptr = pseudo_device_;
        }
        ptr = pseudo_device_ + (idx * size_bitmap);
        //ptr = ptr + size_bitmap;

        std::lock_guard<std::mutex> lock(mtx_); //cond_.waitは利用できない。
        LARGE_INTEGER qpc;
        if (!QueryPerformanceCounter(&qpc)) {
            throw "Failed:QueryPerformanceCounter";
        }

        //先頭の8バイトにはタイムスタンプ、それ以降に画像データ
        memcpy(doublebuff_[0], (void*)(&qpc.QuadPart), size_timestamp);
        memcpy(doublebuff_[0] + size_timestamp, ptr, size_bitmap);
        is_dirty_ = false;
        cond_.notify_one(); //ダーティでなくなった通知
    }
}

uchar* VideoDeviceThread::get_image(int* width, int* height, int* step)
{
    *width = width_;
    *height = height_;
    *step = step_;

    std::unique_lock<std::mutex> lock(mtx_);
    cond_.wait(lock,[this]{return !is_dirty_;});
    //swap
    uchar* tmp = doublebuff_[0];
    doublebuff_[0] = doublebuff_[1];
    doublebuff_[1] = tmp;
    is_dirty_ = true;
    return doublebuff_[1];
    //デストラクタでロック解除
}

