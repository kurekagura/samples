#ifndef VIDEODEVICETHREAD_H
#define VIDEODEVICETHREAD_H

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <opencv2/opencv.hpp>
#include <Windows.h>

/*
To verify timestamp method
*/
class VideoDeviceThread
{
public:
    VideoDeviceThread();
    virtual ~VideoDeviceThread();

    void start();
    void stop();
    void waitForFinished();
    void getCaptureSize(int* width, int* height);
    uchar* get_image(int* width, int* height, int* step);
    uchar* get_image(int* width, int* height, int* step, LARGE_INTEGER** timestamp);
    double convert_queryperformancecounter_to_msec(const LARGE_INTEGER* qpc);

private:
    //ファイルIOがビデオキャプチャなどより低速なことを想定しオンメモリからとする為。
    //std::vector<cv::Mat> pseudo_device_;
    //timestampの検証
    uchar* pseudo_device_;
    uint pseudo_device_max_;
    uint width_;
    uint height_;
    uint channels_;
    int step_;

    uchar* doublebuff_[2];
    //uchar* curr_doublebuff_;
    std::thread thr_;
    bool thread_stop_requested_;
    bool is_dirty_;
    void func_thread();

    std::mutex mtx_;
    std::condition_variable cond_;

    LARGE_INTEGER query_perf_freq_;
};

#endif // VIDEODEVICETHREAD_H
