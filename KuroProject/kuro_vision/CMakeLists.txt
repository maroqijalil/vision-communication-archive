cmake_minimum_required(VERSION 3.5)
project(kuro_vision)

find_package(kuro_cmake REQUIRED)

kuro_setup_compiler()

kuro_include_directories(
  include
)

kuro_dependencies(
  kuro_interface
  kuro_utility
  rclcpp
  rclpy
)

kuro_dependencies(
  OpenCV
)

kuro_create_library(
  TARGET ${PROJECT_NAME}
  SOURCES
    src/${PROJECT_NAME}/device/camera.cpp
    src/${PROJECT_NAME}/camera_manager.cpp
)

kuro_create_executable(
  TARGET camera
  SOURCES
    src/camera_main.cpp
)

kuro_create_executable(
  TARGET image_viewer
  SOURCES
    src/image_viewer_main.cpp
)

kuro_install_directories(
  config
)

kuro_package()
