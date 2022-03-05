#include "mywindow1.h"
#include "mywindow2.h"
#include "mywindow3.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<cv::Mat>("cv::Mat&");

    //QThreadを用いる方式
//    MyWindow1 w1;
//    w1.show();

    //std::threadを用いる方式
//    MyWindow2 w2;
//    w2.show();

    //キュー、条件変数を用いる
    MyWindow3 w3;
    w3.show();

    return a.exec();
}
