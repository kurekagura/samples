#ifndef IMAGECAPTURETHREAD_H
#define IMAGECAPTURETHREAD_H

#include <thread>
#include <vector>
#include <QObject>
#include <opencv2/opencv.hpp>
#include "queuechannel.h"

//Signal/Slot is not required, so the base does not have to be a QObject.
class ImageCaptureThread : public QObject
{
    Q_OBJECT
public:
    explicit ImageCaptureThread(QueueChannel<cv::Mat>* qch, bool toRGB, QObject *parent = nullptr);
    void start();
    void stop();
    void waitForFinished();
    void getCaptureSize(int* width, int* height);

private:
    QueueChannel<cv::Mat>* qch_;
    //MatQueueChannel* qch_;

    std::thread th_;
    bool thread_stop_requested_;
    void func_thread();

    std::vector<cv::Mat> pseudo_device_;
};

#endif // IMAGECAPTURETHREAD_H
