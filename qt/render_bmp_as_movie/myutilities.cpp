#include "myutilities.h"
#include <vector>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <Windows.h>

std::vector<cv::Mat> my_load_images(bool toRGB){
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
        cv::Mat mat_bgr = cv::imread(afile);
        if(toRGB){
            cv::Mat mat_rgb;
            cv::cvtColor(mat_bgr, mat_rgb, cv::COLOR_BGR2RGB);
            pseudo_device.push_back(mat_rgb);
        }else{
            pseudo_device.push_back(mat_bgr);
        }
    }
    return pseudo_device;
}

extern uchar* my_load_images_as_uchar2x2(uint* count, uint* width, uint* height, uint* channels, bool toRGB)
{
    std::vector<cv::Mat> matVec = my_load_images(toRGB);

    cv::Mat& firstMat = matVec[0];
    *width = firstMat.cols;
    *height = firstMat.rows;
    *channels = firstMat.channels();
    int step_ = static_cast<int>(firstMat.step);
    *count = matVec.size();
    size_t size_bitmap = (*width) * (*height) * (*channels);

    uchar* images = (uchar*)malloc(sizeof(uchar) * size_bitmap * (*count));
    uchar* image_head = images;
    for (int i=0; i < *count; i++) {
        cv::Mat& mat = matVec[i];
        if(mat.isContinuous()){
            memcpy(image_head, mat.data, size_bitmap);
            image_head += size_bitmap;
        }else{
            throw "Exception: The Mat must be isContinuous.";
        }

    }

    return images;
}
