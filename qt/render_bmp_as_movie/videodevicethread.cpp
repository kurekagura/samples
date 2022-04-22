#include <filesystem>
#include <memory>
#include <Windows.h>
#include "videodevicethread.h"
#include "myutilities.h"

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
    //,curr_doublebuff_(nullptr)
    ,doublebuff_{nullptr, nullptr}
    ,thread_stop_requested_(false)
    ,is_dirty_(true)
    //,curr_doublebuff_(doublebuff_[0])
{
    if (!QueryPerformanceFrequency(&query_perf_freq_)) {
        throw "QueryPerformanceFrequency NOT supported.";
    }

    pseudo_device_ = my_load_images_as_uchar2x2(&pseudo_device_max_, &width_, &height_, &channels_, false);
    size_t size_bitmap = width_ * height_  * channels_;

    //doublebuff_の先頭8バイトには配信時にタイムスタンプを入れる
    size_t size_timestamp = sizeof(LARGE_INTEGER); //=8
    size_t size_row = size_timestamp + size_bitmap;
    doublebuff_[0] = (uchar*)malloc(sizeof(uchar) * size_row);
    doublebuff_[1] = (uchar*)malloc(sizeof(uchar) * size_row);
}

void VideoDeviceThread::getCaptureSize(int* width, int* height)
{
    *width = width_;
    *height = height_;
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

uchar* VideoDeviceThread::get_image(int* width, int* height, int* step, LARGE_INTEGER** timestamp)
{
    *width = width_;
    *height = height_;
    *step = step_;

    std::unique_lock<std::mutex> lock(mtx_);
    cond_.wait(lock, [this] {return !is_dirty_; });
    //swap
    uchar* tmp = doublebuff_[0];
    doublebuff_[0] = doublebuff_[1];
    doublebuff_[1] = tmp;
    is_dirty_ = true;
    *timestamp = reinterpret_cast<LARGE_INTEGER*>(doublebuff_[1]);
    return doublebuff_[1];
    //デストラクタでロック解除
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

double VideoDeviceThread::convert_queryperformancecounter_to_msec(const LARGE_INTEGER* qpc) {
    return ((double)qpc->QuadPart / (double)query_perf_freq_.QuadPart);
}

