#include "myutilities.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <filesystem>

std::vector<cv::Mat> my_load_images(){
    std::vector<cv::Mat> pseudo_device;

    //映像入力デバイスを想定，ダミーでオンメモリ（pseudo_device_）に取り込んでおく．
    //連番画像ファイルの一覧取得・ソート
    const char* input_file_dir = "C:\\dev\\samplevideo\\out-avframe2mat-fhd";
    std::vector<std::string> filePathVec;
    for (const auto& entry : std::filesystem::directory_iterator(input_file_dir))
        filePathVec.push_back(entry.path().string());

    sort(filePathVec.begin(), filePathVec.end(), [](std::string& s1, std::string& s2) { return s1 < s2; });

    //連番画像ファイルをMatに読み込む
    for (const auto& afile : filePathVec) {
        cv::Mat mat = cv::imread(afile);
        pseudo_device.push_back(mat);
    }
    return pseudo_device;
}
