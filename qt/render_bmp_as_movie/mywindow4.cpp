#include "mywindow4.h"
#include "ui_mywindow4.h"
#include <QtCore/QDebug>
#include <Windows.h>
#include <spdlog/spdlog.h>

MyWindow4::MyWindow4(bool useOpenGL, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow4),
    videoThr_(nullptr),
    frame_count_(0),
    useOpenGL_(useOpenGL),
    imageLabel_(nullptr),
    glPlayWidget_(nullptr)
{
    ui->setupUi(this);

    videoThr_ = new VideoDeviceThread();

    if(useOpenGL_)
    {
        this->setWindowTitle(this->windowTitle() + " using OpenGL");
        glPlayWidget_ = new GLPlayWidget(ui->centralwidget);
        int width, height;
        videoThr_->getCaptureSize(&width, &height);
        glPlayWidget_->setFixedSize(width, height);
        ui->verticalLayout->addWidget(glPlayWidget_);
    }else{
        this->setWindowTitle(this->windowTitle() + " using QLabel");
        imageLabel_ = new QLabel(ui->centralwidget);
        ui->verticalLayout->addWidget(imageLabel_);
    }

    sw_ = new spdlog::stopwatch();

    videoThr_->start();

    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, &MyWindow4::Slot_Update);

    //It affects FPS. 0, about 49 fps.
    timer_->start(0); //msec
}

void MyWindow4::Slot_Update()
{
    //spdlog::info("MyWindow4::Slot_Update");
    if(frame_count_ == 0)
        sw_->reset();

    int width, height, step;
    LARGE_INTEGER* timestamp;
    uchar* bytes = this->videoThr_->get_image(&width, &height, &step, &timestamp);
    double msec = this->videoThr_->convert_queryperformancecounter_to_msec(timestamp);
    //spdlog::info("timestamp:{}", msec);

    size_t size_timestamp = sizeof(LARGE_INTEGER); //=8
    uchar* image = bytes + size_timestamp;

    if(useOpenGL_){
        QImage qimg(image, width, height, step, QImage::Format_BGR888);
        glPlayWidget_->setTexture(qimg);
    }else{
        QImage qimg(image, width, height, step, QImage::Format_BGR888);
        QPixmap qpix = QPixmap::fromImage(qimg);
        imageLabel_->setPixmap(qpix);
    }

    frame_count_++;
    if(frame_count_ == 240){
        double sec = sw_->elapsed().count();
        double fps = 240.0 / sec;
        spdlog::info("MyWindow4: elapsed={} fps={:.0f}", sec, fps);
        frame_count_ = 0;
    }
}

MyWindow4::~MyWindow4()
{
    if(sw_ != nullptr)
        delete sw_;
    if(videoThr_ != nullptr)
        delete videoThr_;

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

void MyWindow4::closeEvent(QCloseEvent *event)
{
    timer_->stop();
    if(videoThr_ != nullptr)
    {
        videoThr_->stop();
        videoThr_->waitForFinished();
    }
}

