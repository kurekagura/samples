#include "mywindow1.h"
#include "mywindow2.h"
#include "mywindow3.h"
#include "mywindow4.h"
//#include "mywindow5.h"

#include <QApplication>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

int main(int argc, char *argv[])
{
    try
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic_file_log.txt");

        //logger->info("main");
        spdlog::set_default_logger(logger);
        // periodically flush all *registered* loggers every 3 seconds:
        // warning: only use if all your loggers are thread safe ("_mt" loggers)
        spdlog::flush_every(std::chrono::seconds(3));
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }

    QApplication a(argc, argv);

    qRegisterMetaType<cv::Mat>("cv::Mat&");

    //QThreadを用いる方式
//    MyWindow1 w1(true);
//    w1.show();

    //std::threadを用いる方式
//    MyWindow2 w2(true);
//    w2.show();

    //キュー、条件変数を用いる(拡張性の面で、理想はこの方式)
//    MyWindow3 w3(true);
//    w3.show();

    //Timestamp(VideoDeviceThread)実装
    //QTimerを用いる方式
    MyWindow4 w4(true);
    w4.show();

    //GLPlayWidget2検証 QTimer
//    MyWindow5 w5;
//    w5.show();

    return a.exec();
}
