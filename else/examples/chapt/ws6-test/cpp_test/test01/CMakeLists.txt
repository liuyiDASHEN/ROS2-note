cmake_minimum_required(VERSION 3.8)
project(test01)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)

add_executable(test_node test01.cpp)

ament_target_dependencies(test_node rclcpp)
install(TARGETS
  test_node
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
