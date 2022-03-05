#ifndef MYWINDOW2_H
#define MYWINDOW2_H

#include <QMainWindow>
#include "capturestdthread.h"

namespace Ui {
class MyWindow2;
}

class MyWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow2(QWidget *parent = nullptr);
    ~MyWindow2();

private:
    Ui::MyWindow2 *ui;
    CaptureStdThread *capStdThr_;

protected slots:
    void closeEvent(QCloseEvent *);
    void Slot_RenderImage(cv::Mat&);
};

#endif // MYWINDOW2_H
