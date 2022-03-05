#ifndef IMAGECAPTURETHREAD_H
#define IMAGECAPTURETHREAD_H

#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>
#include "queuechannel.h"

class ImageCaptureThread
{
public:
    explicit ImageCaptureThread(QueueChannel<cv::Mat>* qch);
    void start();
    void stop();
    void waitForFinished();
private:
    QueueChannel<cv::Mat>* qch_;
    std::thread th_;
    bool thread_stop_requested_;
    void func_thread();

    std::vector<cv::Mat> pseudo_device_;
};

#endif // IMAGECAPTURETHREAD_H
