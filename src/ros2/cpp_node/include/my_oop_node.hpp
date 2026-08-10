#ifndef _MY_NODE_HPP__
#define _MY_NODE_HPP__

#include "rclcpp/rclcpp.hpp"

class MyNode: public rclcpp::Node{
public:
    MyNode();
private:
    int counter_;
    void callback();
    rclcpp::TimerBase::SharedPtr timer_;
};

#endif