#ifndef MYWINDOW2_H
#define MYWINDOW2_H

#include <memory>
#include <QMainWindow>
#include <QLabel>
#include <spdlog/stopwatch.h>
#include "glplaywidget.h"
#include "capturestdthread.h"

namespace Ui {
class MyWindow2;
}

class MyWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow2(bool useOpengGL, QWidget *parent = nullptr);
    ~MyWindow2();

private:
    bool useOpenGL_;
    QLabel* imageLabel_;
    GLPlayWidget* glPlayWidget_;
    Ui::MyWindow2 *ui;
    std::unique_ptr<CaptureStdThread> capStdThr_;
    uint frame_count_;
    spdlog::stopwatch* sw_;
protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_RenderImage(cv::Mat&);
};

#endif // MYWINDOW2_H
