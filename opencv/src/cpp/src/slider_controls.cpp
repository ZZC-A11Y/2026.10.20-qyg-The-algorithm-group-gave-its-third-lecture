// 第二部分：滑块交互。
#include "slider_controls.hpp"
#include "parameter_config.hpp"

void onSlider(int, void*) {
    // 回调不处理图像；主循环统一读取滑块。
}

void createSliders(const Config& config) {
    // 普通界面：隐藏 OpenCV 自带的工具栏和状态栏。
    cv::namedWindow(WINDOW, cv::WINDOW_NORMAL | cv::WINDOW_GUI_NORMAL);
    cv::resizeWindow(WINDOW, 560, 500);
    cv::createTrackbar("Mode", WINDOW, nullptr, 1, onSlider);
    cv::setTrackbarPos("Mode", WINDOW, config.mode);
    cv::createTrackbar("h1_min", WINDOW, nullptr, 179, onSlider);
    cv::createTrackbar("h1_max", WINDOW, nullptr, 179, onSlider);
    cv::createTrackbar("h2_min", WINDOW, nullptr, 179, onSlider);
    cv::createTrackbar("h2_max", WINDOW, nullptr, 179, onSlider);
    cv::createTrackbar("s_min", WINDOW, nullptr, 255, onSlider);
    cv::createTrackbar("s_max", WINDOW, nullptr, 255, onSlider);
    cv::createTrackbar("v_min", WINDOW, nullptr, 255, onSlider);
    cv::createTrackbar("v_max", WINDOW, nullptr, 255, onSlider);
    cv::createTrackbar("blur", WINDOW, nullptr, 10, onSlider);
    setSliders(config.colors[config.mode]);
}

void setSliders(const ColorParams& params) {
    cv::setTrackbarPos("h1_min", WINDOW, params.h1_min);
    cv::setTrackbarPos("h1_max", WINDOW, params.h1_max);
    cv::setTrackbarPos("h2_min", WINDOW, params.h2_min);
    cv::setTrackbarPos("h2_max", WINDOW, params.h2_max);
    cv::setTrackbarPos("s_min", WINDOW, params.s_min);
    cv::setTrackbarPos("s_max", WINDOW, params.s_max);
    cv::setTrackbarPos("v_min", WINDOW, params.v_min);
    cv::setTrackbarPos("v_max", WINDOW, params.v_max);
    cv::setTrackbarPos("blur", WINDOW, params.blur);
}

ColorParams readSliders() {
    ColorParams params{};
    params.h1_min = cv::getTrackbarPos("h1_min", WINDOW);
    params.h1_max = cv::getTrackbarPos("h1_max", WINDOW);
    params.h2_min = cv::getTrackbarPos("h2_min", WINDOW);
    params.h2_max = cv::getTrackbarPos("h2_max", WINDOW);
    params.s_min = cv::getTrackbarPos("s_min", WINDOW);
    params.s_max = cv::getTrackbarPos("s_max", WINDOW);
    params.v_min = cv::getTrackbarPos("v_min", WINDOW);
    params.v_max = cv::getTrackbarPos("v_max", WINDOW);
    params.blur = cv::getTrackbarPos("blur", WINDOW);
    normalize(params);
    setSliders(params);
    return params;
}

