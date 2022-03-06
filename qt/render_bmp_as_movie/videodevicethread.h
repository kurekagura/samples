#ifndef VIDEODEVICETHREAD_H
#define VIDEODEVICETHREAD_H

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>

class VideoDeviceThread
{
public:
    VideoDeviceThread();
    virtual ~VideoDeviceThread();

    void start();
    void stop();
    void waitForFinished();
    uchar* get_image(int* width, int* height, int* step);

private:
    //ファイルIOがビデオキャプチャなどより低速なことを想定しオンメモリからとする為。
    //std::vector<cv::Mat> pseudo_device_;
    uchar* pseudo_device_;
    int pseudo_device_max_;
    int width_;
    int height_;
    int channels_;
    int step_;

    uchar* doublebuff_[2];
    //uchar* curr_doublebuff_;
    std::thread thr_;
    bool thread_stop_requested_;
    bool is_dirty_;
    void func_thread();

    std::mutex mtx_;
    std::condition_variable cond_;
};

#endif // VIDEODEVICETHREAD_H
