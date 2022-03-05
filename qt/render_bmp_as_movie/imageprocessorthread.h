#ifndef IMAGEPROCESSORTHREAD_H
#define IMAGEPROCESSORTHREAD_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "queuechannel.h"

/*
 * QtのGUIスレッドとの間でシグナルスロットを利用する為，QObjectを継承．
 */
class ImageProcessorThread : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessorThread(QueueChannel<cv::Mat>* qch, QObject *parent = nullptr);

    void start();
    void stop();
    void waitForFinished();

private:
    QueueChannel<cv::Mat>* qch_;
    std::thread th_;
    bool thread_stop_requested_;
    void func_thread();

signals:
    void Signal_RenderImage(cv::Mat&);
};

#endif // IMAGEPROCESSORTHREAD_H
