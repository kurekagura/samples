#include <opencv2/opencv.hpp>
#include "captureqthread.h"
#include "myutilities.h"

CaptureQThread::CaptureQThread(bool useRGB){
    // Assume a video input device and dummy images into on-memory (pseudo_device_).
    pseudo_device_ = my_load_images(useRGB);
}

void CaptureQThread::getCaptureSize(int* width, int* height)
{
    *width = pseudo_device_[0].cols;
    *height = pseudo_device_[0].rows;
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

        //std::this_thread::sleep_for(std::chrono::microseconds(3));
        //It will be 280 fps at 4K. If 200<=, the drawing will be wrong.
        std::this_thread::sleep_for(std::chrono::nanoseconds(201));
    }
}
