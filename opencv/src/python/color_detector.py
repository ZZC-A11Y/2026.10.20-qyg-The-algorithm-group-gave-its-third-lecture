"""主程序：只负责连接三个部分，具体函数分别在对应模块。
WSL：dev-python，然后 python /mnt/c/Users/13249/Desktop/zzc-/exam/opencv/src/python/color_detector.py
仅拖动滑块实时调参，关闭控制窗口退出。
"""
from pathlib import Path
import sys
import cv2
from parameter_config import CONFIG, YAML, load_parameters
from slider_controls import WINDOW, create_sliders, read_sliders, set_sliders
from image_processing import detect_color


def main():
    # 一、读取独立 YAML 和图片。
    mode, colors = load_parameters(YAML)
    image_path = CONFIG / "original_video_000276.jpg"
    if len(sys.argv) > 1:
        image_path = Path(sys.argv[1])
    image = cv2.imread(str(image_path))
    if image is None:
        raise ValueError(f"图像读取失败：{image_path}")
    # 二、创建滑块。
    create_sliders(mode, colors[mode])
    # 图像各自独立，可拖动窗口或手动调整尺寸；程序不缩放图像。
    for name in ("Original", "Mask", "Result"):
        cv2.namedWindow(name, cv2.WINDOW_NORMAL | cv2.WINDOW_GUI_NORMAL)
    cv2.moveWindow(WINDOW, 20, 40)
    cv2.moveWindow("Result", 600, 40)
    while True:
        # 先保存旧模式参数，再恢复新模式参数，避免红蓝相互覆盖。
        colors[mode] = read_sliders()
        new_mode = cv2.getTrackbarPos("Mode", WINDOW)
        if new_mode != mode:
            mode = new_mode
            set_sliders(colors[mode])
        # 三、分别显示，不拼图，也不反复设置窗口大小。
        mask, result = detect_color(image, colors[mode])
        cv2.imshow("Original", image)
        cv2.imshow("Mask", mask)
        cv2.imshow("Result", result)
        # waitKey 只让窗口响应拖动和刷新，不绑定快捷键。
        cv2.waitKey(20)
        if cv2.getWindowProperty(WINDOW, cv2.WND_PROP_VISIBLE) < 1:
            break



if __name__ == "__main__":
    try:
        main()
    except (cv2.error, ValueError, OSError, SystemError) as error:
        print("运行失败：", error)
        sys.exit(1)
    finally:
        cv2.destroyAllWindows()
