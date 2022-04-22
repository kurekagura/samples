#include "capturestdthread.h"
#include "myutilities.h"
#include <chrono>
#include <thread>

CaptureStdThread::CaptureStdThread(bool useRGB, QObject *parent)
    : QObject(parent), thread_stop_requested_(true)
{
    // Assume a video input device and dummy images into on-memory (pseudo_device_).
    pseudo_device_ = my_load_images(useRGB);
}

CaptureStdThread::~CaptureStdThread()
{
}

void CaptureStdThread::getCaptureSize(int* width, int* height)
{
    *width = pseudo_device_[0].cols;
    *height = pseudo_device_[0].rows;
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
        i++;
        if(i > max)
            i = 0;

        //std::this_thread::sleep_for(std::chrono::microseconds(1));
        //std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
        //It will be 300 fps at 4K. If 200<=, the drawing will be wrong.
        std::this_thread::sleep_for(std::chrono::nanoseconds(201));
    }
}
