#ifndef MYWINDOW3_H
#define MYWINDOW3_H

#include <memory>
#include <QMainWindow>
#include <QLabel>
#include <spdlog/stopwatch.h>
#include "glplaywidget.h"
#include "imagecapturethread.h"
#include "imageprocessorthread.h"

namespace Ui {
class MyWindow3;
}

class MyWindow3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow3(bool useOpenGL, QWidget *parent = nullptr);
    ~MyWindow3();

private:
    bool useOpenGL_;
    QLabel* imageLabel_;
    GLPlayWidget* glPlayWidget_;
    Ui::MyWindow3 *ui;
    std::unique_ptr<ImageCaptureThread> capThr_;
    std::unique_ptr<ImageProcessorThread> procThr_;
    QueueChannel<cv::Mat>* qch_;
    //MatQueueChannel* qch_;
    uint frame_count_;
    spdlog::stopwatch* sw_;
protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_RenderImage(cv::Mat&);
};

#endif // MYWINDOW3_H
