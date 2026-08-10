#include "rclcpp/rclcpp.hpp"
// #include "rcl_interfaces/srv/get_logger_levels.hpp"
// #include "rcl_interfaces/srv/set_logger_levels.hpp"

class LoggerNode:  public rclcpp::Node{
public:
    LoggerNode(): Node("logger_node_cpp"){        
        int number = 1;
        RCLCPP_DEBUG(this->get_logger(), "Logger Debug: %d", number);
        RCLCPP_INFO(this->get_logger(), "Logger Info: %d", number);
        RCLCPP_WARN(this->get_logger(), "Logger Warn: %d", number);
        RCLCPP_ERROR(this->get_logger(), "Logger Error: %d", number);
        RCLCPP_DEBUG_STREAM(this->get_logger(), "Logger Debug: " << number);
        RCLCPP_INFO_STREAM(this->get_logger(), "Logger Info: " << number);
        RCLCPP_WARN_STREAM(this->get_logger(), "Logger Warn: " << number);
        RCLCPP_ERROR_STREAM(this->get_logger(), "Logger Error: " << number);
    }
};

int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<LoggerNode>());
    rclcpp::shutdown();
}