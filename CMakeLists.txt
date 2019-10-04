PROJECT(yolo_console_dll)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
find_package(Threads)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
link_directories(${TENSORFLOW_BUILD_DIR})

add_subdirectory(code)
