#include "rclcpp/rclcpp.hpp"
#include "my_oop_node.hpp"

MyNode::MyNode(): Node("oop_node_cpp"), counter_(0){
    RCLCPP_INFO(this->get_logger(), "INFO");
    RCLCPP_INFO(rclcpp::get_logger("new logger"), "new Logger...");
    RCLCPP_DEBUG(this->get_logger(), "DEBUG");
    RCLCPP_WARN(this->get_logger(), "WARN");
    RCLCPP_ERROR(this->get_logger(), "ERROR");
    timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&MyNode::callback, this));
}

void MyNode::callback(){
    counter_ ++;
    RCLCPP_INFO(this->get_logger(),"%d) Hello", counter_);
}

int main(int argc, char ** args){
    rclcpp::init(argc, args);
    rclcpp::spin(std::make_shared<MyNode>());
    rclcpp::shutdown();
}