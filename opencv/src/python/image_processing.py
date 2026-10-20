"""第三部分：高斯模糊、HSV 筛选、提取颜色和显示。"""
import cv2


def detect_color(image, params):
    # 1. 高斯模糊降噪；0 表示关闭。
    blurred = image
    if params["blur"] > 0:
        size = 2 * params["blur"] + 1
        blurred = cv2.GaussianBlur(image, (size, size), 0)
    # 2. BGR 转 HSV。
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)
    # 3. 范围内像素变白，其他变黑；两段 H 取并集。
    first = cv2.inRange(hsv,
        (params["h1_min"], params["s_min"], params["v_min"]),
        (params["h1_max"], params["s_max"], params["v_max"]))
    second = cv2.inRange(hsv,
        (params["h2_min"], params["s_min"], params["v_min"]),
        (params["h2_max"], params["s_max"], params["v_max"]))
    mask = cv2.bitwise_or(first, second)
    # 4. 掩膜白色位置保留原图，其余变黑。
    result = cv2.bitwise_and(image, image, mask=mask)
    return mask, result
