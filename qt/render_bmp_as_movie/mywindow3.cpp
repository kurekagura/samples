#include "mywindow3.h"
#include "ui_mywindow3.h"

#include <QtCore/QDebug>
#include <opencv2/opencv.hpp>
#include "queuechannel.h"

MyWindow3::MyWindow3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow3),
    qch_(nullptr),
    capThr_(nullptr)
{
    ui->setupUi(this);

    qch_ = new QueueChannel<cv::Mat>();

    capThr_ = new ImageCaptureThread(qch_);
    capThr_->start();

    procThr_ = new ImageProcessorThread(qch_, this);

    connect(procThr_, SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);

    procThr_->start();
}

MyWindow3::~MyWindow3()
{
    if(capThr_ != nullptr)
        delete capThr_;
    if(procThr_ != nullptr)
        delete procThr_;
    if(qch_ != nullptr)
        delete qch_;
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
    qDebug() << "MyWindow3::Slot_RenderImage:";

    cv::Mat mat_dst;
    cv::cvtColor(mat, mat_dst, cv::COLOR_BGR2RGB);

    QImage qimg(mat_dst.data, mat_dst.cols, mat_dst.rows, static_cast<int>(mat_dst.step), QImage::Format_RGB888);

    QPixmap qpix = QPixmap::fromImage(qimg);
    ui->imageLabel->setPixmap(qpix);
}
