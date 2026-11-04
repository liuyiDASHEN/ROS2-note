#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include<vector>
#include<sstream>
#include<string>
#include<thread>

template <typename T>
T getMax(T a, T b)
{
    return a>b?a:b;
}

class subscription:public rclcpp::Node
{
public:
    subscription():Node("rpm_monitor_node")
    {
        RCLCPP_INFO(this->get_logger(),"监督节点创建");
        //测试
        RCLCPP_INFO(this->get_logger(),"\ngetMax verification (int): getMax(3, 8) = %d",getMax<int>(3,8));
        RCLCPP_INFO(this->get_logger(),"\ngetMax verification (double): getMax(2.5, 1.2) =  %.1f",getMax<double>(2.5,1.2));
        subscription_ = this->create_subscription<std_msgs::msg::Int32>("motor_rpm",10,std::bind(&subscription::callback,this,std::placeholders::_1));
    }
private:
    //状态确认封装成函数
    void checkOverspeed(int rpm)
    {
        if (rpm > 3000)
        {
            Status = "OVERSPEED";
        }
        else
        {
            Status = "NORMAL";
        }
        RCLCPP_INFO(this->get_logger(),"Status: %s\n",Status.c_str());
    };

    void callback(const std_msgs::msg::Int32 &msg)
    {
        //转速记录
        v1.push_back(msg.data);

        //最大获取
        if(v1.size()==1)
        {
            max = v1[0];
        }
        else
        {
            max=getMax(max,v1[num]);
        }

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

        //已封装
        // //确认状态
        // if(msg.data > 3000)
        // {
        //     Status = "OVERSPEED";
        // }
        // else
        // {
        //     Status = "NORMAL";
        // }

        //遍历vector统计
        Total_received=0,Overspeed_count=0;
        for(size_t i = 0;i < v1.size();++i)
        {
            if(v1[i]>3000)
            {
                Overspeed_count++;
            }
            Total_received++;
        }

        //最终打印
        RCLCPP_INFO(this->get_logger(),"\nReceive RPM: %d\nAll RPM records: %s\n",msg.data,oss.str().c_str());

        //多线程
        std::thread t1(&subscription::checkOverspeed,this,msg.data);
        t1.join();

        RCLCPP_INFO(this->get_logger(),"Max RPM so far: %d\nTotal received: %d, Overspeed count: %d\n",max,Total_received,Overspeed_count);
        num++;
    }
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
    std::vector<int>v1;
    size_t num{0};
    int max{0},Total_received,Overspeed_count;
    std::string Status;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<subscription>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}