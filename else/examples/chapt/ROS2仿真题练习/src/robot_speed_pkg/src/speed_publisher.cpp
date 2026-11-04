#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include"vector"
using namespace std::chrono_literals;

class speed_publisher:public rclcpp::Node
{
public:
  speed_publisher():Node("speed_publisher_node")
  {
    RCLCPP_INFO(this->get_logger(),"发布节点创建");
    publisher_ = this->create_publisher<std_msgs::msg::Int32>("robot_speed",10);
    timer_ = this->create_wall_timer(1s,std::bind(&speed_publisher::on_time,this));
  }
private:
  void on_time()
  {
    std::vector<int>v={2,5,4,8,3,6};
    std_msgs::msg::Int32 msg;
    msg.data = v[num];
    RCLCPP_INFO(this->get_logger(), "发布信息: speed: %d",msg.data);
    if(num < 5)
    num++;
    else
    num = 0;
    publisher_->publish(msg);
  }
  rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int num = 0;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc,argv);
  auto node = std::make_shared<speed_publisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
