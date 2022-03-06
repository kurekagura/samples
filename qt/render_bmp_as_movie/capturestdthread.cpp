#include "capturestdthread.h"
//#include <QDir>
#include <chrono>
#include <thread>
#include <filesystem>

CaptureStdThread::CaptureStdThread(QObject *parent)
    : QObject(parent), thread_stop_requested_(true)
{
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
    // 連番画像ファイルの一覧取得・ソート
    const char* input_file_dir = "C:\\\\dev\\\\samplevideo\\\\out-avframe2mat-fhd";
    std::vector<std::string> filePathVec;
    for (const auto& entry : std::filesystem::directory_iterator(input_file_dir))
        filePathVec.push_back(entry.path().string());

    sort(filePathVec.begin(), filePathVec.end(), [](std::string& s1, std::string& s2) { return s1 < s2; });

    int i = 0;
    int max = filePathVec.size() - 1;
    while(true)
    {
        if(thread_stop_requested_)
            break;

        std::string filePath = filePathVec[i];
        cv::Mat mat = cv::imread(filePath);
        emit Signal_RenderImage(mat);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        i++;
        if(i > max)
            i = 0;
    }

//    QDir dir("C:/dev/samplevideo/out-avframe2mat");
//    QStringList fileNameList = dir.entryList(QStringList() << "*.bmp" << "*.BMP", QDir::Files);

//    int i = 0;
//    int max = fileNameList.size() - 1;
//    while(true)
//    {
//        if(thread_stop_requested_)
//            break;

//        QString fileName = fileNameList[i];
//        QString filePath = QDir::cleanPath(dir.path() + QDir::separator() + fileName);

//        cv::Mat mat = cv::imread(filePath.toStdString());
//        emit Signal_RenderImage(mat);
//        std::this_thread::sleep_for(std::chrono::microseconds(10));
//        i++;
//        if(i > max)
//            i = 0;
//    }
}
