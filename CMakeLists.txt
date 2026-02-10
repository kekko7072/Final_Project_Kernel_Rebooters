cmake_minimum_required(VERSION 3.5)

project(Final_Project_Kernel_Rebooters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

add_executable(detector
    src/main.cpp
    src/sift.cpp
    # src/surf.cpp
    src/metrics.cpp
)
target_link_libraries(detector ${OpenCV_LIBS})