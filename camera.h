#pragma once
#include <iostream>
#include "MvCameraControl.h"
#include <opencv2/opencv.hpp>
#include <vector>

class Camera {
public:
    Camera();
    ~Camera();
    bool init();
    void close();
    cv::Mat getFrame(); // 新增：获取一帧图像

private:
    void* m_handle;
    MV_CC_DEVICE_INFO_LIST m_stDeviceList;
};