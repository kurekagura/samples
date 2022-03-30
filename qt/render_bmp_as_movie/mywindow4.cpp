#include "mywindow4.h"
#include "ui_mywindow4.h"
#include <QtCore/QDebug>
#include <Windows.h>
#include <spdlog/spdlog.h>

MyWindow4::MyWindow4(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow4),
    videoThr_(nullptr),
    frame_count_(0)
{
    ui->setupUi(this);

    sw_ = new spdlog::stopwatch();

    videoThr_ = new VideoDeviceThread();
    videoThr_->start();

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MyWindow4::Slot_Update);

    //FPSに影響する
    timer_->start(0); //msec
}

void MyWindow4::Slot_Update()
{
    if(frame_count_ == 0)
        sw_->reset();

    int width, height, step;
    uchar* bytes = this->videoThr_->get_image(&width, &height, &step);

    size_t size_timestamp = sizeof(LARGE_INTEGER); //=8
    uchar* image = bytes + size_timestamp;
    QImage qimg(image, width, height, step, QImage::Format_BGR888);
    QPixmap qpix = QPixmap::fromImage(qimg);
    ui->imageLabel->setPixmap(qpix);

    frame_count_++;
    if(frame_count_ == 240){
        double sec = sw_->elapsed().count();
        double fps = 240.0 / sec;
        spdlog::info("elapsed={} fps={:.0f}", sec, fps);
        frame_count_ = 0;
    }
}

MyWindow4::~MyWindow4()
{
    if(sw_ != nullptr)
        delete sw_;
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

