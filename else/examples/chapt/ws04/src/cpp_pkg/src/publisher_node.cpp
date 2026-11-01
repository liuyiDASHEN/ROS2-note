#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/string.hpp"
using namespace std::chrono_literals;

class publisher:public rclcpp::Node
{
public:
  publisher():Node("publisher_node"),num(0)
  {
    RCLCPP_INFO(this->get_logger(),"发布节点创建");
    publisher_ = this->create_publisher<std_msgs::msg::String>("str_test",10);
    timer_ = this->create_wall_timer(1s,std::bind(&publisher::on_time,this));
  }
private:
  void on_time()
  {
    std_msgs::msg::String msg;
    msg.data = "Hello World"+std::to_string(num++);
    RCLCPP_INFO(this->get_logger(),"发布消息: %s",msg.data.c_str());
    publisher_->publish(msg);
  }
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t num;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<publisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
