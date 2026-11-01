#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/float64.hpp"
#include"vector"
using namespace std::chrono_literals;

class temp_publisher:public rclcpp::Node
{
public:
  temp_publisher():Node("temp_publisher_node")
  {
    RCLCPP_INFO(this->get_logger(),"发布节点创建");
    publisher_ = this->create_publisher<std_msgs::msg::Float64>("device_temperature",10);
    timer_ = this->create_wall_timer(1s,std::bind(&temp_publisher::on_time,this));
  }
private:
  void on_time()
  {
    v={22.5, 23.5, 27.8, 24.1, 30.6, 28.8, 31.5, 25.2};
    std_msgs::msg::Float64 msg;
    msg.data = v[num];
    if(num<7)
    num++;
    else
    num=0;
    RCLCPP_INFO(this->get_logger(),"temp: %.1f",msg.data);
    publisher_->publish(msg);
    }
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::vector<double>v;
  int num=0;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc,argv);
  auto node = std::make_shared<temp_publisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
