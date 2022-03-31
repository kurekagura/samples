#include "capturestdthread.h"
#include "myutilities.h"
#include <chrono>
#include <thread>

CaptureStdThread::CaptureStdThread(QObject *parent)
    : QObject(parent), thread_stop_requested_(true)
{
    //映像入力デバイスを想定，ダミーでオンメモリ（pseudo_device_）に取り込んでおく．
    pseudo_device_ = my_load_images();
}

CaptureStdThread::~CaptureStdThread()
{
}

void CaptureStdThread::start()
{
    thread_stop_requested_ = false;
    if(!this->th_.joinable()){
        this->th_ = std::thread(&CaptureStdThread::thread_func, this);
    }
}

void CaptureStdThread::stop(){
    thread_stop_requested_ = true;
}

void CaptureStdThread::waitForFinished(){
    if(this->th_.joinable())
        this->th_.join();
}

void CaptureStdThread::thread_func()
{
    int i = 0;
    int max = pseudo_device_.size() - 1;
    while(true)
    {
        if(thread_stop_requested_)
            break;

        emit Signal_RenderImage(pseudo_device_[i]);
        std::this_thread::sleep_for(std::chrono::microseconds(5));
        i++;
        if(i > max)
            i = 0;

        std::this_thread::sleep_for(std::chrono::nanoseconds(200));
    }
}
