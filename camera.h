#pragma once
#include <iostream>
#include "MvCameraControl.h"
#include <opencv2/opencv.hpp>

class Camera {
public:
    Camera();  
    ~Camera(); 

    bool init(); 
    void close(); 

private:
    void* m_handle; 
};