// 第三部分：图像处理。
#include "image_processing.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

Result detectColor(const cv::Mat& image, const ColorParams& params) {
    Result result;
    // 1. 高斯模糊降噪，blur=0 时直接使用原图。
    cv::Mat blurred = image.clone();
    if (params.blur > 0) {
        const int size = 2 * params.blur + 1;
        cv::GaussianBlur(image, blurred, cv::Size(size, size), 0);
    }
    // 2. BGR 转 HSV。
    cv::Mat hsv;
    cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
    // 3. 范围内为白色 255，其余为黑色 0；两段 H 取并集。
    cv::Mat first, second;
    cv::inRange(hsv, cv::Scalar(params.h1_min, params.s_min, params.v_min),
                cv::Scalar(params.h1_max, params.s_max, params.v_max), first);
    cv::inRange(hsv, cv::Scalar(params.h2_min, params.s_min, params.v_min),
                cv::Scalar(params.h2_max, params.s_max, params.v_max), second);
    cv::bitwise_or(first, second, result.mask);
    // 4. 掩膜白色位置保留原图，其余位置变黑。
    cv::bitwise_and(image, image, result.extracted, result.mask);
    return result;
}
