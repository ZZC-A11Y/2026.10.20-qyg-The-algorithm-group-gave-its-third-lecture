"""第一部分：读取、保存 YAML，检查参数范围。"""
from pathlib import Path
import cv2

CONFIG = Path(__file__).resolve().parent.parent.parent / "config"
YAML = CONFIG / "color_params.yaml"
NAMES = ["h1_min", "h1_max", "h2_min", "h2_max", "s_min", "s_max", "v_min", "v_max", "blur"]


def maximum(name):
    if name.startswith("h"):
        return 179
    if name == "blur":
        return 10
    return 255


def normalize(params):
    # 限制范围；上下限写反时交换。
    for name in NAMES:
        params[name] = max(0, min(maximum(name), params[name]))
    for name in ("h1", "h2", "s", "v"):
        low, high = name + "_min", name + "_max"
        if params[low] > params[high]:
            params[low], params[high] = params[high], params[low]


def load_parameters(path):
    # 参数只在 YAML 定义，不再在代码里重复一份默认值。
    file = cv2.FileStorage(str(path), cv2.FILE_STORAGE_READ)
    try:
        if not file.isOpened():
            raise ValueError(f"无法读取 YAML：{path}")
        if not file.getNode("version").isInt() or int(file.getNode("version").real()) != 1:
            raise ValueError("version 必须为整数 1")
        if not file.getNode("mode").isInt():
            raise ValueError("mode 必须为整数")
        mode = max(0, min(1, int(file.getNode("mode").real())))
        colors = []
        for color in ("red", "blue"):
            params = {}
            for name in NAMES:
                node = file.getNode(color).getNode(name)
                if not node.isInt():
                    raise ValueError(f"请检查 YAML 中的 {color}.{name}，应为整数")
                params[name] = int(node.real())
            normalize(params)
            colors.append(params)
        return mode, colors
    finally:
        file.release()


