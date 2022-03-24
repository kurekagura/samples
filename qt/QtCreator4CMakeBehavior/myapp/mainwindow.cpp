#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    try
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic_file_log.txt");

        logger->info("Welcome to spdlog!");
        logger->error("Some error message with arg: {}", 1);

        logger->warn("Easy padding in numbers like {:08d}", 12);
        logger->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
        logger->info("Support for floats {:03.2f}", 1.23456);
        logger->info("Positional args are {1} {0}..", "too", "supported");
        logger->info("{:<30}", "left aligned");

        logger->set_level(spdlog::level::debug); // Set global log level to debug
        logger->debug("This message should be displayed..");

        // change log pattern
        logger->set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

        // Compile time log levels
        // define SPDLOG_ACTIVE_LEVEL to desired level
        SPDLOG_TRACE("Some trace message with param {}", 42);
        SPDLOG_DEBUG("Some debug message");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }

    cv::Mat mat = cv::imread("C:/Windows/Web/Wallpaper/Theme1/img1.jpg");
    if(mat.empty())
        return;
}

MainWindow::~MainWindow()
{
    delete ui;
}

