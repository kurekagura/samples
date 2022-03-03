#include "captureqthread.h"
#include <QDir>
#include <QtCore/QDebug>
#include <opencv2/opencv.hpp>

CaptureQThread::CaptureQThread(){
}

void CaptureQThread::run(){

    QDir dir("C:/dev/samplevideo/out-avframe2mat");
    QStringList fileNameList = dir.entryList(QStringList() << "*.bmp" << "*.BMP", QDir::Files);

    int i = 0;
    int max = fileNameList.size() - 1;
    while(true)
    {
        QString fileName = fileNameList[i];
        QString filePath = QDir::cleanPath(dir.path() + QDir::separator() + fileName);

        cv::Mat mat = cv::imread(filePath.toStdString());
        emit Signal_RenderImage(mat);
        this->usleep(10);
        i++;
        if(i > max)
            i = 0;
    }
}
