#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include"queue"
#include"vector"
#include"thread"

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
        RCLCPP_INFO(this->get_logger(),"\n测试int类型: \ncurrent_v = 5, previous_v = 2, \ndelta_v = %d",absoluteDifference<int>(5,2));
        RCLCPP_INFO(this->get_logger(),"\n测试double类型: \ncurrent_v = 3.5, previous_v = 2.5, \ndelta_v = %.2f",absoluteDifference<double>(3.5,2.5));
        monitor_ = this->create_subscription<std_msgs::msg::Int32>("robot_speed",10,std::bind(&speed_monitor::callback,this,std::placeholders::_1));
    }
private:
    void callback(const std_msgs::msg::Int32 &msg)
    {
        received_v = msg.data;
        if(num == 0)
        {
            RCLCPP_INFO(this->get_logger(),"\nReceive Speed: %d\nWaiting for next command...",received_v);
            num++;
            q1.push(received_v);
            return;
        }else if(num>=1)
        {
            q1.push(received_v);
            processing_v = q1.front();
            current_v = processing_v;
            if(have_previous_v == 0)
            {
                previous_v = 0;
                have_previous_v = 1;
            }else
            {
                //获取上一次处理速度
                previous_v = save_previous_v;
            }
            //保存这次处理速度
            save_previous_v = processing_v;
            q1.pop();
        }

        //变化速度获取
        delta_v = absoluteDifference<int>(current_v, previous_v);

        //保存变化的速度
        v1.push_back(delta_v);

        // //确认此次状态
        // if(delta_v >= 3)
        // state = 1;
        // else
        // state = 0;

        // //状态转化为字符串型
        // if(state)
        //     state_ = "WARNING";
        // else
        //     state_ = "NORMAL";

        RCLCPP_INFO(this->get_logger(),"\nReceive Speed: %d\nProcessing Speed: %d\nPrevious Speed: %d\nSpeed Difference: %d",
        received_v,processing_v,previous_v,delta_v);

        std::thread thread1(&speed_monitor::checkSpeedChange,this,current_v,previous_v);
        thread1.join();

        //警告次数获取
        warning_time = 0;
        if (v1.size() > 4)
            v1.erase(v1.begin());
        for (size_t i = 0; i < v1.size(); i++)
        {
            if (v1[i] >= 3)
                warning_time++;
        }

        //确认系统状态
        if(warning_time >= 2)
        system_state = 1;
        else
        system_state = 0;

        //系统状态转化为字符串型
        if(system_state)
            system_state_ = "UNSTABLE";
        else
            system_state_ = "STABLE";

        RCLCPP_INFO(this->get_logger(),"System State: %s",system_state_.c_str());
    }

    void checkSpeedChange(int currentSpeed, int previousSpeed)
    {
        if(absoluteDifference(currentSpeed,previousSpeed)>=3)
        RCLCPP_INFO(this->get_logger(),"State: WARNING");
        else
        RCLCPP_INFO(this->get_logger(),"State: NORMAL");
    }

    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr monitor_;
    std::queue<int>q1;
    int num = 0, received_v, processing_v, current_v, previous_v, delta_v, save_previous_v, warning_time;
    bool have_previous_v = 0,state,system_state;
    std::string state_,system_state_;
    std::vector<int>v1;
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<speed_monitor>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}