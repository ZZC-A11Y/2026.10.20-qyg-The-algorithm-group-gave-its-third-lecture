#pragma once
#include "color_detector.hpp"
#include <filesystem>

// 第一部分：YAML 参数读写和范围检查。
void normalize(ColorParams& params);
Config loadParameters(const std::filesystem::path& path);
