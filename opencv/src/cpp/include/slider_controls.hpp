#pragma once
#include "color_detector.hpp"
#include <opencv2/highgui.hpp>

// 第二部分：滑块交互。inline 变量可被多个 cpp 安全引用。
inline const std::string WINDOW = "Controls (0=Red, 1=Blue)";
void onSlider(int value, void* data);
void createSliders(const Config& config);
void setSliders(const ColorParams& params);
ColorParams readSliders();
