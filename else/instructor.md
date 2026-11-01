经常忘记的指令。

1、构建流程
ros2 pkg create xxx_pkg --build-type ament_cmake --dependencies rclcpp (std_msgs base_interfaces_demo) --node-name xxx
(在includepath那里加/opt/ros/humble/include/**)
增加cpp文件时要加add_executable、ament_target_dependencies、INSTALL。

2、auto node = std::make_shared<xxx>(); (xxx可为自定义类的类型。)

3、 rclcpp::Publisher<xxx>::SharedPtr publisher_;(xxx可以为std_msgs::msg::String)
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<xxx>::SharedPtr subscription_;

4、初始化。
(1)subscription_ = this->create_subscription<std_msgs::msg::Int32>("robot_speed",10,bind(&speed_monitor::callback,this,std::placeholders::_1));
(2)publisher_ = this->create_publisher<std_msgs::msg::Int32>("robot_speed",10);
(3)timer_ = this->create_wall_timer(1s,bind(&speed_publisher::on_time,this));

5、命名空间。
using namespace std::chrono_literals;

6、头文件。
#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include"std_msgs/msg/string.hpp"
#include<vector>
#include<queue>
#include<algorithm>
#include<thread>
#include<mutex>
#include"sstream"(可用于字符串拼接，打印数组。)
```
vector<int>arr={1,2,3,4}; 
std::ostringstream oss;
for(auto x : arr){oss<<x};
RCLCPP_INFO(this->get_logger(),"%s",oss.str().c_str());
```

