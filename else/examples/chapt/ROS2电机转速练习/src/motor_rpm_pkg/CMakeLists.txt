cmake_minimum_required(VERSION 3.8)
project(motor_rpm_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

add_executable(rpm_publisher src/rpm_publisher.cpp)
add_executable(rpm_monitor src/rpm_monitor.cpp)
target_include_directories(rpm_publisher PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(rpm_publisher PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  rpm_publisher
  "rclcpp"
  "std_msgs"
)
ament_target_dependencies(
  rpm_monitor
  "rclcpp"
  "std_msgs"
)

install(TARGETS rpm_publisher rpm_monitor
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
