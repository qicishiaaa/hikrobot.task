cmake_minimum_required(VERSION 3.10)
project(hikrobot.task)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(/opt/MVS/include)
link_directories(/opt/MVS/lib/64)

add_executable(Hikrobot_Task main.cpp Camera.cpp)
target_link_libraries(Hikrobot_Task MvCameraControl ${OpenCV_LIBS} pthread)