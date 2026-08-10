
#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using add2Int = example_interfaces::srv::AddTwoInts;
class Client: public rclcpp::Node{
private:
  rclcpp::Client<add2Int>::SharedPtr client_; 
  std::vector<std::thread> threads;

public:
  Client();
  ~Client();
  void clientCallback(int a, int b);
};


#endif