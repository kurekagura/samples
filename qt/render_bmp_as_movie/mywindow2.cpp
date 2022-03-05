#include "mywindow2.h"
#include "ui_mywindow2.h"
#include <QtCore/QDebug>

MyWindow2::MyWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow2)
{
    ui->setupUi(this);

    capStdThr_ = new CaptureStdThread(this);
    connect(capStdThr_, SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);

    capStdThr_->start();
}

MyWindow2::~MyWindow2()
{
    delete ui;
    delete capStdThr_;
}

void MyWindow2::closeEvent(QCloseEvent *event)
{
    capStdThr_->stop();
    capStdThr_->waitForFinished();
}

void MyWindow2::Slot_RenderImage(cv::Mat& mat)
{
    qDebug() << "Slot_RenderImage:";

    cv::Mat mat_dst;
    cv::cvtColor(mat, mat_dst, cv::COLOR_BGR2RGB);

    QImage qimg(mat_dst.data, mat_dst.cols, mat_dst.rows, static_cast<int>(mat_dst.step), QImage::Format_RGB888);

    QPixmap qpix = QPixmap::fromImage(qimg);
    ui->imageLabel->setPixmap(qpix);
}
