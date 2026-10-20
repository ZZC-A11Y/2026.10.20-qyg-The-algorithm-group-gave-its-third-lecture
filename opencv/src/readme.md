# OpenCV 红蓝颜色检测：滑块版

## 界面

- Controls：只有滑块。Mode=0 检测红色，Mode=1 检测蓝色。
- Original：原图；Mask：黑白掩膜；Result：提取的颜色。
- 图像分别在独立窗口显示，可以自由移动、手动调整窗口大小。
- 不拼图，不自动放大或缩小，不设保存、打印或重置快捷键。
- 拖动滑块，结果实时刷新；关闭 Controls 窗口退出。
- YAML 只用于启动时读取参数，滑块修改不写回文件。

## 文件分类

```text
opencv/
├── config/
│   ├── color_params.yaml       # 共用初始参数
│   └── original_video_000276.jpg
├── src/
│   ├── python/
│   │   ├── color_detector.py   # 入口：读取 → 滑块 → 处理 → 显示
│   │   ├── parameter_config.py # 读取 YAML、检查范围
│   │   ├── slider_controls.py  # 创建和读写滑块
│   │   └── image_processing.py # HSV 颜色筛选
│   └── cpp/
│       ├── CMakeLists.txt
│       ├── include/            # 同名 .hpp：函数声明、数据类型
│       └── src/                # 同名 .cpp：具体实现
└── readme.md
```

C++ 用 `#include "image_processing.hpp"` 引入声明；Python 用 `from image_processing import detect_color` 引入函数。
建议先看 YAML，再看主程序，最后看各部分实现。

## 滑块范围

| 滑块 | 范围 | 作用 |
|---|---|---|
| Mode | 0～1 | 红色 / 蓝色 |
| h1_min、h1_max | 0～179 | 第一段色相上下限 |
| h2_min、h2_max | 0～179 | 第二段色相上下限，两段取并集 |
| s_min、s_max | 0～255 | 饱和度上下限 |
| v_min、v_max | 0～255 | 明度上下限 |
| blur | 0～10 | 0 关闭模糊，n 对应 2*n+1 大小的高斯核 |

先调 H 确定颜色，再提高 s_min 排除灰白、提高 v_min 排除暗部。
漏检时扩大 H 或降低 S/V 下限；误检时反向调整。红色跨越 H 首尾，需要两段；蓝色默认两段相同。

## 用到的 OpenCV 函数

### 图像处理

Python 直接返回结果，C++ 多数使用输出参数。

| 函数 | Python 示例 | 含义 |
|---|---|---|
| imread | `image = cv2.imread(path)` | 读取 BGR 彩色图，失败返回 None |
| GaussianBlur | `blurred = cv2.GaussianBlur(image, (5, 5), 0)` | 高斯模糊；核宽高是正奇数，0 表示由核推算标准差 |
| cvtColor | `hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)` | 转到 HSV：H 色相，S 饱和度，V 明度 |
| inRange | `mask = cv2.inRange(hsv, (100,100,50), (130,255,255))` | 三个通道都在范围内（含边界）为 255，否则为 0 |
| bitwise_or | `mask = cv2.bitwise_or(first, second)` | 合并两段色相掩膜，取并集 |
| bitwise_and | `result = cv2.bitwise_and(image, image, mask=mask)` | 掩膜白色处保留原图，其余变黑 |

C++ 对应写法：

```cpp
cv::Mat image = cv::imread(path);
cv::GaussianBlur(image, blurred, cv::Size(5, 5), 0);
cv::cvtColor(blurred, hsv, cv::COLOR_BGR2HSV);
cv::inRange(hsv, cv::Scalar(100,100,50), cv::Scalar(130,255,255), mask);
cv::bitwise_or(first, second, mask);
cv::bitwise_and(image, image, result, mask);
```

`cv::Mat` 存储图像；`empty()` 判断是否为空；`clone()` 复制图像数据。
`cv::Size(宽, 高)` 表示尺寸；`cv::Scalar(H,S,V)` 在这里表示通道界限。

### 窗口和滑块

| 函数 | Python 示例 | 用途 |
|---|---|---|
| namedWindow | `cv2.namedWindow(name, cv2.WINDOW_NORMAL)` | 创建可手动调整大小的窗口 |
| resizeWindow | `cv2.resizeWindow(name, 560, 500)` | 仅设置滑块控制窗口初始大小 |
| moveWindow | `cv2.moveWindow(name, 600, 40)` | 仅启动时设置位置，后续自由拖动 |
| createTrackbar | `cv2.createTrackbar("Mode", name, 0, 1, on_slider)` | 名称、窗口、初值、最大值、回调 |
| getTrackbarPos | `value = cv2.getTrackbarPos("Mode", name)` | 读取滑块值 |
| setTrackbarPos | `cv2.setTrackbarPos("Mode", name, 1)` | 恢复参数或同步合法范围 |
| imshow | `cv2.imshow(name, image)` | 显示独立图像 |
| waitKey | `cv2.waitKey(20)` | 处理窗口刷新和交互事件，本例不绑定按键 |
| getWindowProperty | `cv2.getWindowProperty(name, cv2.WND_PROP_VISIBLE)` | 检查控制窗口是否关闭 |
| destroyAllWindows | `cv2.destroyAllWindows()` | 关闭全部图像窗口 |

C++ 将 `cv2.` 换为 `cv::`。创建滑块时使用：

```cpp
cv::createTrackbar("Mode", WINDOW, nullptr, 1, onSlider);
cv::setTrackbarPos("Mode", WINDOW, 0);
int mode = cv::getTrackbarPos("Mode", WINDOW);
```

`nullptr` 表示不直接绑定整数地址，用 getTrackbarPos 统一取值。回调是单独定义的空函数，处理放在循环中。

### YAML 读取

```python
file = cv2.FileStorage(str(path), cv2.FILE_STORAGE_READ)
node = file.getNode("red").getNode("h1_min")
value = int(node.real())
file.release()
```

```cpp
cv::FileStorage file(path.string(), cv::FileStorage::READ);
cv::FileNode node = file["red"]["h1_min"];
int value = static_cast<int>(node);
```

`isOpened()` 检查是否打开；`isInt()` 检查是否为整数；`root()` 取得根节点；`release()` 关闭文件。
Python 的 `cv2.error` 用于捕获 OpenCV 异常。

## WSL 运行

Python：

```bash
dev-python
python /mnt/c/Users/13249/Desktop/zzc-/exam/opencv/src/python/color_detector.py
```

C++ 使用 CMake：

```bash
dev-cpp
cmake -S /mnt/c/Users/13249/Desktop/zzc-/exam/opencv/src/cpp -B /home/zzc/workspaces/cpp/opencv-color/build -G Ninja
cmake --build /home/zzc/workspaces/cpp/opencv-color/build --parallel 2
/home/zzc/workspaces/cpp/opencv-color/build/color_detector
```

CMake 的 `-S` 是源码目录，`-B` 是编译目录。以后改代码，只需再次执行 build 命令。
两个程序都支持在运行命令末尾追加图片路径。
