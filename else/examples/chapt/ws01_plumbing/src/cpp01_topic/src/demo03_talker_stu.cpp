#include"rclcpp/rclcpp.hpp"
#include"base_interfaces_demo/msg/student.hpp"
using namespace std::chrono_literals;
using base_interfaces_demo::msg::Student;

class TalkerStu : public rclcpp::Node
{
public:
  TalkerStu():Node("talkerstu_node_cpp"),count(0)
  {
    publisher_ =  this->create_publisher<Student>("chatter_stu", 10);
    timer_ = this->create_wall_timer(500ms,std::bind(&TalkerStu::on_timer,this));
  }
private:
  void on_timer()
  {
    auto stu = Student();
    stu.name = "huluwa";
    stu.age = age;
    stu.height = 2.20;
    age++;
    publisher_->publish(stu);
    RCLCPP_INFO(this->get_logger(), "发布消息：(%s,%d,%.2f)",stu.name.c_str(),stu.age,stu.height);
  }
  size_t count;
  rclcpp::Publisher<Student>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  int age;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TalkerStu>());
  rclcpp::shutdown();
  return 0;
}
