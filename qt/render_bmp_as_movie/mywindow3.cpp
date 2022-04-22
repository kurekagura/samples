#include "mywindow3.h"
#include "ui_mywindow3.h"
#include <spdlog/spdlog.h>

MyWindow3::MyWindow3(bool useOpenGL, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow3),
    qch_(nullptr),
    capThr_(nullptr),
    frame_count_(0),
    useOpenGL_(useOpenGL),
    glPlayWidget_(nullptr),
    imageLabel_(nullptr)
{
    ui->setupUi(this);
    sw_ = new spdlog::stopwatch();

    qch_ = new QueueChannel<cv::Mat>();
    //qch_ = new MatQueueChannel();
    //OpenGL support BGR, so no convert.
    capThr_ = std::make_unique<ImageCaptureThread>(qch_, !useOpenGL, this);
    int width, height;
    capThr_->getCaptureSize(&width, &height);

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

    capThr_->start();

    procThr_ = std::make_unique<ImageProcessorThread>(qch_, this);
    connect(procThr_.get(), SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);
    procThr_->start();
}

MyWindow3::~MyWindow3()
{
    if(qch_ != nullptr)
        delete qch_;
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

void MyWindow3::closeEvent(QCloseEvent *event)
{
    capThr_->stop();
    capThr_->waitForFinished();

    procThr_->stop();
    procThr_->waitForFinished();
}

void MyWindow3::Slot_RenderImage(cv::Mat& mat){
    if(frame_count_ == 0)
        sw_->reset();

    if(useOpenGL_){

        //The following exception occurs in the constructor of QOpenGLTexture.
        //Exception at 0x7fff4844dfdd, code: 0xc0000005: read access violation at: 0x2700cf61080, flags=0x0 (first chance).
        //You can avoid this by copying the QImage.
        QImage qimg_bgr(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_BGR888);
        glPlayWidget_->setTexture(qimg_bgr.copy());
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
        spdlog::info("MyWindow3: elapsed={} fps={:.0f}", sec, fps);
        frame_count_ = 0;
    }
}
