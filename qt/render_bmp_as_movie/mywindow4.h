#ifndef MYWINDOW4_H
#define MYWINDOW4_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include "capturestdthread.h"
#include "videodevicethread.h"

namespace Ui {
class MyWindow4;
}

class MyWindow4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow4(QWidget *parent = nullptr);
    ~MyWindow4();

private:
    Ui::MyWindow4 *ui;
    VideoDeviceThread *videoThr_;
    QTimer* timer_;    

protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_Update();
};

#endif // MYWINDOW4_H
