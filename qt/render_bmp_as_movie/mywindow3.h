#ifndef MYWINDOW3_H
#define MYWINDOW3_H

#include <QMainWindow>
#include "capturestdthread.h"
#include "imagecapturethread.h"
#include "imageprocessorthread.h"
#include "queuechannel.h"

namespace Ui {
class MyWindow3;
}

class MyWindow3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow3(QWidget *parent = nullptr);
    ~MyWindow3();

private:
    Ui::MyWindow3 *ui;
    ImageCaptureThread* capThr_;
    ImageProcessorThread* procThr_;
    QueueChannel<cv::Mat>* qch_;

protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_RenderImage(cv::Mat&);
};

#endif // MYWINDOW3_H
