#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cv::Mat mat = cv::imread("C:/Windows/Web/Wallpaper/Theme1/img1.jpg");
    if(mat.empty())
        return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

