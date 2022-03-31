//Qt非依存としたい
#include "imagecapturethread.h"
#include "myutilities.h"
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

ImageCaptureThread::ImageCaptureThread(QueueChannel<cv::Mat>* qch)
    :qch_(qch)
{
    //映像入力デバイスを想定，ダミーでオンメモリ（pseudo_device_）に取り込んでおく．
    pseudo_device_ = my_load_images();
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
        qch_->push(&mat);

        i++;
        if(i > max)
            i = 0;

        //TODO:ImageProcessorThread側より高速に回すと例外になる。
        //FPSに影響する
        //std::this_thread::sleep_for(std::chrono::nanoseconds(30000));
        std::this_thread::sleep_for(std::chrono::microseconds(30));
    }
}
