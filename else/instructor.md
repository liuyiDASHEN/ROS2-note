经常忘记的指令。

1、构建流程
ros2 pkg create xxx_pkg --build-type ament_cmake --dependencies rclcpp (std_msgs base_interfaces_demo) --node-name xxx
(在includepath那里加/opt/ros/humble/include/**)
增加cpp文件时要加add_executable、ament_target_dependencies、INSTALL。(有时可能要添加target_compile_features)

2、auto node = std::make_shared<xxx>(); (xxx可为自定义类的类型。)

3、
``` cpp
    rclcpp::Publisher<xxx>::SharedPtr publisher_;(xxx可以为std_msgs::msg::String)
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<xxx>::SharedPtr subscription_;
```
```cpp
//线程
void callback(...)
{
    // ... 入队、窗口维护
    std::thread worker(&函数, this, /*参数*/);
    if (worker.joinable()) {
        worker.join();   // 阻塞等待分析结束并回收线程
    }
}
```

4、初始化。
```
(1)subscription_ = this->create_subscription<std_msgs::msg::Int32>("robot_speed",10,bind(&speed_monitor::callback,this,std::placeholders::_1));
(2)publisher_ = this->create_publisher<std_msgs::msg::Int32>("robot_speed",10);
(3)timer_ = this->create_wall_timer(1s,bind(&speed_publisher::on_time,this));
```

5、命名空间。
using namespace std::chrono_literals;

6、头文件。
```
#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include"std_msgs/msg/string.hpp"
#include<vector>
#include<queue>
#include<algorithm>
#include<thread>
#include<mutex>
#include<sstream>(可用于字符串拼接，打印数组。)
```

7、字符串拼接1
```cpp
vector<int>arr={1,2,3,4}; 
std::ostringstream oss;
for(auto x : arr){oss<<x};
RCLCPP_INFO(this->get_logger(),"%s",oss.str().c_str());
```
字符串拼接2
```cpp
        // 字符串拼接
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < v1.size(); ++i)
        {
            oss << v1[i];
            if (i + 1 < v1.size())
            {
                oss << ", ";
            }
        }
        oss << "]";
```

8、github克隆网站
git clone https://githubfast.com/liuyiDASHEN/ROS2-note.git
git clone https://github.com/liuyiDASHEN/ROS2-note



