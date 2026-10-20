// 主程序：通过 include 引用各部分，具体实现见同名 cpp。
// 使用上级 cpp 目录的 CMakeLists.txt 构建，命令见 ../../../readme.md。
#include "parameter_config.hpp"
#include "slider_controls.hpp"
#include "image_processing.hpp"
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    try {
        // 一、读取独立 YAML 和图片。
        const fs::path directory = fs::absolute(fs::path(__FILE__)).parent_path().parent_path().parent_path().parent_path() / "config";
        const fs::path yaml = directory / "color_params.yaml";
        Config config = loadParameters(yaml);
        const fs::path imagePath = argc > 1 ? fs::path(argv[1]) : directory / "original_video_000276.jpg";
        const cv::Mat image = cv::imread(imagePath.string());
        if (image.empty()) throw std::runtime_error("图像读取失败：" + imagePath.string());
        // 二、创建滑块。
        createSliders(config);
        cv::namedWindow("Original", cv::WINDOW_NORMAL | cv::WINDOW_GUI_NORMAL);
        cv::namedWindow("Mask", cv::WINDOW_NORMAL | cv::WINDOW_GUI_NORMAL);
        cv::namedWindow("Result", cv::WINDOW_NORMAL | cv::WINDOW_GUI_NORMAL);
        while (true) {
            // 先保存旧模式参数，再恢复新模式，避免红蓝相互覆盖。
            config.colors[config.mode] = readSliders();
            const int mode = cv::getTrackbarPos("Mode", WINDOW);
            if (mode != config.mode) {
                config.mode = mode;
                setSliders(config.colors[mode]);
            }
            // 三、独立显示，不拼图，不反复设置窗口大小。
            const Result result = detectColor(image, config.colors[config.mode]);
            cv::imshow("Original", image);
            cv::imshow("Mask", result.mask);
            cv::imshow("Result", result.extracted);
            // 仅处理窗口事件，不绑定快捷键；关闭控制窗口退出。
            cv::waitKey(20);
            if (cv::getWindowProperty(WINDOW, cv::WND_PROP_VISIBLE) < 1) break;
        }
    } catch (const std::exception& error) {
        std::cerr << "运行失败：" << error.what() << '\n';
        cv::destroyAllWindows();
        return 1;
    }
    cv::destroyAllWindows();
    return 0;
}
