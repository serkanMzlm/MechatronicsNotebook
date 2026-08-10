#include <rclcpp/rclcpp.hpp>
#include "example_interfaces/msg/int32.hpp"
#include "example_interfaces/srv/set_bool.hpp"

class ros: public rclcpp::Node
{
private:
    rclcpp::Publisher<example_interfaces::msg::Int32>::SharedPtr timer_publisher;
    rclcpp::TimerBase::SharedPtr _timer;
    rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr reset_service;
    unsigned int _counter;
    std::vector<std::thread> _threads;


public:
    ros(): Node("Qt_ros"), _counter(0)
    {
        timer_publisher = this->create_publisher<example_interfaces::msg::Int32>("Timer", 10);
        reset_service   = this->create_service<example_interfaces::srv::SetBool>("Reset_Timer", std::bind(&ros::resetCallback,
                                                                                 this, std::placeholders::_1, std::placeholders::_2));
        _timer = this->create_wall_timer(std::chrono::seconds(2),
                                            std::bind(&ros::timerCallback,this));
    }
    void timerCallback()
    {
        _counter++;
        RCLCPP_INFO(this->get_logger(), "%d",_counter);
        auto msg = example_interfaces::msg::Int32();
        msg.data = _counter;
        timer_publisher->publish(msg);
    }

    void resetCallback(const example_interfaces::srv::SetBool::Request::SharedPtr _request, const example_interfaces::srv::SetBool::Response::SharedPtr _response)
    {
        _response->success = true;
        _response->message = "Timer Reset ...";
        RCLCPP_WARN(this->get_logger(), "Timer Reset...");

        if(_request->data)
        {
            _timer = 0;
        }

    }

};

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ros>());
    rclcpp::shutdown();

}