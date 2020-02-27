cmake_minimum_required(VERSION 3.5)

project(kuro_dynamixel)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)

find_package(kuro_motion REQUIRED)
find_package(kuro_motion_interfaces REQUIRED)
find_package(kuro_utility REQUIRED)
find_package(rclcpp REQUIRED)

include_directories("include")

add_library(${PROJECT_NAME}
  "src/${PROJECT_NAME}/joint/arm_joints.cpp"
  "src/${PROJECT_NAME}/joint/body_joints.cpp"
  "src/${PROJECT_NAME}/joint/head_joints.cpp"
  "src/${PROJECT_NAME}/joint/joint.cpp"
  "src/${PROJECT_NAME}/joint/leg_joints.cpp"
  "src/${PROJECT_NAME}/packet/packet.cpp"
  "src/${PROJECT_NAME}/packet/sync_write_packet.cpp"
  "src/${PROJECT_NAME}/packet/write_packet.cpp"
  "src/${PROJECT_NAME}/serial.cpp"
)

ament_target_dependencies(${PROJECT_NAME}
  kuro_motion
  kuro_motion_interfaces
  kuro_utility
  rclcpp
)

target_link_libraries(${PROJECT_NAME}
  boost_system
)

add_executable(kuro_dynamixel_manager "src/kuro_dynamixel_manager.cpp")
target_link_libraries(kuro_dynamixel_manager ${PROJECT_NAME})

install(
  DIRECTORY "include"
  DESTINATION "."
)

install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
  RUNTIME DESTINATION "bin"
)

install(
  TARGETS kuro_dynamixel_manager
  DESTINATION "lib/${PROJECT_NAME}"
)

install(
  DIRECTORY "config"
  DESTINATION "share/${PROJECT_NAME}"
)

ament_export_include_directories("include")
ament_export_libraries(${PROJECT_NAME})
ament_export_dependencies(kuro_motion)
ament_export_dependencies(kuro_motion_interfaces)
ament_export_dependencies(kuro_utility)
ament_export_dependencies(rclcpp)
ament_package()