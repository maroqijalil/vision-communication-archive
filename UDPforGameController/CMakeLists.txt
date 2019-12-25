cmake_minimum_required(VERSION 3.10)
project(try_pro VERSION 1.0.0)

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
    sr
    UDPreceiver.cpp
)

target_link_libraries(
    sr
    ${Boost_LIBRARIES}
)