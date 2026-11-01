#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/string.hpp"

class Listener: public rclcpp::Node
{
public:
    Listener(): Node("Listener_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "订阅方创立");
        subscription_ = this->create_subscription<std_msgs::msg::String>("chatter", 10, std::bind(&Listener::do_cb,this,std::placeholders::_1));
    }
private:
    void do_cb(const std_msgs::msg::String &msg)
    {
        RCLCPP_INFO(this->get_logger(), "订阅到的消息是：%s",msg.data.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Listener>());
    rclcpp::shutdown();
    return 0;
}