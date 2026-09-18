#include "camera.h"

int main() {
    Camera cam;
    if (cam.init()) {
        std::cout << "按 ESC 键退出程序。" << std::endl;
        
        while (true) {
            cv::Mat frame = cam.getFrame();
            if (!frame.empty()) {
                cv::imshow("Hikrobot Camera", frame);
            }
            // 等待 30 毫秒，如果按下 ESC 键就退出循环
            if (cv::waitKey(30) == 27) { 
                break;
            }
        }
    }
    return 0;
}