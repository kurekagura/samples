#ifndef MYUTILITIES_H
#define MYUTILITIES_H

#include <vector>
#include <opencv2/opencv.hpp>

extern std::vector<cv::Mat> my_load_images(bool toRGB = false);
extern uchar* my_load_images_as_uchar2x2(uint* count, uint* width, uint* height, uint* channels, bool toRGB = false);

#endif // MYUTILITIES_H
