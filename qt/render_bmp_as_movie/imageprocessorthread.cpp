#include "imageprocessorthread.h"
#include <QtCore/QDebug>

ImageProcessorThread::ImageProcessorThread(QueueChannel<cv::Mat>* qch, QObject *parent)
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
        //qDebug() << "ImageProcessorThread::thread_func";
        if(thread_stop_requested_)
            break;

        cv::Mat* mat = qch_->pop();
        if(mat != nullptr){
            emit Signal_RenderImage(*mat);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}

