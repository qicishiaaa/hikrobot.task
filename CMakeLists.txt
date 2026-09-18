cmake_minimum_required(VERSION 3.10)
project(hikrobot.task)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(/opt/MVS/include)
link_directories(/opt/MVS/lib/64)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(hikrobot.task main.cpp camera.cpp)
target_link_libraries(hikrobot.task MvCameraControl ${OpenCV_LIBS} pthread)