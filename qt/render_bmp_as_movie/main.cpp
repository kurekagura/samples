#include "mywindow1.h"
#include "mywindow2.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<cv::Mat>("cv::Mat&");

    MyWindow1 w1;
    w1.show();

    MyWindow2 w2;
    w2.show();

    return a.exec();
}
