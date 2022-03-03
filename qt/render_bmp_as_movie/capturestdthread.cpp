#include "capturestdthread.h"
#include <QDir>
#include <chrono>
#include <thread>

CaptureStdThread::CaptureStdThread(QObject *parent)
    : QObject(parent), thread_stop_requested_(true)
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
    QDir dir("C:/dev/samplevideo/out-avframe2mat");
    QStringList fileNameList = dir.entryList(QStringList() << "*.bmp" << "*.BMP", QDir::Files);

    int i = 0;
    int max = fileNameList.size() - 1;
    while(true)
    {
        if(thread_stop_requested_)
            break;

        QString fileName = fileNameList[i];
        QString filePath = QDir::cleanPath(dir.path() + QDir::separator() + fileName);

        cv::Mat mat = cv::imread(filePath.toStdString());
        emit Signal_RenderImage(mat);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        i++;
        if(i > max)
            i = 0;
    }
}
