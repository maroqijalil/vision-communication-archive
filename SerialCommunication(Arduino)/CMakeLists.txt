cmake_minimum_required(VERSION 3.10)
project(project2 VERSION 1.0.0)

find_package(
    Boost 1.65.1
    REQUIRED COMPONENTS
    thread
    filesystem
    system
    chrono
    atomic
    date_time
)

include_directories(
    include
    ${Boost_INCLUDE_DIRS}
)

add_executable(
    sc
    SerialCommunication.cpp
)

target_link_libraries(
    sc
    ${Boost_LIBRARIES}
)