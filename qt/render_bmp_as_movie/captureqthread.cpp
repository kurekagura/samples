#include "captureqthread.h"
#include <QDir>
#include <QtCore/QDebug>
#include <opencv2/opencv.hpp>

CaptureQThread::CaptureQThread(){
}

void CaptureQThread::run(){

    QDir dir("C:/dev/samplevideo/out-avframe2mat-fhd");
    QStringList fileNameList = dir.entryList(QStringList() << "*.bmp" << "*.BMP", QDir::Files);

    int i = 0;
    int max = fileNameList.size() - 1;
    while(true)
    {
        QString fileName = fileNameList[i];
        QString filePath = QDir::cleanPath(dir.path() + QDir::separator() + fileName);

        cv::Mat mat = cv::imread(filePath.toStdString());
        emit Signal_RenderImage(mat);

        i++;
        if(i > max)
            i = 0;

        //FPSに影響する
        //10ns->FPSは150越えするが、再描画されない。
        std::this_thread::sleep_for(std::chrono::nanoseconds(200));
    }
}
