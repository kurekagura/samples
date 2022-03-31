#include "captureqthread.h"
#include "myutilities.h"
#include <QDir>
#include <QtCore/QDebug>
#include <opencv2/opencv.hpp>

CaptureQThread::CaptureQThread(){
    //映像入力デバイスを想定，ダミーでオンメモリ（pseudo_device_）に取り込んでおく．
    pseudo_device_ = my_load_images();
}

void CaptureQThread::run(){

    int i = 0;
    int max = pseudo_device_.size() - 1;
    while(true)
    {
        emit Signal_RenderImage(pseudo_device_[i]);

        i++;
        if(i > max)
            i = 0;

        //FPSに影響する
        //<3usecだとFPSは150越えするが、再描画されない。
        std::this_thread::sleep_for(std::chrono::microseconds(3));
    }
}
