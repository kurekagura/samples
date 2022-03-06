#include "mywindow4.h"
#include "ui_mywindow4.h"
#include <QtCore/QDebug>
#include <Windows.h>

MyWindow4::MyWindow4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow4),
    videoThr_(nullptr)
{
    ui->setupUi(this);

    videoThr_ = new VideoDeviceThread();
    videoThr_->start();

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MyWindow4::Slot_Update);

    timer_->start(8);
}

void MyWindow4::Slot_Update()
{
    int width, height, step;
    uchar* bytes = this->videoThr_->get_image(&width, &height, &step);

    size_t size_timestamp = sizeof(LARGE_INTEGER); //=8
    uchar* image = bytes + size_timestamp;
    QImage qimg(image, width, height, step, QImage::Format_BGR888);
    QPixmap qpix = QPixmap::fromImage(qimg);
    ui->imageLabel->setPixmap(qpix);
}

MyWindow4::~MyWindow4()
{
    if(videoThr_ != nullptr)
        delete videoThr_;
    delete ui;
}

void MyWindow4::closeEvent(QCloseEvent *event)
{
    if(videoThr_ != nullptr)
    {
        videoThr_->stop();
        videoThr_->waitForFinished();
    }
}

