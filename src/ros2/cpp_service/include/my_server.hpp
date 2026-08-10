#ifndef _MY_SERVER_HPP__
#define _MY_SERVER_HPP__

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

typedef example_interfaces::srv::AddTwoInts AddTwoInts_msg;
using std::placeholders::_1;
using std::placeholders::_2;

class Server: public rclcpp::Node{
public:
    Server();
    void serverCallback(const AddTwoInts_msg::Request::SharedPtr request, 
                        const AddTwoInts_msg::Response::SharedPtr response);
private:
    rclcpp::Service<AddTwoInts_msg>::SharedPtr server_;
};

#endif