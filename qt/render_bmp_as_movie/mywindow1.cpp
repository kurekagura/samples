#include "mywindow1.h"
#include "ui_mywindow1.h"

#include <opencv2/opencv.hpp>
#include <QDir>
#include <QtCore/QDebug>

MyWindow1::MyWindow1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyWindow1)
{
    ui->setupUi(this);

    capThread_.start();
    connect(&capThread_, SIGNAL(Signal_RenderImage(cv::Mat&)), this, SLOT(Slot_RenderImage(cv::Mat&)), Qt::QueuedConnection);
}

MyWindow1::~MyWindow1()
{
    delete ui;
}

void MyWindow1::closeEvent(QCloseEvent *event)
{
    //おそらく非推奨．
    capThread_.terminate();
}

void MyWindow1::Slot_RenderImage(cv::Mat& mat){
    qDebug() << "Slot_RenderImage:";

    cv::Mat mat_dst;
    cv::cvtColor(mat, mat_dst, cv::COLOR_BGR2RGB);

    QImage qimg(mat_dst.data, mat_dst.cols, mat_dst.rows, static_cast<int>(mat_dst.step), QImage::Format_RGB888);

    QPixmap qpix = QPixmap::fromImage(qimg);
    ui->imageLabel->setPixmap(qpix);
}


