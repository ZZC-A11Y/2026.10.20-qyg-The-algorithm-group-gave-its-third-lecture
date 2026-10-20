#pragma once
#include "color_detector.hpp"

// 第三部分：图像处理与显示。
Result detectColor(const cv::Mat& image, const ColorParams& params);
