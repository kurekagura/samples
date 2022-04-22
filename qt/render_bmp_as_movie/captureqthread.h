#ifndef CAPTUREQTHREAD_H
#define CAPTUREQTHREAD_H

#include <vector>
#include <QThread>
#include <opencv2/opencv.hpp>

class CaptureQThread : public QThread
{
    Q_OBJECT
public:
    CaptureQThread(bool useRGB = false);
    void run();
    void getCaptureSize(int* width, int* height);
private:
    std::vector<cv::Mat> pseudo_device_;

signals:
    void Signal_RenderImage(cv::Mat&);
};

#endif // CAPTUREQTHREAD_H
