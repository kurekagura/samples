#ifndef CAPTUREQTHREAD_H
#define CAPTUREQTHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>

class CaptureQThread : public QThread
{
    Q_OBJECT
public:
    CaptureQThread();
    void run();

signals:
    void Signal_RenderImage(cv::Mat&);
};

#endif // CAPTUREQTHREAD_H
