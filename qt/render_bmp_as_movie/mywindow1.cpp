#include "mywindow1.h"
#include "ui_mywindow1.h"

#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>

MyWindow1::MyWindow1(bool useOpengGL, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MyWindow1),
      frame_count_(0),
      useOpenGL_(useOpengGL)
{
    ui->setupUi(this);

    capThread_ = std::make_unique<CaptureQThread>(!useOpengGL); //OpenGL support BGR, so no convert.
    int width, height;
    capThread_->getCaptureSize(&width, &height);

    if(useOpenGL_ == true){
        ui->imageLabel->hide();
        ui->imageLabel->setEnabled(false);
        ui->glPlayWidget->setEnabled(true);
        ui->glPlayWidget->setFixedSize(width, height);
        this->setWindowTitle(this->windowTitle() + " using OpenGL");
    }else{
        ui->glPlayWidget->hide();
        ui->glPlayWidget->setEnabled(false);
        ui->imageLabel->setEnabled(true);
        ui->imageLabel->setFixedSize(width, height);
        this->setWindowTitle(this->windowTitle() + " using QLabel");
    }

    sw_ = new spdlog::stopwatch();
    connect(capThread_.get(), SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);
    capThread_->start();
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
    capThread_->terminate();
}

void MyWindow1::Slot_RenderImage(cv::Mat& mat){
    if(frame_count_ == 0)
        sw_->reset();

    if(useOpenGL_){
        QImage qimg_bgr(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_BGR888);
        ui->glPlayWidget->setTexture(qimg_bgr);
    }else{
        //Processing as BGR does not result in an error, but the operation is abnormally slow and hangs up.
        QImage qimg_rgb(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        QPixmap qpix_rgb = QPixmap::fromImage(qimg_rgb);
        ui->imageLabel->setPixmap(qpix_rgb);
    }

    frame_count_++;
    if(frame_count_ == 240){
        double sec = sw_->elapsed().count();
        double fps = 240.0 / sec;
        spdlog::info("MyWindow1(OpengGL={}): elapsed={} fps={:.0f}", useOpenGL_, sec, fps);
        frame_count_ = 0;
    }
}


