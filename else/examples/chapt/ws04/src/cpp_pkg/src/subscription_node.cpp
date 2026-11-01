#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/string.hpp"

class subscription:public rclcpp::Node
{
public:
    subscription():Node("subscription_node")
    {
        RCLCPP_INFO(this->get_logger(),"订阅节点创建");
        subscription_ = this->create_subscription<std_msgs::msg::String>("str_test",10,std::bind(&subscription::callback,this,std::placeholders::_1));
    }
private:
    void callback(const std_msgs::msg::String &msg)
    {
        RCLCPP_INFO(this->get_logger(),"订阅方获取信息:%s",msg.data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<subscription>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}