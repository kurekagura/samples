#include "mywindow2.h"
#include "ui_mywindow2.h"
#include <spdlog/spdlog.h>

MyWindow2::MyWindow2(bool useOpenGL, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MyWindow2),
      frame_count_(0),
      useOpenGL_(useOpenGL),
      glPlayWidget_(nullptr),
      imageLabel_(nullptr)
{
    ui->setupUi(this);
    sw_= new spdlog::stopwatch();

    //OpenGL support BGR, so no convert.
    capStdThr_ = std::make_unique<CaptureStdThread>(!useOpenGL, this);
    int width, height;
    capStdThr_->getCaptureSize(&width, &height);

    if(useOpenGL_)
    {
        this->setWindowTitle(this->windowTitle() + " using OpenGL");
        glPlayWidget_ = new GLPlayWidget(ui->centralwidget);
        glPlayWidget_->setFixedSize(width, height);
        ui->verticalLayout->addWidget(glPlayWidget_);
    }else{
        this->setWindowTitle(this->windowTitle() + " using QLabel");
        imageLabel_ = new QLabel(ui->centralwidget);
        ui->verticalLayout->addWidget(imageLabel_);
    }

    connect(capStdThr_.get(), SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);
    capStdThr_->start();
}

MyWindow2::~MyWindow2()
{
    if(sw_ != nullptr)
        delete sw_;
    if(glPlayWidget_ != nullptr){
        ui->verticalLayout->removeWidget(glPlayWidget_);
        delete glPlayWidget_;
    }
    if(imageLabel_ != nullptr){
        ui->verticalLayout->removeWidget(imageLabel_);
        delete imageLabel_;
    }
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

    if(useOpenGL_){
        QImage qimg_bgr(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_BGR888);
        glPlayWidget_->setTexture(qimg_bgr);
    }else{
        //Processing as BGR does not result in an error, but the operation is abnormally slow and hangs up.
        QImage qimg_rgb(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        QPixmap qpix_rgb = QPixmap::fromImage(qimg_rgb);
        imageLabel_->setPixmap(qpix_rgb);
    }

    frame_count_++;
    if(frame_count_ == 240){
        double sec = sw_->elapsed().count();
        double fps = 240.0 / sec;
        spdlog::info("MyWindow2: elapsed={} fps={:.0f}", sec, fps);
        frame_count_ = 0;
    }
}
