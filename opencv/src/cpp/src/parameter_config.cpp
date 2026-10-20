// 第一部分：YAML 读写。
#include "parameter_config.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

void normalize(ColorParams& params) {
    // 限制数值范围；上下限写反时交换。
    params.h1_min = std::clamp(params.h1_min, 0, 179);
    params.h1_max = std::clamp(params.h1_max, 0, 179);
    params.h2_min = std::clamp(params.h2_min, 0, 179);
    params.h2_max = std::clamp(params.h2_max, 0, 179);
    params.s_min = std::clamp(params.s_min, 0, 255);
    params.s_max = std::clamp(params.s_max, 0, 255);
    params.v_min = std::clamp(params.v_min, 0, 255);
    params.v_max = std::clamp(params.v_max, 0, 255);
    params.blur = std::clamp(params.blur, 0, 10);
    if (params.h1_min > params.h1_max) std::swap(params.h1_min, params.h1_max);
    if (params.h2_min > params.h2_max) std::swap(params.h2_min, params.h2_max);
    if (params.s_min > params.s_max) std::swap(params.s_min, params.s_max);
    if (params.v_min > params.v_max) std::swap(params.v_min, params.v_max);
}

int readInteger(const cv::FileNode& node, const std::string& name) {
    if (!node[name].isInt()) throw std::runtime_error("YAML 字段应为整数：" + name);
    return static_cast<int>(node[name]);
}

Config loadParameters(const fs::path& path) {
    cv::FileStorage file(path.string(), cv::FileStorage::READ);
    if (!file.isOpened()) throw std::runtime_error("无法读取 YAML：" + path.string());
    if (readInteger(file.root(), "version") != 1) throw std::runtime_error("version 必须为 1");
    Config config{};
    config.mode = std::clamp(readInteger(file.root(), "mode"), 0, 1);
    for (int index = 0; index < 2; ++index) {
        const std::string color = index == 0 ? "red" : "blue";
        const cv::FileNode node = file[color];
        ColorParams& params = config.colors[index];
        params.h1_min = readInteger(node, "h1_min");
        params.h1_max = readInteger(node, "h1_max");
        params.h2_min = readInteger(node, "h2_min");
        params.h2_max = readInteger(node, "h2_max");
        params.s_min = readInteger(node, "s_min");
        params.s_max = readInteger(node, "s_max");
        params.v_min = readInteger(node, "v_min");
        params.v_max = readInteger(node, "v_max");
        params.blur = readInteger(node, "blur");
        normalize(params);
    }
    return config;
}

