#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/int32.hpp"
#include"queue"
#include"absoluteDifference.hpp"
#include"vector"

template<class T>
class speed_monitor:public rclcpp::Node
{
public:
    speed_monitor():Node("speed_monitor_node")
    {
        RCLCPP_INFO(this->get_logger(), "订阅方创建");
        subscription_ = this->create_subscription<std_msgs::msg::Int32>("robot_speed",10,std::bind(&speed_monitor::do_cb,this,std::placeholders::_1));
    }

private:
    void do_cb(const std_msgs::msg::Int32 &msg)
    {
        q1.push(msg.data);
        int32_t msg_ = q1.front();
        // v_previous = 0;
        v_current = msg_;
        // Dealt_v = absoluteDifference(v_current, v_previous);
        num = 0;
        print_num = 0;
        //vector
        // v1.reserve(5);
        // v1.resize(0);
        // if(v1.size() >= 4)
        // {
        //     v1.erase(v1.begin());
        //     v1.resize(4);
        // }
        // for(std::vector<int>::iterator it = v1.begin();it!=v1.end();it++)
        // {
        //     if(State)
        //     {
        //         num++;
        //     }
        // }

        // if(num>=2)
        // {
        //     System_State = 1;
        // }
        // else
        // {
        //     System_State = 0;
        // }

        //queue
        if(q1.size() == 1)
        {
            Receive_Speed = q1.back();
            // Dealt_v = absoluteDifference(v_current, v_previous);
            // if(Dealt_v >= 3)
            // {
            //     //RCLCPP_INFO(this->get_logger(),"WARNING");
            //     State = 1;
            // }else
            // {
            //     //RCLCPP_INFO(this->get_logger(),"NORMAL");
            //     State = 0;
            // }
            print_num = 0;
        }
        //else if(q1.size() == 2)
        // {
        //     v_previous = 0;
        //     Receive_Speed = q1.back();
        //     print_num = 1;
        // }
        else if(q1.size() == 2)
        {
            //RCLCPP_INFO(this->get_logger(),"收到:%d->处理，队列 [%d]",msg_,msg.data);
            Receive_Speed = q1.back();
            v_current = q1.front();
            Processing_Speed = v_current;
            v_previous = 0;
            Dealt_v = absoluteDifference(v_current, v_previous);
            // if(Dealt_v >= 3)
            // {
            //     //RCLCPP_INFO(this->get_logger(),"WARNING");
            //     State = 1;
            // }else
            // {
            //     //RCLCPP_INFO(this->get_logger(),"NORMAL");
            //     State = 0;
            // }
            // //遍历vector数组
            // for(std::vector<int>::iterator it = v1.begin();it!=v1.end();it++)
            // {
            //     if(num < 4)
            //     {
            //         if(State)
            //         {
            //             num++;
            //         }
            //     }
            //     if(num >= 4)
            //     {
            //         num = 0;
            //     }
            // }
            print_num = 1;
        }
        else if(q1.size() == 3)
        {
            q1.pop();
            Receive_Speed = q1.back();
            v_current = q1.front();
            Processing_Speed = v_current;
            v_previous = msg_;
            Dealt_v = absoluteDifference(v_current, v_previous);
            // if(Dealt_v >= 3)
            // {
            //     //RCLCPP_INFO(this->get_logger(),"WARNING");
            //     State = 1;
            // }else
            // {
            //     //RCLCPP_INFO(this->get_logger(),"NORMAL");
            //     State = 0;
            // }
            print_num = 1;
        }

        //delta_V
        if(Dealt_v >= 3)
        {
            //RCLCPP_INFO(this->get_logger(),"WARNING");
            State = 1;
        }else
        {
            //RCLCPP_INFO(this->get_logger(),"NORMAL");
            State = 0;
        }

        //vector
        // v1.reserve(5);
        if(State)
        {
            v1.push_back(1);
        }
        else
        {
            v1.push_back(0);
        }
        if(v1.size() > 4)
        {
            v1.erase(v1.begin());
        }

        // 遍历vector数组
        for (size_t i=0;i<v1.size();i++)
        {
            if(v1[i] == 1)
            {
                num++;
            }
        }
        // //vector
        // if(v1.size() >= 4)
        // {
        //     v1.erase(v1.front());
        // }
        // for(std::vector<int>::iterator it = v1.begin();it!=v1.end();it++)
        // {
        //     if(State)
        //     {
        //         num = 0;
        //         num++;
        //     }
        // }
        // if(num>=2)
        // {
        //     System_State = 1;
        // }
        // else
        // {
        //     System_State = 0;
        // }
        //设置System_State状态
        if(num>=2)
        {
            System_State = 1;
        }
        else
        {
            System_State = 0;
        }

        //状态确认
        if(State)
        {
            State_ = "WARNING";
        }else
        {
            State_ = "NORMAL";
        }

        if(System_State)
        {
            System_State_ = "UNSTABLE";
        }else
        {
            System_State_ = "STABLE";
        }

        if(print_num == 0)
        {
            RCLCPP_INFO(this->get_logger(),"\nReceive Speed:%d\nWaiting for next command...",Receive_Speed);
        }
        else if(print_num == 1)
        {
            RCLCPP_INFO(this->get_logger(),"\nReceive Speed:%d\nProcessing Speed:%d\nPrevious Speed:%d\nSpeed Difference:%d\nState:%s\nSystem State:%s\n\n"
            ,Receive_Speed,Processing_Speed,v_previous,Dealt_v,State_.c_str(),System_State_.c_str());
        }
    }
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subscription_;
    std::queue<int32_t>q1;
    T v_previous;
    T v_current;
    T Dealt_v;
    std::vector<int>v1;
    T Receive_Speed;
    T Processing_Speed;
    int num;
    bool State;
    std::string State_;
    bool System_State;
    std::string System_State_;
    int print_num;
};

int main(int argc, char** argv)
{
    rclcpp::init(argc,argv);
    auto node = std::make_shared<speed_monitor<int>>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}