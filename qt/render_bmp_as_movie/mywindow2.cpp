#include "mywindow2.h"
#include "ui_mywindow2.h"
#include <QtCore/QDebug>
#include <spdlog/spdlog.h>

MyWindow2::MyWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow2),
    frame_count_(0)
{
    ui->setupUi(this);
    sw_= new spdlog::stopwatch();
    capStdThr_ = new CaptureStdThread(this);
    connect(capStdThr_, SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);
    capStdThr_->start();
}

MyWindow2::~MyWindow2()
{
    if(capStdThr_ != nullptr)
        delete capStdThr_;
    if(sw_ != nullptr)
        delete sw_;
    delete ui;
}

void MyWindow2::closeEvent(QCloseEvent *event)
{
    capStdThr_->stop();
    capStdThr_->waitForFinished();
}

void MyWindow2::Slot_RenderImage(cv::Mat& mat)
{
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
