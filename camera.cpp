#include "camera.h"

Camera::Camera() {
    m_handle = nullptr;
    std::cout << "Camera 对象被创建了" << std::endl;
}

Camera::~Camera() {
    close();
    std::cout << "Camera 对象被销毁了" << std::endl;
}

bool Camera::init() {
    // 1. 枚举设备
    memset(&m_stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
    int nRet = MV_CC_EnumDevices(MV_USB_DEVICE, &m_stDeviceList);
    if (nRet != MV_OK || m_stDeviceList.nDeviceNum == 0) {
        std::cerr << "错误：找不到USB相机！" << std::endl;
        return false;
    }

    // 2. 创建句柄
    nRet = MV_CC_CreateHandle(&m_handle, m_stDeviceList.pDeviceInfo[0]);
    if (nRet != MV_OK) {
        std::cerr << "错误：创建句柄失败！错误码: " << nRet << std::endl;
        return false;
    }

    // 3. 打开设备
    nRet = MV_CC_OpenDevice(m_handle);
    if (nRet != MV_OK) {
        std::cerr << "错误：打开设备失败！错误码: " << nRet << std::endl;
        return false;
    }

    std::cout << "相机打开成功！" << std::endl;
    return true;
}

void Camera::close() {
    if (m_handle != nullptr) {
        MV_CC_CloseDevice(m_handle);
        MV_CC_DestroyHandle(m_handle);
        m_handle = nullptr;
        std::cout << "相机已安全关闭并释放资源。" << std::endl;
    }
}

cv::Mat Camera::getFrame() {
    if (m_handle == nullptr) return cv::Mat();

    // 在 StartGrabbing 之前调用
    int nRet = MV_CC_SetEnumValue(m_handle, "TriggerMode", 0); // 0 通常代表 Off
    if (nRet != MV_OK) {
        std::cerr << "设置触发模式失败！错误码：" << nRet << std::endl;
    }

    // 开启取流
    nRet = MV_CC_StartGrabbing(m_handle);
    if (nRet != MV_OK) {
    std::cerr << "开启取流失败！错误码：" << nRet << std::endl;
    return cv::Mat();
    }
    std::cout << "相机开启取流成功！" << std::endl;

    // 准备接收图像的数据结构
    MV_FRAME_OUT_INFO_EX stImageInfo;
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    
    // 分配缓冲区
    std::vector<unsigned char> rawData(1024 * 1024 * 5);
    
    // 等待一帧图像
    nRet = MV_CC_GetOneFrameTimeout(m_handle, rawData.data(), rawData.size(), &stImageInfo, 3000);
    if (nRet != MV_OK) {
        std::cerr << "获取图像超时或失败，错误码: " << nRet << std::endl;
        MV_CC_StopGrabbing(m_handle);
        return cv::Mat();
    }

    int width = stImageInfo.nExtendWidth;
    int height = stImageInfo.nExtendHeight;
    std::vector<unsigned char> convertedData(width * height * 3); 

    MV_CC_PIXEL_CONVERT_PARAM_EX stConvertParam;
    memset(&stConvertParam, 0, sizeof(stConvertParam));
    stConvertParam.nWidth = width;
    stConvertParam.nHeight = height;
    stConvertParam.pSrcData = rawData.data();
    stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;
    stConvertParam.enSrcPixelType = stImageInfo.enPixelType;
    stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed;
    stConvertParam.pDstBuffer = convertedData.data();
    stConvertParam.nDstBufferSize = convertedData.size();

    nRet = MV_CC_ConvertPixelTypeEx(m_handle, &stConvertParam);
    if (nRet != MV_OK) {
        std::cerr << "像素格式转换失败！" << std::endl;
        MV_CC_StopGrabbing(m_handle);
        return cv::Mat();
    }

    MV_CC_StopGrabbing(m_handle);
    

    cv::Mat frame(height, width, CV_8UC3, convertedData.data());
    return frame.clone();
}