cmake_minimum_required(VERSION 3.10)
project(testOCV VERSION 1.0.0)

find_package(
    OpenCV 4.1.2
    REQUIRED
)

add_executable(
    lap
    lap.cpp
)

target_link_libraries(
    lap
    ${OpenCV_LIBRARIES}
)


