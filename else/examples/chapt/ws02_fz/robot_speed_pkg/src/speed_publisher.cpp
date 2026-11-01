#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include<vector>
using namespace std::chrono_literals;

size_t index_ = 0;
class speed_publisher: public rclcpp::Node
{
public:
    speed_publisher():Node("speed_publisher_node")
    {
        RCLCPP_INFO(this->get_logger(), "发布者节点创建");
        publisher_ = this->create_publisher<std_msgs::msg::Int32>("robot_speed",10);
        timer_ = this->create_wall_timer(1s,std::bind(&speed_publisher::on_timer,this));
    }
private:
    void on_timer()
    {
        std::vector<int>v1 = {6,5,4,8,3,6};
        auto message = std_msgs::msg::Int32();
        if(index_ >= v1.size())
        {
            index_ = 0;
        }
        message.data = v1[index_];
        RCLCPP_INFO(this->get_logger(), "speed:%d",message.data);
        publisher_->publish(message);
        index_++;
    }
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int speed; 
};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<speed_publisher>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}