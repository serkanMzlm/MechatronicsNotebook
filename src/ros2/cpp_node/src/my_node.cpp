#include "rclcpp/rclcpp.hpp"

int main(int argc, char** args){
    rclcpp::init(argc, args);
    auto node = std::make_shared<rclcpp::Node>("cpp_my_node");
    RCLCPP_INFO(node->get_logger(), "INFO");
    RCLCPP_DEBUG(node->get_logger(), "DEBUG");
    RCLCPP_WARN(node->get_logger(), "WARN");
    RCLCPP_ERROR(node->get_logger(), "ERROR");
    rclcpp::spin(node);
    rclcpp::shutdown();
}