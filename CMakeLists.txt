cmake_minimum_required(VERSION 3.5)

project(Final_Project_Kernel_Rebooters LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

set(HEADER_FILES include/flower_image.hpp include/flower_image_container.hpp include/flower_type.hpp include/preprocessing.hpp)
add_executable(flower_classifier
    src/main.cpp
    src/flower_image.cpp
    src/flower_image_container.cpp
    src/preprocessing.cpp
    src/sift.cpp
    src/surf.cpp
    src/orb.cpp
    src/metrics.cpp
    ${HEADER_FILES}
)
target_include_directories(flower_classifier PRIVATE include)
target_link_libraries(flower_classifier ${OpenCV_LIBS})
