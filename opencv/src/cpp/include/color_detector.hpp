#pragma once
#include <opencv2/core.hpp>

// 公共数据类型：各模块传递这些数据。
// 只存放一套颜色参数；红色和蓝色各用一个对象。
struct ColorParams {
    int h1_min, h1_max;
    int h2_min, h2_max;
    int s_min, s_max;
    int v_min, v_max;
    int blur;
};

struct Config {
    int mode;  // 0 红色，1 蓝色
    ColorParams colors[2];
};

// 只返回掩膜和颜色提取结果。
struct Result {
    cv::Mat mask;
    cv::Mat extracted;
};

