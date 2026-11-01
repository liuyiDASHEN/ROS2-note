#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include"queue"
#include"vector"

template<typename T>
T absoluteDifference(T a, T b)
{
    return a>b ? a-b : b-a;
}

class speed_monitor:public rclcpp::Node
{
public:
    speed_monitor():Node("speed_monitor_node")
    {
        RCLCPP_INFO(this->get_logger(),"监督节点创建");
        monitor_ = this->create_subscription<std_msgs::msg::Int32>("robot_speed",10,std::bind(&speed_monitor::callback,this,std::placeholders::_1));
    }
private:
    void callback(const std_msgs::msg::Int32 &msg)
    {
        q1.push(msg.data);
        if (q1.size() == 1)
        {
            RCLCPP_INFO(this->get_logger(), "\ndouble类型测试,current_v = 3.5, previous_v = 2.5,Delta_v = %.2f\n", absoluteDifference<double>(3.5, 2.5));
            RCLCPP_INFO(this->get_logger(), "\nint类型测试,current_v = 6, previous_v = 2,Delta_v = %d\n", absoluteDifference<int>(6, 2));
            RCLCPP_INFO(this->get_logger(), "\nReceive Speed:%d\nWaiting for next command...\n", msg.data);
            return;
        }
        else if (q1.size() == 2)
        {
            previous_v = 0;
            current_v = q1.front();
            delta_v = absoluteDifference<int>(current_v, previous_v);
        }
        else if (q1.size() > 2)
        {
            previous_v = q1.front();
            q1.pop();
            current_v = q1.front();
            delta_v = absoluteDifference<int>(current_v, previous_v);
        }

        // 状态确认
        if (delta_v >= 3)
        {
            state = 1;
            v1.push_back(1);
        }
        else
        {
            state = 0;
            v1.push_back(0);
        }

        //保存数最大为4
        if(v1.size() > 4)
        {
            int temp[4];
            for(int i=0;i<4;i++)
            {
                temp[i] = v1[i+1];
            }
            for(int i=0;i<4;i++)
            {
                v1[i] = temp[i];
            }
            v1.pop_back();
        }

        //警告次数
        int warning_num=0;
        for(size_t i=0;i<v1.size();i++)
        {
            if(v1[i]==1)
            {
                warning_num++;
            }
        }


        //系统状态
        if(warning_num >= 2)
        {
            system_state = 1;
        }
        else
        {
            system_state = 0;
        }

        //状态转为字符串
        if(state)
        state_ = "WARNING";
        else
        state_ = "NORMAL";
        if(system_state)
        system_state_ = "UNSTABLE";  
        else
        system_state_ = "STABLE";

        //最终打印
        RCLCPP_INFO(this->get_logger(),"\nReceive Speed: %d\nProcessing Speed: %d\nPrevious Speed: %d\nSpeed Difference: %d\nState: %s\nSystem State: %s\n"
        ,msg.data,current_v,previous_v,delta_v,state_.c_str(),system_state_.c_str());
    }
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr monitor_;
    std::queue<int>q1;
    int previous_v, current_v,delta_v=0;
    bool state,system_state;
    std::string state_,system_state_;
    std::vector<size_t>v1;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<speed_monitor>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}