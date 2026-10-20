"""第二部分：创建滑块，读取和恢复滑块位置。"""
import cv2
from parameter_config import NAMES, maximum, normalize

WINDOW = "Controls (0=Red, 1=Blue)"


def on_slider(value):
    # 回调不处理图像，主循环统一读取滑块。
    pass


def create_sliders(mode, params):
    # 普通界面：隐藏 OpenCV 自带的工具栏和状态栏。
    cv2.namedWindow(WINDOW, cv2.WINDOW_NORMAL | cv2.WINDOW_GUI_NORMAL)
    cv2.resizeWindow(WINDOW, 560, 500)
    cv2.createTrackbar("Mode", WINDOW, mode, 1, on_slider)
    for name in NAMES:
        cv2.createTrackbar(name, WINDOW, params[name], maximum(name), on_slider)


def set_sliders(params):
    for name in NAMES:
        cv2.setTrackbarPos(name, WINDOW, params[name])


def read_sliders():
    params = {}
    for name in NAMES:
        params[name] = cv2.getTrackbarPos(name, WINDOW)
    previous = params.copy()
    normalize(params)
    if previous != params:
        set_sliders(params)
    return params


