#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include<vector>
using namespace std::chrono_literals;

class publisher:public rclcpp::Node
{
public:
  publisher():Node("rpm_publisher_node"),num(0)
  {
    RCLCPP_INFO(this->get_logger(),"发布节点创建");
    publisher_ = this->create_publisher<std_msgs::msg::Int32>("motor_rpm",10);
    timer_ = this->create_wall_timer(1s,std::bind(&publisher::on_time,this));
  }
private:
  void on_time()
  {
    std_msgs::msg::Int32 msg;
    msg.data = v[num];
    RCLCPP_INFO(this->get_logger(),"RPM: %d",msg.data);
    publisher_->publish(msg);
    if(num<5)
    {
      num++;
    }else
    {
      num=0;
    }
  }
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  size_t num{0};
  std::vector<int>v={1500, 2200, 3200, 2800, 3500, 2000};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<publisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}