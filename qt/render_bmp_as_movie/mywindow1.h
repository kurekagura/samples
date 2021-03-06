#ifndef MYWINDOW1_H
#define MYWINDOW1_H

#include <memory>
#include <QMainWindow>
#include <QThread>
#include <spdlog/stopwatch.h>
#include <opencv2/opencv.hpp>
#include "captureqthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MyWindow1; }
QT_END_NAMESPACE

class MyWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    MyWindow1(bool useOpenGL, QWidget *parent = nullptr);
    ~MyWindow1();

private:
    bool useOpenGL_;
    Ui::MyWindow1 *ui;
    std::unique_ptr<CaptureQThread> capThread_;
    uint frame_count_;
    spdlog::stopwatch* sw_;

protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_RenderImage(cv::Mat&);
};
#endif // MYWINDOW1_H
