#ifndef CAPTURESTDTHREAD_H
#define CAPTURESTDTHREAD_H

#include <vector>
#include <QObject>
#include <opencv2/opencv.hpp>

class CaptureStdThread : public QObject
{
    Q_OBJECT
public:
    explicit CaptureStdThread(bool useRGB = false, QObject *parent = nullptr);
    virtual ~CaptureStdThread();
    void start();
    void stop();
    void waitForFinished();
    void getCaptureSize(int* width, int* height);

private:
    std::vector<cv::Mat> pseudo_device_;
    std::thread th_;
    bool thread_stop_requested_;
    void thread_func();

signals:
    void Signal_RenderImage(cv::Mat&);
};

#endif // CAPTURESTDTHREAD_H
