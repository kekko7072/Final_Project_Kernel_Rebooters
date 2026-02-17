cmake_minimum_required(VERSION 3.5)

project(Final_Project_Kernel_Rebooters LANGUAGES CXX)

option(CONFIG_ENABLE_SURF "Enable SURF feature extractor (requires xfeatures2d module)" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CONFIG_ENABLE_SURF)
    set(TARGET_DEFINITIONS ${TARGET_DEFINITIONS} -DENABLE_SURF)
endif()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(include)

set(HEADER_FILES
    include/flower_image.hpp
    include/flower_image_container.hpp
    include/flower_type.hpp
    include/preprocessing.hpp
    include/metrics.h
    include/orb.h
    include/sift.h
    include/hog.h
    include/bow.h
    include/matching.h
    include/sift_processing.h
    include/template_match_processing.hpp
    include/print_stats.h
)
set(SOURCE_FILES
    src/main.cpp
    src/flower_image.cpp
    src/flower_image_container.cpp
    src/flower_type.cpp
    src/preprocessing.cpp
    src/sift.cpp
    src/orb.cpp
    src/hog.cpp
    src/bow.cpp
    src/matching.cpp
    src/metrics.cpp
    src/sift_processing.cpp
    src/print_stats.cpp
)
if(CONFIG_ENABLE_SURF)
    set(HEADER_FILES
        ${HEADER_FILES}
        include/surf.h
        include/surf_processing.h
    )
    set(SOURCE_FILES
        ${SOURCE_FILES}
        src/surf.cpp
        src/surf_processing.cpp
    )
endif()
add_executable(flower_classifier
    ${SOURCE_FILES}
#    ${HEADER_FILES}
)
target_compile_definitions(flower_classifier PRIVATE ${TARGET_DEFINITIONS})
target_include_directories(flower_classifier PRIVATE include)
target_link_libraries(flower_classifier ${OpenCV_LIBS})
