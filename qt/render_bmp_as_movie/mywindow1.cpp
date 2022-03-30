#include "mywindow1.h"
#include "ui_mywindow1.h"

#include <opencv2/opencv.hpp>
#include <QDir>
#include <QtCore/QDebug>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

MyWindow1::MyWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyWindow1)
    , frame_count_(0)
{
    ui->setupUi(this);
    sw_ = new spdlog::stopwatch();
    connect(&capThread_, SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);
    capThread_.start();
}

MyWindow1::~MyWindow1()
{
    if(sw_ != nullptr)
        delete sw_;
    delete ui;
}

void MyWindow1::closeEvent(QCloseEvent *event)
{
    //おそらく非推奨．
    capThread_.terminate();
}

void MyWindow1::Slot_RenderImage(cv::Mat& mat){
    if(frame_count_ == 0)
        sw_->reset();

    cv::Mat mat_dst;
    cv::cvtColor(mat, mat_dst, cv::COLOR_BGR2RGB);

    QImage qimg(mat_dst.data, mat_dst.cols, mat_dst.rows, static_cast<int>(mat_dst.step), QImage::Format_RGB888);

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


