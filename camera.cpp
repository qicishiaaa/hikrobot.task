#include "Camera.h"

Camera::Camera() {
    m_handle = nullptr;
    std::cout << "Camera 对象被创建了" << std::endl;
}

Camera::~Camera() {
    close(); // 确保退出时释放资源
    std::cout << "Camera 对象被销毁了" << std::endl;
}

bool Camera::init() {
    std::cout << "这里未来会写：枚举设备、创建句柄、打开设备" << std::endl;
    return true; // 先假装成功
}

void Camera::close() {
    // 未来在这里写 MV_CC_CloseDevice 和 MV_CC_DestroyHandle
}