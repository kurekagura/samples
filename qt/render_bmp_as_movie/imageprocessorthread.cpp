#include <spdlog/spdlog.h>
#include <QtCore/QDebug>
#include <opencv2/opencv.hpp>
#include "imageprocessorthread.h"

ImageProcessorThread::ImageProcessorThread(QueueChannel<cv::Mat>* qch, QObject *parent)
//ImageProcessorThread::ImageProcessorThread(MatQueueChannel* qch, QObject *parent)
    :QObject(parent)
    ,qch_(qch)
{
}

void ImageProcessorThread::start()
{
    thread_stop_requested_ = false;
    if(!this->th_.joinable()){
        this->th_ = std::thread(&ImageProcessorThread::func_thread, this);
    }
}

void ImageProcessorThread::stop(){
    thread_stop_requested_ = true;
    this->qch_->close();
}

void ImageProcessorThread::waitForFinished(){
    if(this->th_.joinable())
        this->th_.join();
}

void ImageProcessorThread::func_thread()
{
    //キューから取り出し，GUIへ
    while(true)
    {
        if(thread_stop_requested_)
            break;

        const cv::Mat mat = qch_->pop();
        if(!mat.empty()){
            //spdlog::info("refcount={}", mat.u ? mat.u->refcount : 0);
            emit Signal_RenderImage(const_cast<cv::Mat&>(mat));
        }else{
            spdlog::info("mat is empty");
        }

        //TODO:ImageCaptureThread側より遅く回すと例外になる。
        //FPSに影響する
        //std::this_thread::sleep_for(std::chrono::nanoseconds(20000));
        //std::this_thread::sleep_for(std::chrono::microseconds(0));
    }
}

