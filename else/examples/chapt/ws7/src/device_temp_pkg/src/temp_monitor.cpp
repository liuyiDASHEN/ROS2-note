#include"rclcpp/rclcpp.hpp"
#include"std_msgs/msg/float64.hpp"
#include<string>
#include<sstream>
#include<vector>
#include<queue>
#include<algorithm>
#include<thread>
#include<mutex>

template<typename T>
T calcAverage(const std::vector<T>& data)
{   
    T total=0;
    for(size_t i=0;i<data.size();++i)
    {
        total += data[i];
    }
    return total/data.size();
}

class temp_monitor:public rclcpp::Node
{
public:
    temp_monitor():Node("temp_monitor_node")
    {
        RCLCPP_INFO(this->get_logger(),"监督节点创建");
        std::vector<int>v1={1,2,3,4};
        RCLCPP_INFO(this->get_logger(),"calcAverage测试,int,1,2,3,4,Average_Temperature=%d",calcAverage(v1));
        std::vector<double>v2={1.5,2.5,3.5,4.5};
        RCLCPP_INFO(this->get_logger(),"calcAverage测试,double,1.5,2.5,3.5,4.5,Average_Temperature=%.1f",calcAverage(v2));
        monitor_ = this->create_subscription<std_msgs::msg::Float64>("device_temperature",10,std::bind(&temp_monitor::callback,this,std::placeholders::_1));
    }
private:
    void callback(const std_msgs::msg::Float64 &msg)
    {
        d.push_back(msg.data);
        if(d.size() > 4)
        {
            d.pop_front();
        }

        // double Average_Temperature=0;
        // int Integer_Average=0;
        // if (d.size() >= 4)
        // {
        //     std::vector<double> v(d.begin(), d.end());
        //     // 平均温度计算
        //     Average_Temperature = calcAverage(v);

        //     // 整数平均温度获取
        //     std::vector<int> v_;
        //     for (size_t i = 0; i < d.size(); ++i)
        //     {
        //         v_.push_back(static_cast<int>(v[i]));//重点，不能直接用v_[i]赋值
        //     }
        //     Integer_Average = calcAverage(v_);
        // }

        // 输出编写
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < d.size(); ++i)
        {
            oss << d[i];
            if (i < d.size() - 1)
            {
                oss << ", ";
            }
        }
        oss << "]";

        // 统计
        /*
        double Temperature_Range = 0, max = 0, min = 0;
        int CRITICAL_times = 0, WARNING_times = 0;
        std::string System_State;
        std::string Current_Level;

        if (d.size() >= 4)
        {
            // 找出元素最大最小值，统计温差
            if (!d.empty())
            {
                max = *std::max_element(d.begin(), d.end());
                min = *std::min_element(d.begin(), d.end());
                Temperature_Range = max - min;
            }

            // 分级告警
            if (msg.data >= 30.0)
            {
                Current_Level = "CRITICAL";
            }
            else if (msg.data >= 28.0 && msg.data < 30.0)
            {
                Current_Level = "WARNING";
            }
            else if (msg.data < 28.0)
            {
                Current_Level = "NORMAL";
            }

            // 保存告警
            v_t.reserve(4);
            v_t.push_back(Current_Level);
            if (v_t.size() > 4)
            {
                v_t.erase(v_t.begin());
            }

            // 输出系统状态
            for (size_t i = 0; i < v_t.size(); ++i)
            {
                if (v_t[i] == "CRITICAL")
                {
                    ++CRITICAL_times;
                }
                if (v_t[i] == "WARNING")
                {
                    ++WARNING_times;
                }
            }
            if (CRITICAL_times >= 1 || WARNING_times >= 2)
            {
                System_State = "UNSTABLE";
            }
            else
            {
                System_State = "STABLE";
            }
        }
*/
        //最终打印
        if(d.size()<4)
        {
        RCLCPP_INFO(this->get_logger(),"\nReceive Temperature: %.1f\nWindow: %s\nWindow not full, collecting data...",msg.data, oss.str().c_str());
        }
        /*else
        {
        RCLCPP_INFO(this->get_logger(),"\nReceive Temperature: %.1f\nWindow: %s\nWindow Max: %.1f\nWindow Min: %.1f\nTemperature Range: %.1f\nAverage Temperature: %.1f\nInteger Average (verification): %d\nCurrent Level: %s\nSystem State: %s\n"
        ,msg.data, oss.str().c_str(),max,min,Temperature_Range,Average_Temperature,Integer_Average,Current_Level.c_str(),System_State.c_str());
        }*/
        else
        {
            std::thread thread1(&temp_monitor::analyzeWindow,this,msg);
            thread1.join();
        }
    }

    void analyzeWindow(const std_msgs::msg::Float64 &msg)
    {
        std::lock_guard<std::mutex> lg(mut);

        double Temperature_Range = 0, max = 0, min = 0;
        int CRITICAL_times = 0, WARNING_times = 0;
        std::string System_State;
        std::string Current_Level;

        double Average_Temperature = 0;
        int Integer_Average = 0;
        std::vector<double> v(d.begin(), d.end());
        // 平均温度计算
        Average_Temperature = calcAverage(v);

        // 整数平均温度获取
        std::vector<int> v_;
        for (size_t i = 0; i < d.size(); ++i)
        {
            v_.push_back(static_cast<int>(v[i])); // 重点，不能直接用v_[i]赋值
        }
        Integer_Average = calcAverage(v_);
        // 找出元素最大最小值，统计温差
        if (!d.empty())
        {
            max = *std::max_element(d.begin(), d.end());
            min = *std::min_element(d.begin(), d.end());
            Temperature_Range = max - min;
        }

        // 分级告警
        if (msg.data >= 30.0)
        {
            Current_Level = "CRITICAL";
        }
        else if (msg.data >= 28.0 && msg.data < 30.0)
        {
            Current_Level = "WARNING";
        }
        else if (msg.data < 28.0)
        {
            Current_Level = "NORMAL";
        }

        // 保存告警
        v_t.reserve(4);
        v_t.push_back(Current_Level);
        if (v_t.size() > 4)
        {
            v_t.erase(v_t.begin());
        }

        // 输出系统状态
        for (size_t i = 0; i < v_t.size(); ++i)
        {
            if (v_t[i] == "CRITICAL")
            {
                ++CRITICAL_times;
            }
            if (v_t[i] == "WARNING")
            {
                ++WARNING_times;
            }
        }
        if (CRITICAL_times >= 1 || WARNING_times >= 2)
        {
            System_State = "UNSTABLE";
        }
        else
        {
            System_State = "STABLE";
        }

        // 输出编写
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < d.size(); ++i)
        {
            oss << d[i];
            if (i < d.size() - 1)
            {
                oss << ", ";
            }
        }
        oss << "]";

        RCLCPP_INFO(this->get_logger(), "\nReceive Temperature: %.1f\nWindow: %s\nWindow Max: %.1f\nWindow Min: %.1f\nTemperature Range: %.1f\nAverage Temperature: %.1f\nInteger Average (verification): %d\nCurrent Level: %s\nSystem State: %s\n"
        , msg.data, oss.str().c_str(), max, min, Temperature_Range, Average_Temperature, Integer_Average, Current_Level.c_str(), System_State.c_str());
    }

    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr monitor_;
    std::deque<double>d;
    std::vector<std::string>v_t;
    std::mutex mut;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc,argv);
  auto node = std::make_shared<temp_monitor>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
