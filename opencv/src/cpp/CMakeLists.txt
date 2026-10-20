cmake_minimum_required(VERSION 3.16)
project(color_detector LANGUAGES CXX)

# 本例用 std::filesystem，需要 C++17。
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找图像处理、图片读取和窗口交互模块。
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui)

# 四个实现文件共同组成一个可执行程序。
add_executable(color_detector
    src/color_detector.cpp
    src/parameter_config.cpp
    src/slider_controls.cpp
    src/image_processing.cpp
)

# 告诉编译器头文件在哪里，以及需要链接哪些 OpenCV 库。
target_include_directories(color_detector PRIVATE include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(color_detector PRIVATE ${OpenCV_LIBS})
