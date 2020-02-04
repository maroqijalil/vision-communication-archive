cmake_minimum_required(VERSION 3.5)
project(kuro_dynamixel)

find_package(kuro_cmake REQUIRED)

kuro_setup_compiler()

kuro_include_directories(include)

kuro_dependencies(
  kuro_interface
  kuro_utility
  rclcpp
)

kuro_add_libraries(
  boost_system
)

kuro_create_library(
  TARGET ${PROJECT_NAME}
  SOURCES
    src/${PROJECT_NAME}/dynamixel_manager.cpp
    src/${PROJECT_NAME}/mx_servo.cpp
    src/${PROJECT_NAME}/packet.cpp
    src/${PROJECT_NAME}/serial.cpp
)

kuro_create_executable(
  TARGET dynamixel
  SOURCES
    src/dynamixel_main.cpp
)

kuro_package()