#ifndef MYWINDOW4_H
#define MYWINDOW4_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <spdlog/stopwatch.h>
#include <opencv2/opencv.hpp>
#include "capturestdthread.h"
#include "videodevicethread.h"
#include "glplaywidget.h"

namespace Ui {
class MyWindow4;
}

class MyWindow4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow4(bool useOpengGL, QWidget *parent = nullptr);
    ~MyWindow4();

private:
    bool useOpenGL_;
    QLabel* imageLabel_;
    GLPlayWidget* glPlayWidget_;
    Ui::MyWindow4 *ui;
    VideoDeviceThread *videoThr_;
    QTimer* timer_;    
    uint frame_count_;
    spdlog::stopwatch* sw_;
protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_Update();
};

#endif // MYWINDOW4_H
